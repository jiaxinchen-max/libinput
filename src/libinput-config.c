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

/* Calibration functions */
int libinput_device_config_calibration_has_matrix(struct libinput_device *device)
{
    (void)device;
    return 1; /* Mock: assume calibration is available */
}

enum libinput_config_status libinput_device_config_calibration_set_matrix(struct libinput_device *device, const float matrix[6])
{
    (void)device;
    (void)matrix;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

int libinput_device_config_calibration_get_matrix(struct libinput_device *device, float matrix[6])
{
    (void)device;
    /* Return identity matrix */
    matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
    matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
    return 1;
}

int libinput_device_config_calibration_get_default_matrix(struct libinput_device *device, float matrix[6])
{
    (void)device;
    /* Return identity matrix */
    matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
    matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
    return 1;
}

/* Device group functions */
struct libinput_device_group *libinput_device_get_device_group(struct libinput_device *device)
{
    (void)device;
    /* Mock: return a dummy pointer - use a simple int as placeholder */
    static int dummy_group;
    return (struct libinput_device_group *)&dummy_group;
}

/* Switch functions */
int libinput_device_switch_has_switch(struct libinput_device *device, enum libinput_switch sw)
{
    (void)device;
    (void)sw;
    return 0; /* Mock: no switches available */
}

/* Pointer functions */
int libinput_device_pointer_has_button(struct libinput_device *device, uint32_t code)
{
    (void)device;
    (void)code;
    return 1; /* Mock: assume all buttons are available */
}

/* Area configuration functions */
struct libinput_config_area_rectangle libinput_device_config_area_get_rectangle(struct libinput_device *device)
{
    (void)device;
    struct libinput_config_area_rectangle rect = {0.0, 0.0, 1.0, 1.0};
    return rect;
}

enum libinput_config_status libinput_device_config_area_set_rectangle(struct libinput_device *device, const struct libinput_config_area_rectangle *rect)
{
    (void)device;
    (void)rect;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

/* Tablet pad functions */
int libinput_device_tablet_pad_get_num_buttons(struct libinput_device *device)
{
    (void)device;
    return 0; /* Mock: no tablet pad buttons */
}

int libinput_device_tablet_pad_get_num_dials(struct libinput_device *device)
{
    (void)device;
    return 0; /* Mock: no tablet pad dials */
}

int libinput_device_tablet_pad_get_num_rings(struct libinput_device *device)
{
    (void)device;
    return 0; /* Mock: no tablet pad rings */
}

int libinput_device_tablet_pad_get_num_strips(struct libinput_device *device)
{
    (void)device;
    return 0; /* Mock: no tablet pad strips */
}

int libinput_device_tablet_pad_get_num_mode_groups(struct libinput_device *device)
{
    (void)device;
    return 0; /* Mock: no tablet pad mode groups */
}

struct libinput_tablet_pad_mode_group *libinput_device_tablet_pad_get_mode_group(struct libinput_device *device, unsigned int index)
{
    (void)device;
    (void)index;
    return NULL; /* Mock: no mode groups available */
}

/* Tablet pad mode group functions */
unsigned int libinput_tablet_pad_mode_group_get_index(struct libinput_tablet_pad_mode_group *group)
{
    (void)group;
    return 0;
}

unsigned int libinput_tablet_pad_mode_group_get_num_modes(struct libinput_tablet_pad_mode_group *group)
{
    (void)group;
    return 1; /* Default to 1 mode */
}

unsigned int libinput_tablet_pad_mode_group_get_mode(struct libinput_tablet_pad_mode_group *group)
{
    (void)group;
    return 0; /* Default mode */
}

int libinput_tablet_pad_mode_group_has_button(struct libinput_tablet_pad_mode_group *group, unsigned int button)
{
    (void)group;
    (void)button;
    return 0;
}

int libinput_tablet_pad_mode_group_has_dial(struct libinput_tablet_pad_mode_group *group, unsigned int dial)
{
    (void)group;
    (void)dial;
    return 0;
}

int libinput_tablet_pad_mode_group_has_ring(struct libinput_tablet_pad_mode_group *group, unsigned int ring)
{
    (void)group;
    (void)ring;
    return 0;
}

int libinput_tablet_pad_mode_group_has_strip(struct libinput_tablet_pad_mode_group *group, unsigned int strip)
{
    (void)group;
    (void)strip;
    return 0;
}

/* Device LED functions */
void libinput_device_led_update(struct libinput_device *device, enum libinput_led leds)
{
    (void)device;
    (void)leds;
    /* Mock implementation - do nothing */
}

/* Area configuration functions */
int libinput_device_config_area_has_rectangle(struct libinput_device *device)
{
    (void)device;
    return 0; /* No area rectangle support in mock */
}

/* Rotation configuration functions */
int libinput_device_config_rotation_is_available(struct libinput_device *device)
{
    (void)device;
    return 0; /* No rotation support in mock */
}

unsigned int libinput_device_config_rotation_get_angle(struct libinput_device *device)
{
    (void)device;
    return 0; /* Default angle */
}

enum libinput_config_status libinput_device_config_rotation_set_angle(struct libinput_device *device, unsigned int degrees_cw)
{
    (void)device;
    (void)degrees_cw;
    return LIBINPUT_CONFIG_STATUS_SUCCESS;
}

unsigned int libinput_device_config_rotation_get_default_angle(struct libinput_device *device)
{
    (void)device;
    return 0; /* Default angle */
}