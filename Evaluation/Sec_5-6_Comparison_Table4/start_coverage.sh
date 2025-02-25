#!/bin/bash -e
##
# Pre-requirements:
# - env FUZZER: fuzzer name (from fuzzers/)
# - env TARGET: target name (from targets/)
# + env HOPPER_OUT: path to hopper translated files
# - env STORAGE: path to store coverage files
# + env AFFINITY: the CPU to bind the container to (default: no affinity)
##

cleanup() {
    docker rm -f $container_id &> /dev/null
    exit 0
}

if [ -z $HOPPER_OUT ] || [ -z $TARGET ] || [ -z $ROUND ]; then
    echo '$HOPPER_OUT, $TARGET, and ROUND must be specified as' \
         'environment variables.'
    exit 1
fi

export container_id="coverage_${FUZZER}_${TARGET}"

echo_time "Starting ${container_id}"


set -x
cd "$BASE/oss-fuzz_hopper"
python3 infra/helper.py introspector \
    --hopper_out $HOPPER_OUT \
    --container-name $container_id \
    --cpu $AFFINITY \
    --new-bin \
    $TARGET &
pid=$!
trap cleanup EXIT SIGINT SIGTERM
wait $pid

#covdirs=(dumps merged_coverage textcov_reports report report_target)
# Not taking report_target/ as it's too big... report/ should be enough
covdirs=(dumps merged_coverage textcov_reports report)
for covdir in "${covdirs[@]}"; do
    cp -r "$BASE/oss-fuzz_hopper/build/out/$TARGET/$covdir" "$STORAGE"
done
