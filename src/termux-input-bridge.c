/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "termux-input-bridge.h"
#include "libinput.h"
#include "evdev.h"
#include <termux/render/render.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <string.h>

struct termux_input_bridge {
    struct libinput *libinput;
    pthread_mutex_t mutex;
};

static struct termux_input_bridge *global_bridge = NULL;

int
termux_input_bridge_dispatch(int termux_fd)
{
    lorieEvent event;
    ssize_t bytes_read;
    int events_processed = 0;
    
    if (!global_bridge || termux_fd < 0) {
        return -1;
    }
    
    /* Read all available events from termux-display-client */
    while ((bytes_read = read(termux_fd, &event, sizeof(event))) == sizeof(event)) {
        pthread_mutex_lock(&global_bridge->mutex);
        process_android_event(global_bridge, &event);
        pthread_mutex_unlock(&global_bridge->mutex);
        events_processed++;
    }
    
    if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        return -1;  /* Real error */
    }
    
    return events_processed;
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
    
    if (pthread_mutex_init(&global_bridge->mutex, NULL) != 0) {
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
    
    pthread_mutex_destroy(&global_bridge->mutex);
    free(global_bridge);
    global_bridge = NULL;
}

int
termux_input_bridge_is_connected(void)
{
    return global_bridge && global_bridge->conn_fd >= 0;
}