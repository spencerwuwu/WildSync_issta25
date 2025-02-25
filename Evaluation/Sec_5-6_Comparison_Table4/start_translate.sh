#!/bin/bash -e
##
# Pre-requirements:
# - env FUZZER: fuzzer name (from fuzzers/)
# - env TARGET: target name (from targets/)
# + env SHARED: path to host-local volume where fuzzer load saved ones
# + env STORED: path to host-local volume where debugger save results
##

IMG_NAME="trans-$FUZZER/$TARGET"

killncleanup() {
    docker stop $container_id &> /dev/null
    docker rm -f $container_id &> /dev/null
    exit 0
}

cleanup() {
    docker rm -f $container_id &> /dev/null
    exit 0
}

trap cleanup EXIT 
trap killncleanup SIGINT SIGTERM SIGKILL

export container_id="${FUZZER}_${TARGET}"

if [ -z $FUZZER ] || [ -z $TARGET ]; then
    echo '$FUZZER, $TARGET must be specified as' \
         'environment variables.'
    exit 1
fi


if [ ! -z "$SHARED" ]; then
    SHARED="$(realpath "$SHARED")"
    flag_volume="--volume=$SHARED:/hopper_shared --volume=$STORAGE:/hopper_out"
fi

container_id=$(
docker run -dt $flag_volume \
    --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
    --env=TIMEOUT="" \
    "$IMG_NAME"
)

container_id=$(cut -c-12 <<< $container_id)
echo_time "Container for $FUZZER/$TARGET started in $container_id"
docker logs -f "$container_id" &
exit_code=$(docker wait $container_id)
exit $exit_code
