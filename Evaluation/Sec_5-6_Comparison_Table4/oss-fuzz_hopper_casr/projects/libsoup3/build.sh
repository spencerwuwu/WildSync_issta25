#!/bin/bash -eu
# Copyright 2018 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

# Build glib
pushd $SRC/glib/
meson \
    setup \
    --libdir=lib \
    --default-library=static \
    -Db_lundef=false \
    -Doss_fuzz=enabled \
    -Dlibmount=disabled \
    _builddir
ninja -C _builddir
ninja -C _builddir install
popd


new_fuzzers="libsoup3__soup_message_body_append_bytes__0 libsoup3__soup_message_body_append_bytes__1 libsoup3__soup_message_get_method__0 libsoup3__soup_message_get_method__1 libsoup3__soup_message_get_method__2 libsoup3__soup_message_get_reason_phrase__3 libsoup3__soup_message_get_reason_phrase__7 libsoup3__soup_message_get_request_headers__1 libsoup3__soup_message_get_request_headers__10 libsoup3__soup_message_get_request_headers__3 libsoup3__soup_message_get_request_headers__4 libsoup3__soup_message_get_request_headers__5 libsoup3__soup_message_get_request_headers__6 libsoup3__soup_message_get_request_headers__7 libsoup3__soup_message_get_request_headers__8 libsoup3__soup_message_get_request_headers__9 libsoup3__soup_message_get_response_headers__0 libsoup3__soup_message_get_response_headers__1 libsoup3__soup_message_get_response_headers__2 libsoup3__soup_message_get_response_headers__3 libsoup3__soup_message_get_response_headers__4 libsoup3__soup_message_get_response_headers__5 libsoup3__soup_message_get_response_headers__6 libsoup3__soup_message_get_status__0 libsoup3__soup_message_get_status__10 libsoup3__soup_message_get_status__2 libsoup3__soup_message_get_status__3 libsoup3__soup_message_get_status__5 libsoup3__soup_message_get_status__6 libsoup3__soup_message_get_status__7 libsoup3__soup_message_get_status__8 libsoup3__soup_message_get_tls_peer_certificate__0 libsoup3__soup_message_get_tls_peer_certificate_errors__0 libsoup3__soup_message_get_uri__0 libsoup3__soup_message_get_uri__1 libsoup3__soup_message_get_uri__2 libsoup3__soup_message_get_uri__3 libsoup3__soup_message_get_uri__4 libsoup3__soup_message_get_uri__5 libsoup3__soup_message_get_uri__6 libsoup3__soup_message_get_uri__7 libsoup3__soup_message_headers_get_content_disposition__0 libsoup3__soup_message_headers_get_content_type__0 libsoup3__soup_message_new_from_uri__0 libsoup3__soup_message_new_from_uri__1 libsoup3__soup_message_new_from_uri__10 libsoup3__soup_message_new_from_uri__2 libsoup3__soup_message_new_from_uri__3 libsoup3__soup_message_new_from_uri__4 libsoup3__soup_message_new_from_uri__5 libsoup3__soup_message_new_from_uri__6 libsoup3__soup_message_new_from_uri__7 libsoup3__soup_message_new_from_uri__8 libsoup3__soup_message_new_from_uri__9 libsoup3__soup_multipart_append_part__0 libsoup3__soup_multipart_append_part__1 libsoup3__soup_session_send__0 libsoup3__soup_session_send_async__0 libsoup3__soup_session_send_async__1"

for new_f in $new_fuzzers; do
  cp $SRC/new_fuzzers/${new_f}.cpp fuzzing/${new_f}.c
done

for dict in "fuzzing/*.dict"; do
    cat $dict >> fuzzing/all.dict
done

meson setup \
    --default-library=static \
    -Dvapi=disabled        \
    -Dgssapi=disabled      \
    -Dsysprof=disabled     \
    --wrap-mode=nofallback \
    -Dtls_check=false \
    -Dntlm=disabled \
    -Dintrospection=disabled \
    -Dfuzzing=enabled build


orig_fuzzers="fuzz_content_sniffer fuzz_cookie_parse fuzz_date_time fuzz_decode_data_uri"

for orig_fuzz in $orig_fuzzers; do
    ninja -v -C build fuzzing/${orig_fuzz}
    cp build/fuzzing/${orig_fuzz} $OUT/
    cp fuzzing/all.dict $OUT/${orig_fuzz}.dict
done




for new_f in $new_fuzzers; do
  ninja -v -C build fuzzing/${new_f}
  mv build/fuzzing/${new_f} $OUT/
  cp fuzzing/all.dict $OUT/${new_f}.dict
done

