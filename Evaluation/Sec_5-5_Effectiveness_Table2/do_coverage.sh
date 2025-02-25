#!/bin/bash -e

##
# Pre-requirements:
# + $1: path to captainrc (default: ./captainrc)
##

if [ -z $1 ]; then
    set -- "./coveragerc"
fi

if [ -z $ONLY_CATEGORY ]; then
    export ONLY_CATEGORY=0
fi

# load the configuration file (captainrc)
set -a
source "$1"
set +a

if [ -z $FUZZERS ]; then
    export FUZZERS=(libfuzzer)
fi

BASE="$(cd "$(dirname "${BASH_SOURCE[0]}")/" >/dev/null 2>&1 && pwd)"

export BASE
source "$BASE/common.sh"


WORKDIR="$(realpath "$WORKDIR")"
export ARDIR="$WORKDIR/ar"
export CORPUSDIR="$WORKDIR/corpus"
export LOGDIR="$WORKDIR/coverage_logs"
export COVARDIR="$WORKDIR/coverage_ar"

mkdir -p "$LOGDIR"
mkdir -p "$COVARDIR"
mkdir -p "$CORPUSDIR"

start_coverage()
{
    # set $SHARED as corpus_storage
    if [ $ONLY_CATEGORY -eq 0 ]; then
        MYGROUPS=(orig new)
    elif [ $ONLY_CATEGORY -eq 1 ]; then
        MYGROUPS=(new)
    elif [ $ONLY_CATEGORY -eq 2 ]; then
        MYGROUPS=(orig)
    fi
    for GROUP in "${MYGROUPS[@]}"; do
        export GROUP=$GROUP

        export campaign="coverage_${FUZZER}_${TARGET}_${GROUP}_${ROUND}"

        export SHARED="${CORPUSDIR}/${FUZZER}_${TARGET}_${GROUP}_${ROUND}"
        rm -rf "$SHARED"
        mkdir -p "$SHARED" && chmod 777 "$SHARED"

        export STORAGE="${COVARDIR}/${FUZZER}_${TARGET}_${GROUP}_${ROUND}"
        rm -rf "$STORAGE"
        mkdir -p "$STORAGE" && chmod 777 "$STORAGE"

        log_name="${LOGDIR}/${FUZZER}_${TARGET}_${GROUP}_${ROUND}_container.log"
        echo_time "Container $campaign started on CPU $AFFINITY"
        "$BASE"/start_coverage.sh &> "$log_name"
        echo_time "Container $campaign stopped"
    done

    covdirs=(dumps merged_coverage textcov_reports report report_target)
    for covdir in "${covdirs[@]}"; do
        sudo rm -rf "$BASE/oss-fuzz_coverage/build/out/$TARGET/$covdir"
    done
}
export -f start_coverage

# First check if there's any mis-archive
mischeck=$(find $WORKDIR/ar -type d -name 5)
if [[ "$mischeck" != "" ]]; then
    echo_time "There are mis-archived folders. Please check."
    echo "$mischeck"
    exit 1
fi

for FUZZER in "${FUZZERS[@]}"; do
    export FUZZER

    TARGETS=($(get_var_or_default $FUZZER 'TARGETS'))
    for ITARGET in "${TARGETS[@]}"; do
        export TARGET=${ITARGET//_/-}
        export ITARGET=${TARGET//-/_}

        export FUZZER=$FUZZER
        export IMG_NAME="coverage-${FUZZER}-${TARGET}"
        echo_time "Building $IMG_NAME"
        # Don't need this as involving introspector will automatically build
        if ! "$BASE"/build_coverage.sh &> \
            "${LOGDIR}/${IMG_NAME}_build.log"; then
                    echo_time "Failed to build $IMG_NAME. Check build log for info."
                    continue
        fi
        echo_time "Starting campaigns for $FUZZER $TARGET"
        export AFFINITY=22
        for ROUND in {0..4}; do
            export ROUND=$ROUND
            start_coverage
        done
    done
done
