fs = import('fs')

fuzz_targets = [
  'fuzz_decode_data_uri',
  'fuzz_cookie_parse',
  'fuzz_content_sniffer',
  'fuzz_date_time',

  'libsoup3__soup_message_body_append_bytes__0',

  'libsoup3__soup_message_body_append_bytes__1',

  'libsoup3__soup_message_get_method__0',

  'libsoup3__soup_message_get_method__1',

  'libsoup3__soup_message_get_method__2',

  'libsoup3__soup_message_get_reason_phrase__3',

  'libsoup3__soup_message_get_reason_phrase__7',

  'libsoup3__soup_message_get_request_headers__1',

  'libsoup3__soup_message_get_request_headers__10',

  'libsoup3__soup_message_get_request_headers__3',

  'libsoup3__soup_message_get_request_headers__4',

  'libsoup3__soup_message_get_request_headers__5',

  'libsoup3__soup_message_get_request_headers__6',

  'libsoup3__soup_message_get_request_headers__7',

  'libsoup3__soup_message_get_request_headers__8',

  'libsoup3__soup_message_get_request_headers__9',

  'libsoup3__soup_message_get_response_headers__0',

  'libsoup3__soup_message_get_response_headers__1',

  'libsoup3__soup_message_get_response_headers__2',

  'libsoup3__soup_message_get_response_headers__3',

  'libsoup3__soup_message_get_response_headers__4',

  'libsoup3__soup_message_get_response_headers__5',

  'libsoup3__soup_message_get_response_headers__6',

  'libsoup3__soup_message_get_status__0',

  'libsoup3__soup_message_get_status__10',

  'libsoup3__soup_message_get_status__2',

  'libsoup3__soup_message_get_status__3',

  'libsoup3__soup_message_get_status__5',

  'libsoup3__soup_message_get_status__6',

  'libsoup3__soup_message_get_status__7',

  'libsoup3__soup_message_get_status__8',

  'libsoup3__soup_message_get_tls_peer_certificate__0',

  'libsoup3__soup_message_get_tls_peer_certificate_errors__0',

  'libsoup3__soup_message_get_uri__0',

  'libsoup3__soup_message_get_uri__1',

  'libsoup3__soup_message_get_uri__2',

  'libsoup3__soup_message_get_uri__3',

  'libsoup3__soup_message_get_uri__4',

  'libsoup3__soup_message_get_uri__5',

  'libsoup3__soup_message_get_uri__6',

  'libsoup3__soup_message_get_uri__7',

  'libsoup3__soup_message_headers_get_content_disposition__0',

  'libsoup3__soup_message_headers_get_content_type__0',

  'libsoup3__soup_message_new_from_uri__0',

  'libsoup3__soup_message_new_from_uri__1',

  'libsoup3__soup_message_new_from_uri__10',

  'libsoup3__soup_message_new_from_uri__2',

  'libsoup3__soup_message_new_from_uri__3',

  'libsoup3__soup_message_new_from_uri__4',

  'libsoup3__soup_message_new_from_uri__5',

  'libsoup3__soup_message_new_from_uri__6',

  'libsoup3__soup_message_new_from_uri__7',

  'libsoup3__soup_message_new_from_uri__8',

  'libsoup3__soup_message_new_from_uri__9',

  'libsoup3__soup_multipart_append_part__0',

  'libsoup3__soup_multipart_append_part__1',

  'libsoup3__soup_session_send__0',

  'libsoup3__soup_session_send_async__0',

  'libsoup3__soup_session_send_async__1',

]

fuzzing_args = '-fsanitize=fuzzer,address,undefined'
have_fuzzing = cc.has_argument(fuzzing_args)
fuzzing_feature = get_option('fuzzing')

if not have_fuzzing and fuzzing_feature.enabled()
  error('clang and libfuzzer are required for fuzzing')
endif

if have_fuzzing and (fuzzing_feature.enabled() or fuzzing_feature.auto())
  foreach target : fuzz_targets
    exe = executable(target, [target + '.c'],
      c_args : [fuzzing_args, '-DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION'],
      link_args : fuzzing_args,
      dependencies : libsoup_dep,
    )

    extra_args = []
    dict_file = join_paths(meson.current_source_dir(), target + '.dict')
    if fs.exists(dict_file)
      extra_args += '-dict=' + dict_file
    endif

    test(target, exe,
      args: [
        '-runs=200000',
        '-artifact_prefix=meson-logs/' + target + '-',
        '-print_final_stats=1',
      ] + extra_args,
      env: [
        'ASAN_OPTIONS=fast_unwind_on_malloc=0',
        'UBSAN_OPTIONS=print_stacktrace=1',
      ],
      suite: 'fuzzing',
      timeout: 360,
      priority: -1,
    )
  endforeach
endif