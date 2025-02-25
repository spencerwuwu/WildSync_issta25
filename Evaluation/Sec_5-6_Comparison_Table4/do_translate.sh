#!/bin/bash -e

##
# Pre-requirements:
# + $1: path to captainrc (default: ./captainrc)
##

if [ -z $1 ]; then
    set -- "./captainrc"
fi

# load the configuration file (captainrc)
set -a
source "$1"
set +a

if [ -z $FUZZERS ]; then
    export FUZZERS=(hopper)
fi

BASE="$(cd "$(dirname "${BASH_SOURCE[0]}")/" >/dev/null 2>&1 && pwd)"

export BASE
source "$BASE/common.sh"


WORKDIR="$(realpath "$WORKDIR")"
export ARDIR="$WORKDIR/ar"
export LOGDIR="$WORKDIR/trans_logs"
export DEBARDIR="$WORKDIR/trans_ar"

mkdir -p "$LOGDIR"
mkdir -p "$DEBARDIR"

start_coverage()
{
    export campaign="trans_${FUZZER}_${TARGET}"

    export SHARED="${ARDIR}/${FUZZER}/${TARGET}"
    export STORAGE="${DEBARDIR}/${FUZZER}_${TARGET}"
    rm -rf "$STORAGE"
    mkdir -p "$STORAGE" && chmod 777 "$STORAGE"

    log_name="${LOGDIR}/${FUZZER}_${TARGET}_container.log"
    echo_time "Container $campaign started"
    "$BASE"/start_translate.sh &> "$log_name"
    echo_time "Container $campaign stopped"

    #covdirs=(dumps merged_coverage textcov_reports report report_target)
    #for covdir in "${covdirs[@]}"; do
    #    sudo rm -rf "$BASE/oss-fuzz_hopper/build/out/$TARGET/hopper/$ROUND/cov/$covdir"
    #done
}
export -f start_coverage


for FUZZER in "${FUZZERS[@]}"; do
    export FUZZER

    TARGETS=($(get_var_or_default $FUZZER 'TARGETS'))
    for ITARGET in "${TARGETS[@]}"; do
        export TARGET=${ITARGET//_/-}
        export ITARGET=${TARGET//-/_}

        export FUZZER=$FUZZER
        export IMG_NAME="debug-${FUZZER}-${TARGET}"
        echo_time "Building $IMG_NAME"
        if ! "$BASE"/build_trans.sh &> \
            "${LOGDIR}/${IMG_NAME}_build.log"; then
                    echo_time "Failed to build $IMG_NAME. Check build log for info."
                    continue
        fi
        echo_time "Starting trans for $FUZZER $TARGET"
        start_coverage 
    done
done
