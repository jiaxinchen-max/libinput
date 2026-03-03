/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "evdev-mt-touchpad.h"
#include "libinput-private.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Device management functions */

struct libinput_device *
libinput_device_create(struct libinput *libinput,
                       const char *name,
                       const char *sysname,
                       unsigned int vendor_id,
                       unsigned int product_id,
                       unsigned int bus_type)
{
    struct libinput_device *device;
    
    device = calloc(1, sizeof(*device));
    if (!device)
        return NULL;
        
    device->refcount = 1;
    device->name = strdup(name ? name : "Unknown Device");
    device->sysname = strdup(sysname ? sysname : "unknown");
    device->output_name = strdup("");
    
    device->vendor_id = vendor_id;
    device->product_id = product_id;
    device->bus_type = bus_type;
    
    /* Create a default seat */
    device->seat = calloc(1, sizeof(*device->seat));
    if (device->seat) {
        device->seat->name = strdup("seat0");
        device->seat->libinput = libinput;
    }
    
    /* Create a udev_device for this libinput device using libudev */
    struct udev *udev = udev_new();
    if (udev) {
        struct udev_device *udev_device = udev_device_new_from_subsystem_sysname(udev, "input", sysname);
        device->udev_device = udev_device;
        udev_unref(udev);  /* We only need the device, not the context */
    }
    
    return device;
}

void
libinput_device_set_capabilities(struct libinput_device *device,
                                 int keyboard, int pointer, int touch,
                                 int tablet_tool, int tablet_pad,
                                 int gesture, int switch_cap)
{
    if (!device)
        return;
        
    device->has_keyboard = keyboard;
    device->has_pointer = pointer;
    device->has_touch = touch;
    device->has_tablet_tool = tablet_tool;
    device->has_tablet_pad = tablet_pad;
    device->has_gesture = gesture;
    device->has_switch = switch_cap;
}

struct libinput_device *
libinput_device_ref(struct libinput_device *device)
{
    if (!device)
        return NULL;
        
    device->refcount++;
    return device;
}

struct libinput_device *
libinput_device_unref(struct libinput_device *device)
{
    if (!device)
        return NULL;
        
    device->refcount--;
    if (device->refcount == 0) {
        if (device->seat) {
            free(device->seat->name);
            free(device->seat);
        }
        if (device->udev_device) {
            udev_device_unref((struct udev_device *)device->udev_device);
        }
        free(device->name);
        free(device->sysname);
        free(device->output_name);
        free(device);
        return NULL;
    }
    
    return device;
}

const char *
libinput_device_get_name(struct libinput_device *device)
{
    return device ? device->name : NULL;
}

const char *
libinput_device_get_sysname(struct libinput_device *device)
{
    return device ? device->sysname : NULL;
}

const char *
libinput_device_get_output_name(struct libinput_device *device)
{
    return device ? device->output_name : NULL;
}

struct libinput_seat *
libinput_device_get_seat(struct libinput_device *device)
{
    return device ? device->seat : NULL;
}

void
libinput_device_set_user_data(struct libinput_device *device, void *user_data)
{
    if (device)
        device->user_data = user_data;
}

void *
libinput_device_get_user_data(struct libinput_device *device)
{
    return device ? device->user_data : NULL;
}

struct udev_device *
libinput_device_get_udev_device(struct libinput_device *device)
{
    return device ? device->udev_device : NULL;
}

int
libinput_device_has_capability(struct libinput_device *device,
                               enum libinput_device_capability capability)
{
    if (!device)
        return 0;
        
    switch (capability) {
    case LIBINPUT_DEVICE_CAP_KEYBOARD:
        return device->has_keyboard;
    case LIBINPUT_DEVICE_CAP_POINTER:
        return device->has_pointer;
    case LIBINPUT_DEVICE_CAP_TOUCH:
        return device->has_touch;
    case LIBINPUT_DEVICE_CAP_TABLET_TOOL:
        return device->has_tablet_tool;
    case LIBINPUT_DEVICE_CAP_TABLET_PAD:
        return device->has_tablet_pad;
    case LIBINPUT_DEVICE_CAP_GESTURE:
        return device->has_gesture;
    case LIBINPUT_DEVICE_CAP_SWITCH:
        return device->has_switch;
    default:
        return 0;
    }
}

int
libinput_device_get_size(struct libinput_device *device,
                         double *width,
                         double *height)
{
    if (!device || !width || !height)
        return -1;
        
    *width = 100.0;
    *height = 60.0;
    
    return 0;
}

/* libinput_device_keyboard_has_key moved to libinput-config.c */

unsigned int
libinput_device_get_id_product(struct libinput_device *device)
{
    return device ? device->product_id : 0;
}

unsigned int
libinput_device_get_id_vendor(struct libinput_device *device)
{
    return device ? device->vendor_id : 0;
}

unsigned int
libinput_device_get_id_bustype(struct libinput_device *device)
{
    return device ? device->bus_type : 0;
}

/* udev functions are now provided by libudev library */