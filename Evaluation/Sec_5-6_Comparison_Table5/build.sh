#!/bin/bash -e

if [ -z $FUZZER ] || [ -z $TARGET ]; then
    echo '$FUZZER and $TARGET must be specified as environment variables.'
    exit 1
fi

pushd "$BASE/oss-fuzz_$FUZZER" &> /dev/null
set -x
python3 infra/helper.py build_fuzzers --engine=$FUZZER $TARGET
set +x

popd &> /dev/null
