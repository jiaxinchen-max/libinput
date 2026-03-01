/*
 * SPDX-FileCopyrightText: 2024 Termux LibInput Implementation
 * SPDX-License-Identifier: MIT
 */

#ifndef TERMUX_INPUT_BRIDGE_H
#define TERMUX_INPUT_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

struct libinput;

/**
 * Initialize the termux input bridge
 * 
 * @param libinput The libinput context
 * @return 0 on success, -1 on error
 */
int termux_input_bridge_init(struct libinput *libinput);

/**
 * Set the eventfd to signal when new events are available
 * 
 * @param event_fd The eventfd to signal
 */
void termux_input_bridge_set_eventfd(int event_fd);

/**
 * Destroy the termux input bridge
 */
void termux_input_bridge_destroy(void);

/**
 * Check if the bridge is connected to termux-display-client
 * 
 * @return 1 if connected, 0 if not connected
 */
int termux_input_bridge_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* TERMUX_INPUT_BRIDGE_H */