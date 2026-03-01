/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#ifndef EVDEV_H
#define EVDEV_H

#include "libinput.h"
#include "libinput-private.h"

/* Device management and event processing for Android/Termux */

struct libinput_device *libinput_device_create(struct libinput *libinput,
                                               const char *name,
                                               const char *sysname,
                                               unsigned int vendor_id,
                                               unsigned int product_id,
                                               unsigned int bus_type);

void libinput_device_set_capabilities(struct libinput_device *device,
                                      int keyboard, int pointer, int touch,
                                      int tablet_tool, int tablet_pad,
                                      int gesture, int switch_cap);

/* Event creation functions */
struct libinput_event *
create_keyboard_event(struct libinput *libinput,
                     struct libinput_device *device,
                     enum libinput_event_type type,
                     uint32_t key,
                     enum libinput_key_state state);

struct libinput_event *
create_pointer_event(struct libinput *libinput,
                    struct libinput_device *device,
                    enum libinput_event_type type,
                    double dx, double dy);

struct libinput_event *
create_pointer_button_event(struct libinput *libinput,
                           struct libinput_device *device,
                           uint32_t button,
                           enum libinput_button_state state);

struct libinput_event *
create_touch_event(struct libinput *libinput,
                  struct libinput_device *device,
                  enum libinput_event_type type,
                  int32_t slot,
                  double x, double y);

/* Event queue management */
void queue_event(struct libinput *libinput, struct libinput_event *event);

#endif /* EVDEV_H */