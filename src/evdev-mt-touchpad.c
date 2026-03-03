/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#include "libinput.h"
#include "libinput-private.h"
#include <libudev.h>
#include <stdio.h>

/* Tablet tool management functions - these are Termux-specific */

struct libinput_tablet_tool *
libinput_tablet_tool_ref(struct libinput_tablet_tool *tool)
{
    return tool;
}

struct libinput_tablet_tool *
libinput_tablet_tool_unref(struct libinput_tablet_tool *tool)
{
    return tool;
}

enum libinput_tablet_tool_type
libinput_tablet_tool_get_type(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return LIBINPUT_TABLET_TOOL_TYPE_PEN;
}

int
libinput_tablet_tool_has_pressure(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return 0;
}

int
libinput_tablet_tool_has_distance(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return 0;
}

int
libinput_tablet_tool_has_rotation(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return 0;
}

int
libinput_tablet_tool_has_tilt(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return 0;
}

int
libinput_tablet_tool_has_slider(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return 0;
}

int
libinput_tablet_tool_has_wheel(struct libinput_tablet_tool *tool)
{
    (void)tool;
    return 0;
}

/* All other device functions are implemented in libinput-mock-all.c */