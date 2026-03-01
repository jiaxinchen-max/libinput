/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "libinput.h"
#include "libinput-private.h"
#include "termux-input-bridge.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

struct libinput {
    int refcount;
    void *user_data;
    
    const struct libinput_interface *interface;
    struct udev *udev;
    
    /* Event queue */
    struct libinput_event *event_queue_head;
    struct libinput_event *event_queue_tail;
    pthread_mutex_t event_mutex;
    
    /* Devices */
    struct libinput_device *devices;
    int num_devices;
    
    /* Logging */
    enum libinput_log_priority log_priority;
    libinput_log_handler log_handler;
    
    /* State */
    int suspended;
    char *seat_id;
};

static void
libinput_log(struct libinput *libinput,
             enum libinput_log_priority priority,
             const char *format, ...)
{
    va_list args;
    
    if (!libinput->log_handler || priority < libinput->log_priority)
        return;
        
    va_start(args, format);
    libinput->log_handler(libinput, priority, format, args);
    va_end(args);
}

struct libinput *
libinput_udev_create_context(const struct libinput_interface *interface,
                             void *user_data,
                             struct udev *udev)
{
    struct libinput *libinput;
    
    if (!interface || !interface->open_restricted || !interface->close_restricted) {
        return NULL;
    }
    
    libinput = calloc(1, sizeof(*libinput));
    if (!libinput)
        return NULL;
        
    libinput->refcount = 1;
    libinput->interface = interface;
    libinput->user_data = user_data;
    libinput->udev = udev;
    libinput->log_priority = LIBINPUT_LOG_PRIORITY_ERROR;
    libinput->fd = -1;
    libinput->termux_event_fd = -1;
    
    if (pthread_mutex_init(&libinput->event_mutex, NULL) != 0) {
        free(libinput);
        return NULL;
    }
    
    /* For udev context, we don't initialize termux bridge here */
    /* Use libinput_termux_create_context() for termux integration */
    
    return libinput;
}

struct libinput *
libinput_path_create_context(const struct libinput_interface *interface,
                             void *user_data)
{
    return libinput_udev_create_context(interface, user_data, NULL);
}

struct libinput *
libinput_termux_create_context(const struct libinput_interface *interface,
                               void *user_data,
                               int termux_event_fd)
{
    struct libinput *libinput;
    
    if (!interface || !interface->open_restricted || !interface->close_restricted) {
        return NULL;
    }
    
    if (termux_event_fd < 0) {
        return NULL;
    }
    
    libinput = calloc(1, sizeof(*libinput));
    if (!libinput)
        return NULL;
        
    libinput->refcount = 1;
    libinput->interface = interface;
    libinput->user_data = user_data;
    libinput->udev = NULL;
    libinput->log_priority = LIBINPUT_LOG_PRIORITY_ERROR;
    libinput->fd = termux_event_fd;  /* Use the termux-display-client eventfd */
    
    if (pthread_mutex_init(&libinput->event_mutex, NULL) != 0) {
        free(libinput);
        return NULL;
    }
    
    /* Initialize termux input bridge with the provided fd */
    if (termux_input_bridge_init(libinput) != 0) {
        libinput_log(libinput, LIBINPUT_LOG_PRIORITY_ERROR,
                    "Failed to initialize termux input bridge");
        pthread_mutex_destroy(&libinput->event_mutex);
        free(libinput);
        return NULL;
    }
    
    return libinput;
}

struct libinput_device *
libinput_path_add_device(struct libinput *libinput,
                         const char *path)
{
    /* Create virtual device for Android */
    return libinput_device_create(libinput, "Android Virtual Device", path, 0x1234, 0x5678, 0x03);
}

void
libinput_path_remove_device(struct libinput_device *device)
{
    if (device)
        libinput_device_unref(device);
}

int
libinput_udev_assign_seat(struct libinput *libinput,
                          const char *seat_id)
{
    if (!libinput || !seat_id)
        return -1;
        
    free(libinput->seat_id);
    libinput->seat_id = strdup(seat_id);
    
    /* Create default virtual devices */
    struct libinput_device *keyboard = libinput_device_create(libinput, 
        "Android Virtual Keyboard", "android-keyboard", 0x1234, 0x5678, 0x03);
    if (keyboard) {
        libinput_device_set_capabilities(keyboard, 1, 0, 0, 0, 0, 0, 0);
        struct libinput_event *event = libinput_event_create(keyboard, 
            LIBINPUT_EVENT_DEVICE_ADDED, libinput_now_usec());
        if (event) {
            libinput_post_event(libinput, event);
        }
    }
    
    struct libinput_device *pointer = libinput_device_create(libinput,
        "Android Virtual Pointer", "android-pointer", 0x1234, 0x5679, 0x03);
    if (pointer) {
        libinput_device_set_capabilities(pointer, 0, 1, 0, 0, 0, 1, 0);
        struct libinput_event *event = libinput_event_create(pointer,
            LIBINPUT_EVENT_DEVICE_ADDED, libinput_now_usec());
        if (event) {
            libinput_post_event(libinput, event);
        }
    }
    
    struct libinput_device *touch = libinput_device_create(libinput,
        "Android Virtual Touch", "android-touch", 0x1234, 0x567A, 0x03);
    if (touch) {
        libinput_device_set_capabilities(touch, 0, 0, 1, 0, 0, 1, 0);
        struct libinput_event *event = libinput_event_create(touch,
            LIBINPUT_EVENT_DEVICE_ADDED, libinput_now_usec());
        if (event) {
            libinput_post_event(libinput, event);
        }
    }
    
    return 0;
}

void
libinput_suspend(struct libinput *libinput)
{
    if (!libinput)
        return;
        
    libinput->suspended = 1;
}

int
libinput_resume(struct libinput *libinput)
{
    if (!libinput)
        return -1;
        
    libinput->suspended = 0;
    return 0;
}

struct libinput *
libinput_ref(struct libinput *libinput)
{
    if (!libinput)
        return NULL;
        
    libinput->refcount++;
    return libinput;
}

struct libinput *
libinput_unref(struct libinput *libinput)
{
    if (!libinput)
        return NULL;
        
    libinput->refcount--;
    if (libinput->refcount == 0) {
        libinput_destroy(libinput);
        return NULL;
    }
    
    return libinput;
}

void
libinput_destroy(struct libinput *libinput)
{
    if (!libinput)
        return;
        
    /* Destroy termux input bridge */
    termux_input_bridge_destroy();
        
    /* Clean up event queue */
    pthread_mutex_lock(&libinput->event_mutex);
    while (libinput->event_queue_head) {
        struct libinput_event *event = libinput->event_queue_head;
        libinput->event_queue_head = event->next;
        libinput_event_destroy(event);
    }
    pthread_mutex_unlock(&libinput->event_mutex);
    
    /* Note: We don't close the termux eventfd as it's owned by KWin */
    
    pthread_mutex_destroy(&libinput->event_mutex);
    free(libinput->seat_id);
    free(libinput);
}

void
libinput_set_user_data(struct libinput *libinput,
                       void *user_data)
{
    if (libinput)
        libinput->user_data = user_data;
}

void *
libinput_get_user_data(struct libinput *libinput)
{
    return libinput ? libinput->user_data : NULL;
}

int
libinput_get_fd(struct libinput *libinput)
{
    /* Return the eventfd that KWin should monitor */
    return libinput ? libinput->fd : -1;
}

int
libinput_dispatch(struct libinput *libinput)
{
    if (!libinput || libinput->suspended)
        return 0;
    
    /* Read events from termux-display-client and convert them */
    if (libinput->fd >= 0) {
        termux_input_bridge_dispatch(libinput->fd);
    }
    
    return 0;
}

struct libinput_event *
libinput_get_event(struct libinput *libinput)
{
    struct libinput_event *event = NULL;
    
    if (!libinput)
        return NULL;
        
    pthread_mutex_lock(&libinput->event_mutex);
    
    if (libinput->event_queue_head) {
        event = libinput->event_queue_head;
        libinput->event_queue_head = event->next;
        
        if (!libinput->event_queue_head)
            libinput->event_queue_tail = NULL;
            
        event->next = NULL;
    }
    
    pthread_mutex_unlock(&libinput->event_mutex);
    
    return event;
}

enum libinput_event_type
libinput_next_event_type(struct libinput *libinput)
{
    enum libinput_event_type type = LIBINPUT_EVENT_NONE;
    
    if (!libinput)
        return type;
        
    pthread_mutex_lock(&libinput->event_mutex);
    
    if (libinput->event_queue_head)
        type = libinput->event_queue_head->type;
        
    pthread_mutex_unlock(&libinput->event_mutex);
    
    return type;
}

void
libinput_log_set_priority(struct libinput *libinput,
                          enum libinput_log_priority priority)
{
    if (libinput)
        libinput->log_priority = priority;
}

enum libinput_log_priority
libinput_log_get_priority(const struct libinput *libinput)
{
    return libinput ? libinput->log_priority : LIBINPUT_LOG_PRIORITY_ERROR;
}

void
libinput_log_set_handler(struct libinput *libinput,
                         libinput_log_handler log_handler)
{
    if (libinput)
        libinput->log_handler = log_handler;
}

/* Internal function to add events to the queue */
void
libinput_post_event(struct libinput *libinput, struct libinput_event *event)
{
    if (!libinput || !event)
        return;
        
    pthread_mutex_lock(&libinput->event_mutex);
    
    event->next = NULL;
    
    if (libinput->event_queue_tail) {
        libinput->event_queue_tail->next = event;
    } else {
        libinput->event_queue_head = event;
    }
    
    libinput->event_queue_tail = event;
    
    pthread_mutex_unlock(&libinput->event_mutex);
}

uint64_t
libinput_now_usec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000;
}