/* SPDX-License-Identifier: MIT */

#include "libinput.h"
#include "libinput-private.h"

/* Tap configuration */
int libinput_device_config_tap_get_finger_count(struct libinput_device *device)
{
    (void)device;
    return 3; /* Mock: support 3-finger tap */
}

enum libinput_config_tap_state libinput_device_config_tap_get_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_TAP_ENABLED;
}

enum libinput_config_tap_state libinput_device_config_tap_get_default_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_TAP_ENABLED;
}

enum libinput_config_status libinput_device_config_tap_set_enabled(struct libinput_device *device, enum libinput_config_tap_state enable)
{
    (void)device;
    (void)enable;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

enum libinput_config_tap_button_map libinput_device_config_tap_get_button_map(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_TAP_MAP_LRM;
}

enum libinput_config_tap_button_map libinput_device_config_tap_get_default_button_map(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_TAP_MAP_LRM;
}

enum libinput_config_status libinput_device_config_tap_set_button_map(struct libinput_device *device, enum libinput_config_tap_button_map map)
{
    (void)device;
    (void)map;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

enum libinput_config_drag_state libinput_device_config_tap_get_drag_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_DRAG_ENABLED;
}

enum libinput_config_drag_state libinput_device_config_tap_get_default_drag_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_DRAG_ENABLED;
}

enum libinput_config_status libinput_device_config_tap_set_drag_enabled(struct libinput_device *device, enum libinput_config_drag_state enable)
{
    (void)device;
    (void)enable;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

enum libinput_config_drag_lock_state libinput_device_config_tap_get_drag_lock_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_DRAG_LOCK_DISABLED;
}

enum libinput_config_drag_lock_state libinput_device_config_tap_get_default_drag_lock_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_DRAG_LOCK_DISABLED;
}

enum libinput_config_status libinput_device_config_tap_set_drag_lock_enabled(struct libinput_device *device, enum libinput_config_drag_lock_state enable)
{
    (void)device;
    (void)enable;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Scroll configuration */
uint32_t libinput_device_config_scroll_get_methods(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_SCROLL_2FG | LIBINPUT_CONFIG_SCROLL_EDGE | LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN;
}

enum libinput_config_scroll_method libinput_device_config_scroll_get_method(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_SCROLL_2FG;
}

enum libinput_config_scroll_method libinput_device_config_scroll_get_default_method(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_SCROLL_2FG;
}

enum libinput_config_status libinput_device_config_scroll_set_method(struct libinput_device *device, enum libinput_config_scroll_method method)
{
    (void)device;
    (void)method;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

uint32_t libinput_device_config_scroll_get_button(struct libinput_device *device)
{
    (void)device;
    return 0; /* No button scroll by default */
}

uint32_t libinput_device_config_scroll_get_default_button(struct libinput_device *device)
{
    (void)device;
    return 0;
}

enum libinput_config_status libinput_device_config_scroll_set_button(struct libinput_device *device, uint32_t button)
{
    (void)device;
    (void)button;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

int libinput_device_config_scroll_has_natural_scroll(struct libinput_device *device)
{
    (void)device;
    return 1; /* Support natural scroll */
}

int libinput_device_config_scroll_get_natural_scroll_enabled(struct libinput_device *device)
{
    (void)device;
    return 0; /* Disabled by default */
}

int libinput_device_config_scroll_get_default_natural_scroll_enabled(struct libinput_device *device)
{
    (void)device;
    return 0;
}

enum libinput_config_status libinput_device_config_scroll_set_natural_scroll_enabled(struct libinput_device *device, int enable)
{
    (void)device;
    (void)enable;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Left-handed configuration */
int libinput_device_config_left_handed_is_available(struct libinput_device *device)
{
    (void)device;
    return 1; /* Support left-handed mode */
}

int libinput_device_config_left_handed_get(struct libinput_device *device)
{
    (void)device;
    return 0; /* Right-handed by default */
}

int libinput_device_config_left_handed_get_default(struct libinput_device *device)
{
    (void)device;
    return 0;
}

enum libinput_config_status libinput_device_config_left_handed_set(struct libinput_device *device, int left_handed)
{
    (void)device;
    (void)left_handed;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Click method configuration */
uint32_t libinput_device_config_click_get_methods(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS | LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER;
}

enum libinput_config_click_method libinput_device_config_click_get_method(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;
}

enum libinput_config_click_method libinput_device_config_click_get_default_method(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;
}

enum libinput_config_status libinput_device_config_click_set_method(struct libinput_device *device, enum libinput_config_click_method method)
{
    (void)device;
    (void)method;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Middle emulation configuration */
int libinput_device_config_middle_emulation_is_available(struct libinput_device *device)
{
    (void)device;
    return 1; /* Support middle emulation */
}

enum libinput_config_middle_emulation_state libinput_device_config_middle_emulation_get_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED;
}

enum libinput_config_middle_emulation_state libinput_device_config_middle_emulation_get_default_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED;
}

enum libinput_config_status libinput_device_config_middle_emulation_set_enabled(struct libinput_device *device, enum libinput_config_middle_emulation_state enable)
{
    (void)device;
    (void)enable;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Disable while typing configuration */
int libinput_device_config_dwt_is_available(struct libinput_device *device)
{
    (void)device;
    return 1; /* Support DWT */
}

enum libinput_config_dwt_state libinput_device_config_dwt_get_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_DWT_ENABLED;
}

enum libinput_config_dwt_state libinput_device_config_dwt_get_default_enabled(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_DWT_ENABLED;
}

enum libinput_config_status libinput_device_config_dwt_set_enabled(struct libinput_device *device, enum libinput_config_dwt_state enable)
{
    (void)device;
    (void)enable;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Acceleration configuration */
int libinput_device_config_accel_is_available(struct libinput_device *device)
{
    (void)device;
    return 1; /* Support acceleration */
}

double libinput_device_config_accel_get_speed(struct libinput_device *device)
{
    (void)device;
    return 0.0; /* Default speed */
}

double libinput_device_config_accel_get_default_speed(struct libinput_device *device)
{
    (void)device;
    return 0.0;
}

enum libinput_config_status libinput_device_config_accel_set_speed(struct libinput_device *device, double speed)
{
    (void)device;
    (void)speed;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

uint32_t libinput_device_config_accel_get_profiles(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT | LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
}

enum libinput_config_accel_profile libinput_device_config_accel_get_profile(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
}

enum libinput_config_accel_profile libinput_device_config_accel_get_default_profile(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
}

enum libinput_config_status libinput_device_config_accel_set_profile(struct libinput_device *device, enum libinput_config_accel_profile profile)
{
    (void)device;
    (void)profile;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Send events configuration */
uint32_t libinput_device_config_send_events_get_modes(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_SEND_EVENTS_ENABLED | LIBINPUT_CONFIG_SEND_EVENTS_DISABLED;
}

enum libinput_config_send_events_mode libinput_device_config_send_events_get_mode(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;
}

enum libinput_config_send_events_mode libinput_device_config_send_events_get_default_mode(struct libinput_device *device)
{
    (void)device;
    return LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;
}

enum libinput_config_status libinput_device_config_send_events_set_mode(struct libinput_device *device, enum libinput_config_send_events_mode mode)
{
    (void)device;
    (void)mode;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Keyboard functions */
int libinput_device_keyboard_has_key(struct libinput_device *device, uint32_t code)
{
    (void)device;
    (void)code;
    return 1; /* Mock: assume all keys are available */
}