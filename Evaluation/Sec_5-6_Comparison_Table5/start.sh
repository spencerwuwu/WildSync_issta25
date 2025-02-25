#!/bin/bash -e
##
# Pre-requirements:
# - env FUZZER: fuzzer name (from fuzzers/)
# - env TARGET: target name (from targets/)
# - env PROGRAM: program name (name of binary artifact from $TARGET/build.sh)
# - env TIMEOUT: time to run the campaign
# + env SHARED: path to host-local volume where fuzzer findings are saved
# + env AFFINITY: the CPU to bind the container to (default: no affinity)
##

cleanup() {
    if [ ! -t 1 ]; then
        docker rm -f $container_id &> /dev/null
    fi
    exit 0
}

export container_id="${FUZZER}_${PROGRAM}_${ARCID}"

if [ -z $FUZZER ] || [ -z $TARGET ] || [ -z $PROGRAM ]; then
    echo '$FUZZER, $TARGET, and $PROGRAM must be specified as' \
         'environment variables.'
    exit 1
fi

echo_time "Starting $container_id"

cd "$BASE/oss-fuzz_$FUZZER"
python3 infra/helper.py run_fuzzer \
    --engine $FUZZER \
    --fuzztime $TIMEOUT \
    --container-name $container_id \
    --storage-dir $SHARED \
    --cpu $AFFINITY \
    $TARGET $PROGRAM &
pid=$!
trap cleanup EXIT SIGINT SIGTERM
wait $pid
