/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "libinput.h"
#include "libinput-private.h"

/* Event creation and management - Termux-specific implementations */

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
    event->time_usec = time_usec;
    event->next = NULL;
    
    return event;
}

/* All other event functions are implemented in libinput-mock-all.c */