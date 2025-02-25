
if get_option('fuzzer_ldflags') != ''

  fribidi__fribidi_get_joining_types__0 = executable('fribidi__fribidi_get_joining_types__0',
    'fribidi__fribidi_get_joining_types__0.c', fribidi_unicode_version_h,
    c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
    include_directories: incs,
    link_with: libfribidi,
    link_args: get_option('fuzzer_ldflags').split(),
    install: false)

  fribidi__fribidi_get_joining_types__1 = executable('fribidi__fribidi_get_joining_types__1',
    'fribidi__fribidi_get_joining_types__1.c', fribidi_unicode_version_h,
    c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
    include_directories: incs,
    link_with: libfribidi,
    link_args: get_option('fuzzer_ldflags').split(),
    install: false)

  fribidi__fribidi_join_arabic__0 = executable('fribidi__fribidi_join_arabic__0',
    'fribidi__fribidi_join_arabic__0.c', fribidi_unicode_version_h,
    c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
    include_directories: incs,
    link_with: libfribidi,
    link_args: get_option('fuzzer_ldflags').split(),
    install: false)

  fribidi__fribidi_shape__0 = executable('fribidi__fribidi_shape__0',
    'fribidi__fribidi_shape__0.c', fribidi_unicode_version_h,
    c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
    include_directories: incs,
    link_with: libfribidi,
    link_args: get_option('fuzzer_ldflags').split(),
    install: false)

endif