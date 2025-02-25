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
export LOGDIR="$WORKDIR/casr_logs"
export COVARDIR="$WORKDIR/casr_ar"

mkdir -p "$LOGDIR"
mkdir -p "$COVARDIR"

start_coverage()
{
    export campaign="casr_${FUZZER}_${TARGET}"

    export HOPPER_OUT="hopper/queue"

    export STORAGE="${COVARDIR}/${TARGET}"
    mkdir -p "$STORAGE" && chmod 777 "$STORAGE"

    log_name="${LOGDIR}/${FUZZER}_${TARGET}_container.log"
    echo_time "Container $campaign started on CPU $AFFINITY"
    "$BASE"/start_casr.sh &> "$log_name"
    echo_time "Container $campaign stopped"

    #covdirs=(dumps merged_coverage textcov_reports report report_target)
    #for covdir in "${covdirs[@]}"; do
    #    sudo rm -rf "$BASE/oss-fuzz_hopper/build/out/$TARGET/hopper/$ROUND/cov/$covdir"
    #done
}
export -f start_coverage

# First check if there's any mis-archive
#mischeck=$(find $WORKDIR/ar -type d -name 5)
#if [[ "$mischeck" != "" ]]; then
#    echo_time "There are mis-archived folders. Please check."
#    echo "$mischeck"
#    exit 1
#fi

for FUZZER in "${FUZZERS[@]}"; do
    export FUZZER

    TARGETS=($(get_var_or_default $FUZZER 'TARGETS'))
    for ITARGET in "${TARGETS[@]}"; do
        export TARGET=${ITARGET//_/-}
        export ITARGET=${TARGET//-/_}

        export FUZZER=$FUZZER
        export IMG_NAME="hopper-casr_${TARGET}"
        echo_time "Start building $IMG_NAME"
        if [ ! "$(ls -A ${WORKDIR}/trans_ar/hopper_${TARGET}/crashes_translated/)" ]; then
            echo_time "No to-be triaged crashes for ${TARGET}"
            mkdir -p ${WORKDIR}/casr_ar/${TARGET}/
            continue
        fi
        if ! "$BASE"/build_casr.sh &> \
            "${LOGDIR}/${IMG_NAME}_build.log"; then
                    echo_time "Failed to build $IMG_NAME. Check build log for info."
                    continue
        fi
        echo_time "Starting campaigns for casr_$FUZZER $TARGET"
        start_coverage
    done
done
