#!/bin/bash -e

if [ -z $FUZZER ] || [ -z $TARGET ]; then
    echo '$FUZZER and $TARGET must be specified as environment variables.'
    exit 1
fi

pushd "$BASE/oss-fuzz_coverage" &> /dev/null
set -x
python3 infra/helper.py build_fuzzers --sanitizer coverage $TARGET
set +x

popd &> /dev/null
