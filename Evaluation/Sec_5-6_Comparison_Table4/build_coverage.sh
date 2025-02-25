#!/bin/bash -e

if [ -z $FUZZER ] || [ -z $TARGET ] || [ -z $ROUND ]; then
    echo '$FUZZER $TARGET, $ROUND must be specified as environment variables.'
    exit 1
fi

dest_dir=$BASE/oss-fuzz_hopper/projects/$TARGET/hopper_results/
rm -rf $dest_dir && mkdir -p $dest_dir
cp -r $WORKDIR/ar/hopper/$TARGET/$ROUND/ $dest_dir/

pushd "$BASE/oss-fuzz_hopper" &> /dev/null
set -x
python3 infra/helper.py build_fuzzers --sanitizer coverage $TARGET
set +x

popd &> /dev/null
