/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 * 
 * Compatible libinput API implementation for Termux/Android
 */

#ifndef LIBINPUT_H
#define LIBINPUT_H

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup base Initialization and manipulation of libinput contexts
 */

struct libinput;
struct libinput_device;
struct libinput_event;
struct libinput_seat;
struct udev;

enum libinput_event_type {
	LIBINPUT_EVENT_NONE = 0,
	LIBINPUT_EVENT_DEVICE_ADDED,
	LIBINPUT_EVENT_DEVICE_REMOVED,
	LIBINPUT_EVENT_KEYBOARD_KEY = 300,
	LIBINPUT_EVENT_POINTER_MOTION = 400,
	LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE,
	LIBINPUT_EVENT_POINTER_BUTTON,
	LIBINPUT_EVENT_POINTER_SCROLL_WHEEL,
	LIBINPUT_EVENT_POINTER_SCROLL_FINGER,
	LIBINPUT_EVENT_POINTER_SCROLL_CONTINUOUS,
	LIBINPUT_EVENT_TOUCH_DOWN = 500,
	LIBINPUT_EVENT_TOUCH_UP,
	LIBINPUT_EVENT_TOUCH_MOTION,
	LIBINPUT_EVENT_TOUCH_CANCEL,
	LIBINPUT_EVENT_TOUCH_FRAME,
	LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN = 600,
	LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE,
	LIBINPUT_EVENT_GESTURE_SWIPE_END,
	LIBINPUT_EVENT_GESTURE_PINCH_BEGIN,
	LIBINPUT_EVENT_GESTURE_PINCH_UPDATE,
	LIBINPUT_EVENT_GESTURE_PINCH_END,
	LIBINPUT_EVENT_GESTURE_HOLD_BEGIN,
	LIBINPUT_EVENT_GESTURE_HOLD_END,
	LIBINPUT_EVENT_TABLET_TOOL_AXIS = 700,
	LIBINPUT_EVENT_TABLET_TOOL_PROXIMITY,
	LIBINPUT_EVENT_TABLET_TOOL_TIP,
	LIBINPUT_EVENT_TABLET_TOOL_BUTTON,
	LIBINPUT_EVENT_TABLET_PAD_BUTTON = 800,
	LIBINPUT_EVENT_TABLET_PAD_RING,
	LIBINPUT_EVENT_TABLET_PAD_STRIP,
	LIBINPUT_EVENT_SWITCH_TOGGLE = 900,
};

enum libinput_device_capability {
	LIBINPUT_DEVICE_CAP_KEYBOARD = 0,
	LIBINPUT_DEVICE_CAP_POINTER = 1,
	LIBINPUT_DEVICE_CAP_TOUCH = 2,
	LIBINPUT_DEVICE_CAP_TABLET_TOOL = 3,
	LIBINPUT_DEVICE_CAP_TABLET_PAD = 4,
	LIBINPUT_DEVICE_CAP_GESTURE = 5,
	LIBINPUT_DEVICE_CAP_SWITCH = 6,
};

enum libinput_key_state {
	LIBINPUT_KEY_STATE_RELEASED = 0,
	LIBINPUT_KEY_STATE_PRESSED = 1
};

enum libinput_button_state {
	LIBINPUT_BUTTON_STATE_RELEASED = 0,
	LIBINPUT_BUTTON_STATE_PRESSED = 1
};

enum libinput_pointer_axis {
	LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL = 0,
	LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL = 1,
};

enum libinput_pointer_axis_source {
	LIBINPUT_POINTER_AXIS_SOURCE_WHEEL = 1,
	LIBINPUT_POINTER_AXIS_SOURCE_FINGER,
	LIBINPUT_POINTER_AXIS_SOURCE_CONTINUOUS,
	LIBINPUT_POINTER_AXIS_SOURCE_WHEEL_TILT,
};

enum libinput_tablet_tool_tip_state {
	LIBINPUT_TABLET_TOOL_TIP_UP = 0,
	LIBINPUT_TABLET_TOOL_TIP_DOWN = 1,
};

enum libinput_tablet_pad_ring_axis_source {
	LIBINPUT_TABLET_PAD_RING_SOURCE_UNKNOWN = 1,
	LIBINPUT_TABLET_PAD_RING_SOURCE_FINGER,
};

enum libinput_tablet_pad_strip_axis_source {
	LIBINPUT_TABLET_PAD_STRIP_SOURCE_UNKNOWN = 1,
	LIBINPUT_TABLET_PAD_STRIP_SOURCE_FINGER,
};

enum libinput_switch {
	LIBINPUT_SWITCH_LID = 1,
	LIBINPUT_SWITCH_TABLET_MODE,
};

enum libinput_switch_state {
	LIBINPUT_SWITCH_STATE_OFF = 0,
	LIBINPUT_SWITCH_STATE_ON = 1,
};

enum libinput_log_priority {
	LIBINPUT_LOG_PRIORITY_DEBUG = 10,
	LIBINPUT_LOG_PRIORITY_INFO = 20,
	LIBINPUT_LOG_PRIORITY_ERROR = 30,
};

typedef void (*libinput_log_handler)(struct libinput *libinput,
				     enum libinput_log_priority priority,
				     const char *format, va_list args)
	__attribute__((format(printf, 3, 0)));

struct libinput_interface {
	int (*open_restricted)(const char *path, int flags, void *user_data);
	void (*close_restricted)(int fd, void *user_data);
};

/* Event-specific structs */
struct libinput_event_keyboard;
struct libinput_event_pointer;
struct libinput_event_touch;
struct libinput_event_gesture;
struct libinput_event_tablet_tool;
struct libinput_event_tablet_pad;
struct libinput_event_switch;

/* Core API functions */
struct libinput *
libinput_udev_create_context(const struct libinput_interface *interface,
			     void *user_data,
			     struct udev *udev);

struct libinput *
libinput_path_create_context(const struct libinput_interface *interface,
			     void *user_data);

/* Termux-specific context creation with external event fd */
struct libinput *
libinput_termux_create_context(const struct libinput_interface *interface,
			       void *user_data,
			       int termux_event_fd);

struct libinput_device *
libinput_path_add_device(struct libinput *libinput,
			 const char *path);

void
libinput_path_remove_device(struct libinput_device *device);

int
libinput_udev_assign_seat(struct libinput *libinput,
			  const char *seat_id);

void
libinput_suspend(struct libinput *libinput);

int
libinput_resume(struct libinput *libinput);

struct libinput *
libinput_ref(struct libinput *libinput);

struct libinput *
libinput_unref(struct libinput *libinput);

void
libinput_destroy(struct libinput *libinput);

void
libinput_set_user_data(struct libinput *libinput,
		       void *user_data);

void *
libinput_get_user_data(struct libinput *libinput);

int
libinput_get_fd(struct libinput *libinput);

int
libinput_dispatch(struct libinput *libinput);

struct libinput_event *
libinput_get_event(struct libinput *libinput);

enum libinput_event_type
libinput_next_event_type(struct libinput *libinput);

void
libinput_log_set_priority(struct libinput *libinput,
			  enum libinput_log_priority priority);

enum libinput_log_priority
libinput_log_get_priority(const struct libinput *libinput);

void
libinput_log_set_handler(struct libinput *libinput,
			 libinput_log_handler log_handler);

/* Event handling functions */
void
libinput_event_destroy(struct libinput_event *event);

enum libinput_event_type
libinput_event_get_type(struct libinput_event *event);

struct libinput *
libinput_event_get_context(struct libinput_event *event);

struct libinput_device *
libinput_event_get_device(struct libinput_event *event);

struct libinput_event_pointer *
libinput_event_get_pointer_event(struct libinput_event *event);

struct libinput_event_keyboard *
libinput_event_get_keyboard_event(struct libinput_event *event);

struct libinput_event_touch *
libinput_event_get_touch_event(struct libinput_event *event);

struct libinput_event_gesture *
libinput_event_get_gesture_event(struct libinput_event *event);

/* Device functions */
struct libinput_device *
libinput_device_ref(struct libinput_device *device);

struct libinput_device *
libinput_device_unref(struct libinput_device *device);

const char *
libinput_device_get_name(struct libinput_device *device);

const char *
libinput_device_get_sysname(struct libinput_device *device);

const char *
libinput_device_get_output_name(struct libinput_device *device);

struct libinput_seat *
libinput_device_get_seat(struct libinput_device *device);

void
libinput_device_set_user_data(struct libinput_device *device, void *user_data);

void *
libinput_device_get_user_data(struct libinput_device *device);

int
libinput_device_has_capability(struct libinput_device *device,
			       enum libinput_device_capability capability);

int
libinput_device_get_size(struct libinput_device *device,
			 double *width,
			 double *height);

int
libinput_device_keyboard_has_key(struct libinput_device *device,
				 uint32_t code);

unsigned int
libinput_device_get_id_product(struct libinput_device *device);

unsigned int
libinput_device_get_id_vendor(struct libinput_device *device);

unsigned int
libinput_device_get_id_bustype(struct libinput_device *device);

/* Keyboard event functions */
uint32_t
libinput_event_keyboard_get_key(struct libinput_event_keyboard *event);

enum libinput_key_state
libinput_event_keyboard_get_key_state(struct libinput_event_keyboard *event);

uint64_t
libinput_event_keyboard_get_time_usec(struct libinput_event_keyboard *event);

/* Pointer event functions */
uint64_t
libinput_event_pointer_get_time_usec(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_dx(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_dy(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_dx_unaccelerated(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_dy_unaccelerated(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_absolute_x(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_absolute_y(struct libinput_event_pointer *event);

double
libinput_event_pointer_get_absolute_x_transformed(struct libinput_event_pointer *event,
						  uint32_t width);

double
libinput_event_pointer_get_absolute_y_transformed(struct libinput_event_pointer *event,
						  uint32_t height);

uint32_t
libinput_event_pointer_get_button(struct libinput_event_pointer *event);

enum libinput_button_state
libinput_event_pointer_get_button_state(struct libinput_event_pointer *event);

int
libinput_event_pointer_has_axis(struct libinput_event_pointer *event,
				enum libinput_pointer_axis axis);

double
libinput_event_pointer_get_scroll_value(struct libinput_event_pointer *event,
					enum libinput_pointer_axis axis);

int32_t
libinput_event_pointer_get_scroll_value_v120(struct libinput_event_pointer *event,
					     enum libinput_pointer_axis axis);

/* Touch event functions */
uint64_t
libinput_event_touch_get_time_usec(struct libinput_event_touch *event);

int32_t
libinput_event_touch_get_seat_slot(struct libinput_event_touch *event);

double
libinput_event_touch_get_x(struct libinput_event_touch *event);

double
libinput_event_touch_get_y(struct libinput_event_touch *event);

double
libinput_event_touch_get_x_transformed(struct libinput_event_touch *event,
				       uint32_t width);

double
libinput_event_touch_get_y_transformed(struct libinput_event_touch *event,
				       uint32_t height);

/* Gesture event functions */
uint64_t
libinput_event_gesture_get_time_usec(struct libinput_event_gesture *event);

int
libinput_event_gesture_get_finger_count(struct libinput_event_gesture *event);

double
libinput_event_gesture_get_dx_unaccelerated(struct libinput_event_gesture *event);

double
libinput_event_gesture_get_dy_unaccelerated(struct libinput_event_gesture *event);

double
libinput_event_gesture_get_scale(struct libinput_event_gesture *event);

double
libinput_event_gesture_get_angle_delta(struct libinput_event_gesture *event);

int
libinput_event_gesture_get_cancelled(struct libinput_event_gesture *event);

/* Mock udev structures for compatibility */
struct udev {
    int dummy;
};

struct udev_device {
    char *syspath;
    char *sysname;
    char properties[10][2][256];
    int num_properties;
};

/* Mock udev functions */
struct udev *udev_new(void);
struct udev *udev_ref(struct udev *udev);
struct udev *udev_unref(struct udev *udev);
struct udev_device *udev_device_new_from_subsystem_sysname(struct udev *udev,
                                                           const char *subsystem,
                                                           const char *sysname);
struct udev_device *udev_device_ref(struct udev_device *device);
struct udev_device *udev_device_unref(struct udev_device *device);
const char *udev_device_get_syspath(struct udev_device *device);
const char *udev_device_get_sysname(struct udev_device *device);
const char *udev_device_get_property_value(struct udev_device *device, const char *key);
struct udev_device *libinput_device_get_udev_device(struct libinput_device *device);

#ifdef __cplusplus
}
#endif

#endif /* LIBINPUT_H */