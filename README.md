# Termux LibInput

A libinput-compatible library designed for Android/Termux environments. This library provides the same API as the official libinput library and integrates with termux-display-client to provide real Android input events to applications like KWin.

## Features

- **API Compatible**: Provides the same public API as libinput 1.28.0
- **Android/Termux Ready**: Works without Linux kernel dependencies
- **Real Input Integration**: Integrates with termux-display-client for actual input events
- **Event Handling**: Supports keyboard, pointer, touch, and gesture events
- **Device Management**: Virtual device creation and capability management
- **Configuration Stubs**: Provides all configuration APIs (returns sensible defaults)
- **Meson Build System**: Uses Meson like the official libinput

## Project Structure

```
libinput/
├── src/
│   ├── libinput.c              # Core context and event handling
│   ├── libinput.h              # Public API header
│   ├── evdev.c                 # Event processing and creation
│   ├── evdev-mt-touchpad.c     # Device management and mock udev
│   └── [stub files]            # Placeholder implementations
├── meson.build                 # Root Meson configuration (official style)
├── libinput.pc.in              # pkg-config template
└── README.md                   # This file
```

## Building

### Meson (Official libinput style)

**Note**: termux-display-client library must be installed first.

```bash
# Build libinput
meson setup builddir
meson compile -C builddir
meson install -C builddir
```

If termux-display-client is not found, the build will fail with a clear error message.

### Prerequisites

- Meson 0.64+
- C compiler with C99 support
- pthread library
- math library (libm)
- **termux-display-client library** (required - provides Android input integration)

## Installation

The library will be installed as:
- Library: `/usr/local/lib/libinput.so` (or `libinput.so.10`)
- Headers: `/usr/local/include/libinput/libinput.h`
- pkg-config: `/usr/local/lib/pkgconfig/libinput.pc`

## Usage

This library can be used as a drop-in replacement for libinput in applications like KWin:

```bash
# Check if properly installed
pkg-config --cflags --libs libinput

# Use in your meson.build
dependency('libinput')
```

### Usage Example

```c
#include <libinput.h>
#include <render.h>  // from termux-display-client
#include <sys/epoll.h>

// Get termux-display-client event fd
int termux_fd = get_conn_fd();

// Initialize libinput with termux fd
struct libinput *li = libinput_termux_create_context(&interface, userdata, termux_fd);

// Get the same fd for epoll (libinput_get_fd returns the termux_fd)
int libinput_fd = libinput_get_fd(li);
assert(libinput_fd == termux_fd);

// Add to epoll
struct epoll_event ev = {
    .events = EPOLLIN,
    .data.fd = libinput_fd
};
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, libinput_fd, &ev);

// In event loop
if (events[i].data.fd == libinput_fd) {
    libinput_dispatch(li);  // Reads from termux-display-client and converts events
    while ((event = libinput_get_event(li)) != NULL) {
        // Process converted libinput event
        libinput_event_destroy(event);
    }
}
```

## API Coverage

### Implemented Functions

- **Context Management**: `libinput_udev_create_context()`, `libinput_path_create_context()`
- **Event Handling**: `libinput_dispatch()`, `libinput_get_event()`, `libinput_next_event_type()`
- **Event Types**: All event creation and getter functions for keyboard, pointer, touch, gesture events
- **Device Management**: Device creation, capability queries, reference counting
- **Configuration**: All configuration APIs (return sensible defaults)

### Real Input Integration

- **termux-display-client Bridge**: Connects to termux-display-client for real input events
- **Android Keycode Mapping**: Uses termux-display-client's Android-to-Linux keycode mapping
- **Synchronous Event Processing**: Reads events on-demand when `libinput_dispatch()` is called
- **Event Translation**: Converts Android touch/mouse/keyboard events to libinput format  
- **File Descriptor Integration**: Uses termux-display-client's event fd for epoll/select integration
- **External Event Source**: KWin manages the termux-display-client connection and fd
- **Device Discovery**: Creates virtual devices with standard capabilities

## Compatibility

This implementation mimics the official libinput structure:
- Uses the same soversion (10) and library name (`libinput.so`)
- Provides identical API surface
- Compatible with existing build systems expecting libinput
- Uses Meson build system like the official libinput

## Integration with KWin

This library allows KWin to compile and run on Android/Termux by providing the required libinput dependency. It integrates directly with termux-display-client to provide real Android input events to KWin.

### Build Order

1. First build and install `termux-display-client`
2. Then build and install this `libinput` implementation
3. Finally build KWin with this libinput implementation

## License

MIT License - Compatible with the original libinput license.