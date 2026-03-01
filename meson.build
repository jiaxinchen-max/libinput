project('libinput', 'c',
	version : '1.28.0',
	license : 'MIT/Expat',
	default_options : [ 'c_std=gnu99', 'warning_level=2' ],
	meson_version : '>= 0.64.0')

libinput_version = meson.project_version().split('.')

# Compiler setup
cc = meson.get_compiler('c')
cflags = [
	'-Wno-unused-parameter',
	'-Wmissing-prototypes',
	'-Wstrict-prototypes',
	'-Wundef',
	'-Wpointer-arith',
	'-Wuninitialized',
	'-Winit-self',
	'-Wstrict-prototypes',
	'-Wimplicit-fallthrough',
	'-Wredundant-decls',
	'-Wincompatible-pointer-types',
	'-Wformat=2',
	'-Wno-missing-field-initializers',
	'-Wmissing-declarations',
	'-fvisibility=hidden',
]
add_project_arguments(cc.get_supported_arguments(cflags), language : 'c')

# config.h
config_h = configuration_data()
config_h.set('_GNU_SOURCE', '1')
config_h.set_quoted('LIBINPUT_VERSION', meson.project_version())

# Dependencies for Android/Termux
dep_udev = declare_dependency()  # We provide our own mock
dep_libevdev = declare_dependency()  # We provide our own mock
dep_lm = cc.find_library('m', required : false)
dep_pthread = dependency('threads')

# termux-display-client integration (required dependency)
dep_termux_client = dependency('termux-display-client', required : false)
if not dep_termux_client.found()
    # Try to find as a library
    dep_termux_client = cc.find_library('termux-display-client', required : true)
endif

message('Using termux-display-client library for input integration')

# Include directories
includes_include = include_directories('.')
includes_src = include_directories('src')

############ libinput.so ############
install_headers('src/libinput.h')

src_libinput = [
	'src/libinput.c',
	'src/evdev.c',
	'src/evdev-mt-touchpad.c',
	'src/path-seat.c',
	'src/udev-seat.c',
	'src/filter.c',
	'src/timer.c',
	'src/termux-input-bridge.c',
]

deps_libinput = [
	dep_udev,
	dep_libevdev,
	dep_lm,
	dep_pthread,
	dep_termux_client,
]

# We use libtool-version numbers for compatibility
libinput_lt_c=23
libinput_lt_r=0
libinput_lt_a=13

# convert to soname
libinput_so_version = '@0@.@1@.@2@'.format((libinput_lt_c - libinput_lt_a),
                                            libinput_lt_a, libinput_lt_r)

lib_libinput = shared_library('input',
		src_libinput,
		include_directories : [includes_include, includes_src],
		dependencies : deps_libinput,
		version : libinput_so_version,
		install : true
		)

dep_libinput = declare_dependency(
		link_with : lib_libinput,
		dependencies : deps_libinput)

# pkg-config
pkgconfig = import('pkgconfig')
pkgconfig.generate(
	filebase : 'libinput',
	name : 'Libinput',
	description : 'Input device library (Termux/Android compatible)',
	version : meson.project_version(),
	libraries : lib_libinput,
)

############ output files ############
configure_file(output : 'config.h', configuration : config_h)