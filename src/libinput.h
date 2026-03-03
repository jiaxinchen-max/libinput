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
#include <libudev.h>

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
struct libinput_tablet_tool;
struct libinput_tablet_pad;
struct libinput_event_tablet_tool;
struct libinput_event_tablet_pad;
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

/* Configuration enums - matching official libinput values */
enum libinput_config_status {
	LIBINPUT_CONFIG_STATUS_SUCCESS = 0,
	LIBINPUT_CONFIG_STATUS_UNSUPPORTED,
	LIBINPUT_CONFIG_STATUS_INVALID,
};

enum libinput_config_tap_state {
	LIBINPUT_CONFIG_TAP_DISABLED,
	LIBINPUT_CONFIG_TAP_ENABLED,
};

enum libinput_config_tap_button_map {
	LIBINPUT_CONFIG_TAP_MAP_LRM,
	LIBINPUT_CONFIG_TAP_MAP_LMR,
};

enum libinput_config_drag_state {
	LIBINPUT_CONFIG_DRAG_DISABLED,
	LIBINPUT_CONFIG_DRAG_ENABLED,
};

enum libinput_config_drag_lock_state {
	LIBINPUT_CONFIG_DRAG_LOCK_DISABLED,
	LIBINPUT_CONFIG_DRAG_LOCK_ENABLED,
};

enum libinput_config_scroll_method {
	LIBINPUT_CONFIG_SCROLL_NO_SCROLL = 0,
	LIBINPUT_CONFIG_SCROLL_2FG = (1 << 0),
	LIBINPUT_CONFIG_SCROLL_EDGE = (1 << 1),
	LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN = (1 << 2),
};

enum libinput_config_accel_profile {
	LIBINPUT_CONFIG_ACCEL_PROFILE_NONE = 0,
	LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT = (1 << 0),
	LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE = (1 << 1),
	LIBINPUT_CONFIG_ACCEL_PROFILE_CUSTOM = (1 << 2),
};

enum libinput_config_click_method {
	LIBINPUT_CONFIG_CLICK_METHOD_NONE = 0,
	LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS = (1 << 0),
	LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER = (1 << 1),
};

enum libinput_config_send_events_mode {
	LIBINPUT_CONFIG_SEND_EVENTS_ENABLED = 0,
	LIBINPUT_CONFIG_SEND_EVENTS_DISABLED = (1 << 0),
	LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE = (1 << 1),
};

enum libinput_config_dwt_state {
	LIBINPUT_CONFIG_DWT_DISABLED,
	LIBINPUT_CONFIG_DWT_ENABLED,
};

enum libinput_config_middle_emulation_state {
	LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED,
	LIBINPUT_CONFIG_MIDDLE_EMULATION_ENABLED,
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

/* libinput_device_keyboard_has_key declaration moved to configuration section */

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

/* Tablet tool functions */
struct libinput_tablet_tool *
libinput_event_tablet_tool_get_tool(struct libinput_event_tablet_tool *event);

struct libinput_event_tablet_tool *
libinput_event_get_tablet_tool_event(struct libinput_event *event);

uint64_t
libinput_tablet_tool_get_serial(struct libinput_tablet_tool *tool);

uint64_t
libinput_tablet_tool_get_tool_id(struct libinput_tablet_tool *tool);

/* Configuration functions */
int libinput_device_config_tap_get_finger_count(struct libinput_device *device);
enum libinput_config_tap_state libinput_device_config_tap_get_enabled(struct libinput_device *device);
enum libinput_config_tap_state libinput_device_config_tap_get_default_enabled(struct libinput_device *device);
enum libinput_config_status libinput_device_config_tap_set_enabled(struct libinput_device *device, enum libinput_config_tap_state enable);

enum libinput_config_tap_button_map libinput_device_config_tap_get_button_map(struct libinput_device *device);
enum libinput_config_tap_button_map libinput_device_config_tap_get_default_button_map(struct libinput_device *device);
enum libinput_config_status libinput_device_config_tap_set_button_map(struct libinput_device *device, enum libinput_config_tap_button_map map);

enum libinput_config_drag_state libinput_device_config_tap_get_drag_enabled(struct libinput_device *device);
enum libinput_config_drag_state libinput_device_config_tap_get_default_drag_enabled(struct libinput_device *device);
enum libinput_config_status libinput_device_config_tap_set_drag_enabled(struct libinput_device *device, enum libinput_config_drag_state enable);

enum libinput_config_drag_lock_state libinput_device_config_tap_get_drag_lock_enabled(struct libinput_device *device);
enum libinput_config_drag_lock_state libinput_device_config_tap_get_default_drag_lock_enabled(struct libinput_device *device);
enum libinput_config_status libinput_device_config_tap_set_drag_lock_enabled(struct libinput_device *device, enum libinput_config_drag_lock_state enable);

uint32_t libinput_device_config_scroll_get_methods(struct libinput_device *device);
enum libinput_config_scroll_method libinput_device_config_scroll_get_method(struct libinput_device *device);
enum libinput_config_scroll_method libinput_device_config_scroll_get_default_method(struct libinput_device *device);
enum libinput_config_status libinput_device_config_scroll_set_method(struct libinput_device *device, enum libinput_config_scroll_method method);

uint32_t libinput_device_config_scroll_get_button(struct libinput_device *device);
uint32_t libinput_device_config_scroll_get_default_button(struct libinput_device *device);
enum libinput_config_status libinput_device_config_scroll_set_button(struct libinput_device *device, uint32_t button);

int libinput_device_config_scroll_has_natural_scroll(struct libinput_device *device);
int libinput_device_config_scroll_get_natural_scroll_enabled(struct libinput_device *device);
int libinput_device_config_scroll_get_default_natural_scroll_enabled(struct libinput_device *device);
enum libinput_config_status libinput_device_config_scroll_set_natural_scroll_enabled(struct libinput_device *device, int enable);

int libinput_device_config_left_handed_is_available(struct libinput_device *device);
int libinput_device_config_left_handed_get(struct libinput_device *device);
int libinput_device_config_left_handed_get_default(struct libinput_device *device);
enum libinput_config_status libinput_device_config_left_handed_set(struct libinput_device *device, int left_handed);

uint32_t libinput_device_config_click_get_methods(struct libinput_device *device);
enum libinput_config_click_method libinput_device_config_click_get_method(struct libinput_device *device);
enum libinput_config_click_method libinput_device_config_click_get_default_method(struct libinput_device *device);
enum libinput_config_status libinput_device_config_click_set_method(struct libinput_device *device, enum libinput_config_click_method method);

int libinput_device_config_middle_emulation_is_available(struct libinput_device *device);
enum libinput_config_middle_emulation_state libinput_device_config_middle_emulation_get_enabled(struct libinput_device *device);
enum libinput_config_middle_emulation_state libinput_device_config_middle_emulation_get_default_enabled(struct libinput_device *device);
enum libinput_config_status libinput_device_config_middle_emulation_set_enabled(struct libinput_device *device, enum libinput_config_middle_emulation_state enable);

int libinput_device_config_dwt_is_available(struct libinput_device *device);
enum libinput_config_dwt_state libinput_device_config_dwt_get_enabled(struct libinput_device *device);
enum libinput_config_dwt_state libinput_device_config_dwt_get_default_enabled(struct libinput_device *device);
enum libinput_config_status libinput_device_config_dwt_set_enabled(struct libinput_device *device, enum libinput_config_dwt_state enable);

int libinput_device_config_accel_is_available(struct libinput_device *device);
double libinput_device_config_accel_get_speed(struct libinput_device *device);
double libinput_device_config_accel_get_default_speed(struct libinput_device *device);
enum libinput_config_status libinput_device_config_accel_set_speed(struct libinput_device *device, double speed);

uint32_t libinput_device_config_accel_get_profiles(struct libinput_device *device);
enum libinput_config_accel_profile libinput_device_config_accel_get_profile(struct libinput_device *device);
enum libinput_config_accel_profile libinput_device_config_accel_get_default_profile(struct libinput_device *device);
enum libinput_config_status libinput_device_config_accel_set_profile(struct libinput_device *device, enum libinput_config_accel_profile profile);

uint32_t libinput_device_config_send_events_get_modes(struct libinput_device *device);
enum libinput_config_send_events_mode libinput_device_config_send_events_get_mode(struct libinput_device *device);
enum libinput_config_send_events_mode libinput_device_config_send_events_get_default_mode(struct libinput_device *device);
enum libinput_config_status libinput_device_config_send_events_set_mode(struct libinput_device *device, enum libinput_config_send_events_mode mode);

/* Keyboard functions */
int libinput_device_keyboard_has_key(struct libinput_device *device, uint32_t code);

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