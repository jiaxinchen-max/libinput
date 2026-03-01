/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "termux-input-bridge.h"
#include "libinput.h"
#include "evdev.h"
#include <render.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <string.h>

struct termux_input_bridge {
    struct libinput *libinput;
    pthread_t input_thread;
    int conn_fd;
    int event_fd;
    int running;
    pthread_mutex_t mutex;
};

static struct termux_input_bridge *global_bridge = NULL;

void
termux_input_bridge_set_eventfd(int event_fd)
{
    if (global_bridge) {
        global_bridge->event_fd = event_fd;
    }
}

/* Convert Android event to libinput event */
static void
process_android_event(struct termux_input_bridge *bridge, const lorieEvent *event)
{
    struct libinput *libinput = bridge->libinput;
    struct libinput_device *device;
    struct libinput_event *libinput_event;
    
    if (!libinput || !event)
        return;
        
    /* Get the first available device (we create virtual devices) */
    device = libinput->devices;
    if (!device)
        return;
    
    switch (event->type) {
    case EVENT_KEY:
        {
            /* Convert Android keycode to Linux keycode */
            uint16_t android_key = event->key.key;
            uint16_t linux_key = 0;
            
            if (android_key < 304) {
                extern int android_to_linux_keycode[304];
                linux_key = android_to_linux_keycode[android_key];
            }
            
            if (linux_key == 0) {
                /* Fallback for unmapped keys */
                linux_key = android_key;
            }
            
            libinput_event = create_keyboard_event(
                libinput, device,
                LIBINPUT_EVENT_KEYBOARD_KEY,
                linux_key,
                event->key.state ? LIBINPUT_KEY_STATE_PRESSED : LIBINPUT_KEY_STATE_RELEASED
            );
            
            if (libinput_event) {
                queue_event(libinput, libinput_event);
            }
        }
        break;
        
    case EVENT_MOUSE:
        {
            if (event->mouse.relative) {
                /* Relative mouse motion */
                libinput_event = create_pointer_event(
                    libinput, device,
                    LIBINPUT_EVENT_POINTER_MOTION,
                    event->mouse.x, event->mouse.y
                );
            } else {
                /* Absolute mouse motion */
                libinput_event = create_pointer_event(
                    libinput, device,
                    LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE,
                    event->mouse.x, event->mouse.y
                );
            }
            
            if (libinput_event) {
                queue_event(libinput, libinput_event);
            }
            
            /* Handle mouse button */
            if (event->mouse.detail > 0) {
                libinput_event = create_pointer_button_event(
                    libinput, device,
                    event->mouse.detail,
                    event->mouse.down ? LIBINPUT_BUTTON_STATE_PRESSED : LIBINPUT_BUTTON_STATE_RELEASED
                );
                
                if (libinput_event) {
                    queue_event(libinput, libinput_event);
                }
            }
        }
        break;
        
    case EVENT_TOUCH:
        {
            enum libinput_event_type touch_type;
            
            switch (event->touch.type) {
            case 0: /* DOWN */
                touch_type = LIBINPUT_EVENT_TOUCH_DOWN;
                break;
            case 1: /* UP */
                touch_type = LIBINPUT_EVENT_TOUCH_UP;
                break;
            case 2: /* MOTION */
                touch_type = LIBINPUT_EVENT_TOUCH_MOTION;
                break;
            default:
                return;
            }
            
            libinput_event = create_touch_event(
                libinput, device,
                touch_type,
                event->touch.id,
                event->touch.x, event->touch.y
            );
            
            if (libinput_event) {
                queue_event(libinput, libinput_event);
            }
        }
        break;
        
    case EVENT_STYLUS:
        {
            /* Treat stylus as touch for now */
            libinput_event = create_touch_event(
                libinput, device,
                LIBINPUT_EVENT_TOUCH_MOTION,
                0, /* stylus ID */
                event->stylus.x, event->stylus.y
            );
            
            if (libinput_event) {
                queue_event(libinput, libinput_event);
            }
        }
        break;
        
    default:
        /* Ignore other event types */
        break;
    }
}

/* Input thread function */
static void *
input_thread_func(void *arg)
{
    struct termux_input_bridge *bridge = (struct termux_input_bridge *)arg;
    lorieEvent event;
    ssize_t bytes_read;
    
    while (bridge->running) {
        /* Try to connect if not connected */
        if (bridge->conn_fd < 0) {
            bridge->conn_fd = get_conn_fd();
            if (bridge->conn_fd < 0) {
                usleep(100000); /* Wait 100ms before retry */
                continue;
            }
        }
        
        /* Read event from termux-display-client */
        bytes_read = read(bridge->conn_fd, &event, sizeof(event));
        
        if (bytes_read == sizeof(event)) {
            pthread_mutex_lock(&bridge->mutex);
            process_android_event(bridge, &event);
            pthread_mutex_unlock(&bridge->mutex);
        } else if (bytes_read < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(10000); /* Wait 10ms */
                continue;
            } else {
                /* Connection error, reset */
                close(bridge->conn_fd);
                bridge->conn_fd = -1;
                usleep(100000); /* Wait 100ms before retry */
            }
        } else if (bytes_read == 0) {
            /* Connection closed */
            close(bridge->conn_fd);
            bridge->conn_fd = -1;
            usleep(100000); /* Wait 100ms before retry */
        }
    }
    
    return NULL;
}

int
termux_input_bridge_init(struct libinput *libinput)
{
    if (global_bridge) {
        return 0; /* Already initialized */
    }
    
    global_bridge = calloc(1, sizeof(struct termux_input_bridge));
    if (!global_bridge) {
        return -1;
    }
    
    global_bridge->libinput = libinput;
    global_bridge->conn_fd = -1;
    global_bridge->event_fd = -1;
    global_bridge->running = 1;
    
    if (pthread_mutex_init(&global_bridge->mutex, NULL) != 0) {
        free(global_bridge);
        global_bridge = NULL;
        return -1;
    }
    
    /* Start input thread */
    if (pthread_create(&global_bridge->input_thread, NULL, input_thread_func, global_bridge) != 0) {
        pthread_mutex_destroy(&global_bridge->mutex);
        free(global_bridge);
        global_bridge = NULL;
        return -1;
    }
    
    return 0;
}

void
termux_input_bridge_destroy(void)
{
    if (!global_bridge) {
        return;
    }
    
    global_bridge->running = 0;
    
    /* Wait for thread to finish */
    pthread_join(global_bridge->input_thread, NULL);
    
    if (global_bridge->conn_fd >= 0) {
        close(global_bridge->conn_fd);
    }
    
    pthread_mutex_destroy(&global_bridge->mutex);
    free(global_bridge);
    global_bridge = NULL;
}

int
termux_input_bridge_is_connected(void)
{
    return global_bridge && global_bridge->conn_fd >= 0;
}