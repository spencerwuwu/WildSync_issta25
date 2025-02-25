#!/bin/bash -e
##
# Pre-requirements:
# - env FUZZER: fuzzer name (from fuzzers/)
# - env TARGET: target name (from targets/)
# - env GROUP: new / orig
# + env SHARED: path to store collected corpus 
# - env STORAGE: path to store coverage files
# + env AFFINITY: the CPU to bind the container to (default: no affinity)
##

cleanup() {
    if [ ! -t 1 ]; then
        docker rm -f $container_id &> /dev/null
    fi
    exit 0
}

if [ -z $FUZZER ] || [ -z $TARGET ] || [ -z $GROUP ] || [ -z $ROUND ]; then
    echo '$FUZZER, $TARGET, and $GROUP must be specified as' \
         'environment variables.'
    exit 1
fi

export container_id="coverage_${FUZZER}_${GROUP}"

echo_time "Starting ${container_id}"

# Copy gernerated corpus to $SHARED
PROGRAMS=$(get_programs_from_file "$BASE/targets/$TARGET/${GROUP}_fuzzers.txt")

if [[ $FUZZER == "libfuzzer" ]]; then
    for PROGRAM in $PROGRAMS; do
        mkdir -p "$SHARED/$PROGRAM"
        cp_path="$WORKDIR/ar/$FUZZER/$TARGET/$PROGRAM/$ROUND"
        if [ -d $cp_path ]; then
            cp -r $cp_path "$SHARED/$PROGRAM"
        fi
    done
elif [[ $FUZZER == "afl" ]]; then
    for PROGRAM in $PROGRAMS; do
        mkdir -p "$SHARED/$PROGRAM"
        for corpus_type in queue hangs crashes; do
            cp_path="$WORKDIR/ar/$FUZZER/$TARGET/$PROGRAM/$ROUND/default/$corpus_type"
            if [ -d $cp_path ]; then
                cp -r $cp_path "$SHARED/$PROGRAM"
            fi
        done
    done
fi
# Legacy code with one big accmulated corpus
#if [[ $FUZZER == "libfuzzer" ]]; then
#    for PROGRAM in $PROGRAMS; do
#        mkdir -p "$SHARED/$PROGRAM"
#        pushd "$WORKDIR/ar/$FUZZER/$TARGET/$PROGRAM" &> /dev/null
#        paths=(*)
#        popd &> /dev/null
#        for path in "${paths[@]}"; do
#            mkdir $SHARED/$PROGRAM/$path
#            cp_path="$WORKDIR/ar/$FUZZER/$TARGET/$PROGRAM/$path"
#            if [ -d $cp_path ]; then
#                cp -r $cp_path "$SHARED/$PROGRAM/$path"
#            fi
#        done
#    done
#elif [[ $FUZZER == "afl" ]]; then
#    for PROGRAM in $PROGRAMS; do
#        mkdir -p "$SHARED/$PROGRAM"
#        pushd "$WORKDIR/ar/$FUZZER/$TARGET/$PROGRAM" &> /dev/null
#        paths=(*)
#        popd &> /dev/null
#        for path in "${paths[@]}"; do
#            mkdir $SHARED/$PROGRAM/$path
#            for corpus_type in queue hangs crashes; do
#                cp_path="$WORKDIR/ar/$FUZZER/$TARGET/$PROGRAM/$path/default/$corpus_type"
#                if [ -d $cp_path ]; then
#                    cp -r $cp_path "$SHARED/$PROGRAM/$path"
#                fi
#            done
#        done
#    done
#fi

set -x
cd "$BASE/oss-fuzz_coverage"
python3 infra/helper.py introspector \
    --my-corpus $SHARED \
    --container-name $container_id \
    --cpu $AFFINITY \
    --new-bin \
    $TARGET &
pid=$!
trap cleanup EXIT SIGINT SIGTERM
wait $pid

covdirs=(dumps merged_coverage textcov_reports report report_target)
for covdir in "${covdirs[@]}"; do
    cp -r "$BASE/oss-fuzz_coverage/build/out/$TARGET/$covdir" "$STORAGE"
done
