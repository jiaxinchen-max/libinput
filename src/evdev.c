/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "evdev.h"
#include "libinput-private.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Event handling and device management */

void
libinput_event_destroy(struct libinput_event *event)
{
    if (event)
        free(event);
}

enum libinput_event_type
libinput_event_get_type(struct libinput_event *event)
{
    return event ? event->type : LIBINPUT_EVENT_NONE;
}

struct libinput *
libinput_event_get_context(struct libinput_event *event)
{
    return event ? event->libinput : NULL;
}

struct libinput_device *
libinput_event_get_device(struct libinput_event *event)
{
    return event ? event->device : NULL;
}

struct libinput_event_pointer *
libinput_event_get_pointer_event(struct libinput_event *event)
{
    if (!event)
        return NULL;
        
    switch (event->type) {
    case LIBINPUT_EVENT_POINTER_MOTION:
    case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
    case LIBINPUT_EVENT_POINTER_BUTTON:
    case LIBINPUT_EVENT_POINTER_SCROLL_WHEEL:
    case LIBINPUT_EVENT_POINTER_SCROLL_FINGER:
    case LIBINPUT_EVENT_POINTER_SCROLL_CONTINUOUS:
        return &event->event.pointer;
    default:
        return NULL;
    }
}

struct libinput_event_keyboard *
libinput_event_get_keyboard_event(struct libinput_event *event)
{
    if (!event || event->type != LIBINPUT_EVENT_KEYBOARD_KEY)
        return NULL;
        
    return &event->event.keyboard;
}

struct libinput_event_touch *
libinput_event_get_touch_event(struct libinput_event *event)
{
    if (!event)
        return NULL;
        
    switch (event->type) {
    case LIBINPUT_EVENT_TOUCH_DOWN:
    case LIBINPUT_EVENT_TOUCH_UP:
    case LIBINPUT_EVENT_TOUCH_MOTION:
    case LIBINPUT_EVENT_TOUCH_CANCEL:
    case LIBINPUT_EVENT_TOUCH_FRAME:
        return &event->event.touch;
    default:
        return NULL;
    }
}

struct libinput_event_gesture *
libinput_event_get_gesture_event(struct libinput_event *event)
{
    if (!event)
        return NULL;
        
    switch (event->type) {
    case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
    case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
    case LIBINPUT_EVENT_GESTURE_SWIPE_END:
    case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
    case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE:
    case LIBINPUT_EVENT_GESTURE_PINCH_END:
    case LIBINPUT_EVENT_GESTURE_HOLD_BEGIN:
    case LIBINPUT_EVENT_GESTURE_HOLD_END:
        return &event->event.gesture;
    default:
        return NULL;
    }
}

struct libinput_event *
libinput_event_create(struct libinput_device *device,
                      enum libinput_event_type type,
                      uint64_t time_usec)
{
    struct libinput_event *event;
    
    event = calloc(1, sizeof(*event));
    if (!event)
        return NULL;
        
    event->type = type;
    event->device = device;
    event->libinput = device ? device->seat->libinput : NULL;
    event->time_usec = time_usec;
    
    return event;
}

/* Keyboard event functions */
uint32_t
libinput_event_keyboard_get_key(struct libinput_event_keyboard *event)
{
    return event ? event->key : 0;
}

enum libinput_key_state
libinput_event_keyboard_get_key_state(struct libinput_event_keyboard *event)
{
    return event ? event->state : LIBINPUT_KEY_STATE_RELEASED;
}

uint64_t
libinput_event_keyboard_get_time_usec(struct libinput_event_keyboard *event)
{
    return event ? ((struct libinput_event *)event)->time_usec : 0;
}

/* Pointer event functions */
uint64_t
libinput_event_pointer_get_time_usec(struct libinput_event_pointer *event)
{
    return event ? ((struct libinput_event *)event)->time_usec : 0;
}

double
libinput_event_pointer_get_dx(struct libinput_event_pointer *event)
{
    return event ? event->dx : 0.0;
}

double
libinput_event_pointer_get_dy(struct libinput_event_pointer *event)
{
    return event ? event->dy : 0.0;
}

double
libinput_event_pointer_get_dx_unaccelerated(struct libinput_event_pointer *event)
{
    return event ? event->dx_unaccel : 0.0;
}

double
libinput_event_pointer_get_dy_unaccelerated(struct libinput_event_pointer *event)
{
    return event ? event->dy_unaccel : 0.0;
}

double
libinput_event_pointer_get_absolute_x(struct libinput_event_pointer *event)
{
    return event ? event->absolute_x : 0.0;
}

double
libinput_event_pointer_get_absolute_y(struct libinput_event_pointer *event)
{
    return event ? event->absolute_y : 0.0;
}

double
libinput_event_pointer_get_absolute_x_transformed(struct libinput_event_pointer *event,
                                                  uint32_t width)
{
    if (!event)
        return 0.0;
        
    return event->absolute_x * width / 100.0;
}

double
libinput_event_pointer_get_absolute_y_transformed(struct libinput_event_pointer *event,
                                                  uint32_t height)
{
    if (!event)
        return 0.0;
        
    return event->absolute_y * height / 100.0;
}

uint32_t
libinput_event_pointer_get_button(struct libinput_event_pointer *event)
{
    return event ? event->button : 0;
}

enum libinput_button_state
libinput_event_pointer_get_button_state(struct libinput_event_pointer *event)
{
    return event ? event->button_state : LIBINPUT_BUTTON_STATE_RELEASED;
}

int
libinput_event_pointer_has_axis(struct libinput_event_pointer *event,
                                enum libinput_pointer_axis axis)
{
    if (!event)
        return 0;
        
    switch (axis) {
    case LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL:
        return event->has_scroll_h;
    case LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL:
        return event->has_scroll_v;
    default:
        return 0;
    }
}

double
libinput_event_pointer_get_scroll_value(struct libinput_event_pointer *event,
                                        enum libinput_pointer_axis axis)
{
    if (!event)
        return 0.0;
        
    switch (axis) {
    case LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL:
        return event->scroll_h_value;
    case LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL:
        return event->scroll_v_value;
    default:
        return 0.0;
    }
}

int32_t
libinput_event_pointer_get_scroll_value_v120(struct libinput_event_pointer *event,
                                             enum libinput_pointer_axis axis)
{
    if (!event)
        return 0;
        
    switch (axis) {
    case LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL:
        return event->scroll_h_value_v120;
    case LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL:
        return event->scroll_v_value_v120;
    default:
        return 0;
    }
}

/* Touch event functions */
uint64_t
libinput_event_touch_get_time_usec(struct libinput_event_touch *event)
{
    return event ? ((struct libinput_event *)event)->time_usec : 0;
}

int32_t
libinput_event_touch_get_seat_slot(struct libinput_event_touch *event)
{
    return event ? event->slot : -1;
}

double
libinput_event_touch_get_x(struct libinput_event_touch *event)
{
    return event ? event->x : 0.0;
}

double
libinput_event_touch_get_y(struct libinput_event_touch *event)
{
    return event ? event->y : 0.0;
}

double
libinput_event_touch_get_x_transformed(struct libinput_event_touch *event,
                                       uint32_t width)
{
    if (!event)
        return 0.0;
        
    return event->x * width / 100.0;
}

double
libinput_event_touch_get_y_transformed(struct libinput_event_touch *event,
                                       uint32_t height)
{
    if (!event)
        return 0.0;
        
    return event->y * height / 100.0;
}

/* Gesture event functions */
uint64_t
libinput_event_gesture_get_time_usec(struct libinput_event_gesture *event)
{
    return event ? ((struct libinput_event *)event)->time_usec : 0;
}

int
libinput_event_gesture_get_finger_count(struct libinput_event_gesture *event)
{
    return event ? event->finger_count : 0;
}

double
libinput_event_gesture_get_dx_unaccelerated(struct libinput_event_gesture *event)
{
    return event ? event->dx_unaccel : 0.0;
}

double
libinput_event_gesture_get_dy_unaccelerated(struct libinput_event_gesture *event)
{
    return event ? event->dy_unaccel : 0.0;
}

double
libinput_event_gesture_get_scale(struct libinput_event_gesture *event)
{
    return event ? event->scale : 1.0;
}

double
libinput_event_gesture_get_angle_delta(struct libinput_event_gesture *event)
{
    return event ? event->angle_delta : 0.0;
}

int
libinput_event_gesture_get_cancelled(struct libinput_event_gesture *event)
{
    return event ? event->cancelled : 0;
}

/* Event creation functions for termux input bridge */

static uint64_t
get_current_time_usec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000ULL;
}

struct libinput_event *
create_keyboard_event(struct libinput *libinput,
                     struct libinput_device *device,
                     enum libinput_event_type type,
                     uint32_t key,
                     enum libinput_key_state state)
{
    struct libinput_event *event;
    
    event = libinput_event_create(device, type, get_current_time_usec());
    if (!event)
        return NULL;
        
    event->event.keyboard.key = key;
    event->event.keyboard.state = state;
    
    return event;
}

struct libinput_event *
create_pointer_event(struct libinput *libinput,
                    struct libinput_device *device,
                    enum libinput_event_type type,
                    double dx, double dy)
{
    struct libinput_event *event;
    
    event = libinput_event_create(device, type, get_current_time_usec());
    if (!event)
        return NULL;
        
    if (type == LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE) {
        event->event.pointer.absolute_x = dx;
        event->event.pointer.absolute_y = dy;
    } else {
        event->event.pointer.dx = dx;
        event->event.pointer.dy = dy;
        event->event.pointer.dx_unaccel = dx;
        event->event.pointer.dy_unaccel = dy;
    }
    
    return event;
}

struct libinput_event *
create_pointer_button_event(struct libinput *libinput,
                           struct libinput_device *device,
                           uint32_t button,
                           enum libinput_button_state state)
{
    struct libinput_event *event;
    
    event = libinput_event_create(device, LIBINPUT_EVENT_POINTER_BUTTON, get_current_time_usec());
    if (!event)
        return NULL;
        
    event->event.pointer.button = button;
    event->event.pointer.button_state = state;
    
    return event;
}

struct libinput_event *
create_touch_event(struct libinput *libinput,
                  struct libinput_device *device,
                  enum libinput_event_type type,
                  int32_t slot,
                  double x, double y)
{
    struct libinput_event *event;
    
    event = libinput_event_create(device, type, get_current_time_usec());
    if (!event)
        return NULL;
        
    event->event.touch.slot = slot;
    event->event.touch.x = x;
    event->event.touch.y = y;
    
    return event;
}

void
queue_event(struct libinput *libinput, struct libinput_event *event)
{
    if (!libinput || !event)
        return;
        
    pthread_mutex_lock(&libinput->event_mutex);
    
    /* Add to tail of queue */
    if (libinput->event_queue_tail) {
        libinput->event_queue_tail->next = event;
    } else {
        libinput->event_queue_head = event;
    }
    libinput->event_queue_tail = event;
    event->next = NULL;
    
    pthread_mutex_unlock(&libinput->event_mutex);
    
    /* KWin monitors the termux fd directly, no need to signal */
}