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
    export FUZZERS=(libfuzzer)
fi

if [ -z $ONLY_CATEGORY ]; then
    export ONLY_CATEGORY=0
fi

if [ -z $WORKDIR ] || [ -z $REPEAT ]; then
    echo '$WORKDIR and $REPEAT must be specified as environment variables.'
    exit 1
fi
BASE="$(cd "$(dirname "${BASH_SOURCE[0]}")/" >/dev/null 2>&1 && pwd)"

export BASE
source "$BASE/common.sh"

if [ -z "$WORKER_POOL" ]; then
    WORKER_MODE=${WORKER_MODE:-1}
    WORKERS_ALL=($(lscpu -b -p | sed '/^#/d' | sort -u -t, -k ${WORKER_MODE}g | cut -d, -f1))
    WORKERS=${WORKERS:-${#WORKERS_ALL[@]}}
    export WORKER_POOL="${WORKERS_ALL[@]:0:WORKERS}"
fi

export CAMPAIGN_WORKERS=${CAMPAIGN_WORKERS:-1}

export POLL=${POLL:-5}
export TIMEOUT=${TIMEOUT:-180}

WORKDIR="$(realpath "$WORKDIR")"
export ARDIR="$WORKDIR/ar"
export CACHEDIR="$WORKDIR/cache"
export LOGDIR="$WORKDIR/log"
export LOCKDIR="$WORKDIR/lock"
mkdir -p "$ARDIR"
mkdir -p "$CACHEDIR"
mkdir -p "$LOGDIR"
mkdir -p "$LOCKDIR"

shopt -s nullglob
rm -f "$LOCKDIR"/*
shopt -u nullglob

export MUX_TAR=magma_tar
export MUX_CID=magma_cid

get_next_cid()
{
    ##
    # Pre-requirements:
    # - $1: the directory where campaigns are stored
    ##
    shopt -s nullglob
    campaigns=("$1"/*)
    if [ ${#campaigns[@]} -eq 0 ]; then
        echo 0
        dir="$1/0"
    else
        cids=($(sort -n < <(basename -a "${campaigns[@]}")))
        for ((i=0;;i++)); do
            if [ -z ${cids[i]} ] || [ ${cids[i]} -ne $i ]; then
                echo $i
                dir="$1/$i"
                break
            fi
        done
    fi
    # ensure the directory is created to prevent races
    mkdir -p "$dir"
    while [ ! -d "$dir" ]; do sleep 1; done
}
export -f get_next_cid

mutex()
{
    ##
    # Pre-requirements:
    # - $1: the mutex ID (file descriptor)
    # - $2..N: command to run
    ##
    trap 'rm -f "$LOCKDIR/$mux"' EXIT
    mux=$1
    shift
    (
      flock -xF 200 &> /dev/null
      "${@}"
    ) 200>"$LOCKDIR/$mux"
}
export -f mutex

shorten()
{
file=$1
nlines=$2
while read line ; do
    echo "$line" >> $file
    read clines _ <<< $(wc -l $file)
    if [[ clines -gt nlines ]] ; then
        n=$(( clines - nlines ))
        sed -i -e 1,${n}d $file
    fi
done
}
export -f shorten

start_campaign()
{
    launch_campaign()
    {
        # TODO: how to start campaign
        # probably set $SHARED as corpus_storage
        export SHARED="$CAMPAIGN_CACHEDIR/$CACHECID"
        mkdir -p "$SHARED" && chmod 777 "$SHARED"

		campaign="$FUZZER/$TARGET/$PROGRAM/$ARCID"
		log_name="${LOGDIR}/${FUZZER}_${TARGET}_${PROGRAM}_${ARCID}_container.log"
        echo_time "Container $campaign started on CPU $AFFINITY"
        #if [[ "$FUZZER" == "libfuzzer" && "$TARGET" == "libgd" ]]; then
        #    echo "Reduce log as it expolits" > $log_name
        #    "$BASE"/start.sh &> /dev/null
        #else
        #    "$BASE"/start.sh &> $log_name
        #fi
        "$BASE"/start.sh 2>&1 | shorten $log_name 2000
        echo_time "Container $campaign stopped"


        # overwrites empty $ARCID directory with the $SHARED directory
        mv -T "$SHARED" "${CAMPAIGN_ARDIR}/${ARCID}"
    }
    export -f launch_campaign

	if [ ! -z "$BUG" ]; then
		target_bug="$TARGET-$BUG"
	else
		target_bug="$TARGET"
	fi
    while : ; do
        export CAMPAIGN_CACHEDIR="$CACHEDIR/$FUZZER/$target_bug/$PROGRAM"
        export CACHECID=$(mutex $MUX_CID \
                get_next_cid "$CAMPAIGN_CACHEDIR")
        export CAMPAIGN_ARDIR="$ARDIR/$FUZZER/$target_bug/$PROGRAM"
        export ARCID=$(mutex $MUX_CID \
                get_next_cid "$CAMPAIGN_ARDIR")

        errno_lock=69
        SHELL=/bin/bash flock -xnF -E $errno_lock "${CAMPAIGN_CACHEDIR}/${CACHECID}" \
            flock -xnF -E $errno_lock "${CAMPAIGN_ARDIR}/${ARCID}" \
                -c launch_campaign || \
        if [ $? -eq $errno_lock ]; then
            continue
        fi
        break
    done
}
export -f start_campaign

start_ex()
{
    release_workers()
    {
        IFS=','
        read -a workers <<< "$AFFINITY"
        unset IFS
        for i in "${workers[@]}"; do
            rm -rf "$LOCKDIR/magma_cpu_$i"
        done
    }
    trap release_workers EXIT

    start_campaign
    exit 0
}
export -f start_ex

allocate_workers()
{
    ##
    # Pre-requirements:
    # - env NUMWORKERS
    # - env WORKERSET
    ##
    cleanup()
    {
        IFS=','
        read -a workers <<< "$WORKERSET"
        unset IFS
        for i in "${workers[@]:1}"; do
            rm -rf "$LOCKDIR/magma_cpu_$i"
        done
        exit 0
    }
    trap cleanup SIGINT

    while [ $NUMWORKERS -gt 0 ]; do
        for i in $WORKER_POOL; do
            if ( set -o noclobber; > "$LOCKDIR/magma_cpu_$i" ) &>/dev/null; then
                export WORKERSET="$WORKERSET,$i"
                export NUMWORKERS=$(( NUMWORKERS - 1 ))
                allocate_workers
                return
            fi
        done
        # This times-out every 1 second to force a refresh, since a worker may
        #   have been released by the time inotify instance is set up.
        inotifywait -qq -t 1 -e delete "$LOCKDIR" &> /dev/null
    done
    cut -d',' -f2- <<< $WORKERSET
}
export -f allocate_workers

cleanup()
{
    trap 'echo Cleaning up...' SIGINT
    echo_time "Waiting for jobs to finish"
    for job in `jobs -p`; do
        if ! wait $job; then
            continue
        fi
    done

    find "$LOCKDIR" -type f | while read lock; do
        if inotifywait -qq -e delete_self "$lock" &> /dev/null; then
            continue
        fi
    done
}

trap cleanup EXIT

for FUZZER in "${FUZZERS[@]}"; do
    export FUZZER

    TARGETS=($(get_var_or_default $FUZZER 'TARGETS'))
    for ITARGET in "${TARGETS[@]}"; do
        export TARGET=${ITARGET//_/-}
        export ITARGET=${TARGET//-/_}

        export FUZZER=$FUZZER
        export IMG_NAME="${FUZZER}-${TARGET}"
        echo_time "Building $IMG_NAME"
        if ! "$BASE"/build.sh &> \
            "${LOGDIR}/${IMG_NAME}_build.log"; then
                echo_time "Failed to build $IMG_NAME. Check build log for info."
                continue
        fi
        PROGRAMS=($(get_var_or_default $FUZZER $ITARGET 'PROGRAMS'))
        for PROGRAM in "${PROGRAMS[@]}"; do
            export PROGRAM
            echo_time "Starting campaigns for $FUZZER $TARGET $PROGRAM"
            for ((i=0; i<$REPEAT; i++)); do
                export NUMWORKERS=1
                export AFFINITY=$(allocate_workers)
                start_ex &
            done
        done
    done
done
