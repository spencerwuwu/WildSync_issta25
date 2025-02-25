#!/bin/bash -e

if [ -z $FUZZER ] || [ -z $TARGET ]; then
    echo '$FUZZER $TARGET must be specified as environment variables.'
    exit 1
fi

dest_dir=$BASE/oss-fuzz_hopper_casr/projects/$TARGET/hopper_results/
rm -rf $dest_dir && mkdir -p $dest_dir
cp $WORKDIR/trans_ar/hopper_$TARGET/crashes_translated/* $dest_dir/

pushd "$BASE/oss-fuzz_hopper_casr" &> /dev/null
set -x
python3 infra/helper.py build_fuzzers $TARGET
set +x

popd &> /dev/null
