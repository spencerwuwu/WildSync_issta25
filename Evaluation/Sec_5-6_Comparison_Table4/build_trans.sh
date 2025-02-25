#!/bin/bash -e

if [ -z $FUZZER ] || [ -z $TARGET ]; then
    echo '$FUZZER and $TARGET must be specified as environment variables.'
    exit 1
fi

pushd "$BASE/hopper_translate_crash" &> /dev/null
set -x
timeout 120m docker build -t trans-$FUZZER/$TARGET \
    --shm-size=2g --platform linux/amd64 \
    --build-arg TARGET=$TARGET \
    -f $TARGET/Dockerfile \
    .
set +x

popd &> /dev/null
