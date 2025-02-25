#!/bin/bash -eu

#cat $SRC/add.configure.ac >> src/configure.ac

new_fuzzers="krb5__gss_acquire_cred__0 krb5__gss_import_name__4 krb5__gss_import_name__5 krb5__gss_import_name__6 krb5__gss_import_name__8 krb5__gss_inquire_mech_for_saslname__0 krb5__gss_release_cred__0 krb5__gss_release_cred__1 krb5__gss_release_cred__2 krb5__gss_release_cred__3 krb5__gss_release_cred__4 krb5__gss_release_cred__5 krb5__gss_release_cred__6 krb5__gss_release_cred__7 krb5__gss_release_cred__8 krb5__gss_release_cred__9 krb5__gss_release_name__0 krb5__gss_release_name__1 krb5__gss_release_name__2 krb5__gss_release_name__3 krb5__gss_release_name__4 krb5__gss_release_name__5 krb5__gss_release_name__6 krb5__gss_release_name__7 krb5__gss_release_name__8 krb5__gss_release_name__9"

for new_f in $new_fuzzers; do
    cp $SRC/new_fuzzers/${new_f}.cpp src/tests/new_fuzzing/${new_f}.c
done


# This script plays the role of build.sh in OSS-Fuzz.  If only minor
# changes are required such as changing the fuzzing targets, a PR in
# the OSS-Fuzz repository is not needed and they can be done here.




# Compile krb5 for oss-fuzz.
pushd src/
autoreconf
./configure CFLAGS="-fcommon $CFLAGS" CXXFLAGS="-fcommon $CXXFLAGS" \
    --enable-static --disable-shared --enable-ossfuzz
make
popd

# Copy fuzz targets and seed corpus to $OUT.
pushd src/tests/fuzzing

fuzzers=("fuzz_chpw" "fuzz_gss" "fuzz_json" "fuzz_krad" "fuzz_krb5_ticket"
        "fuzz_marshal_cred" "fuzz_marshal_princ" "fuzz_ndr" "fuzz_pac"
        "fuzz_profile" "fuzz_util")

for fuzzer in "${fuzzers[@]}"; do
    cp "$fuzzer" "$OUT/$fuzzer"
    zip -r "${OUT}/${fuzzer}_seed_corpus.zip" "${fuzzer}_seed_corpus"
done

popd

pushd src/tests/new_fuzzing

for new_f in $new_fuzzers; do
    make $new_f
    cp "${new_f}" ${OUT}/"${new_f}"
    cp "${OUT}/fuzz_util_seed_corpus.zip" "${OUT}/${new_f}_seed_corpus"
done

popd
