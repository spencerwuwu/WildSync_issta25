#!/bin/bash -eu
# - env SHARED: path to directory shared with host (to store results)
# - env TIMEOUT: time to run the campaign
#

rm -rf $SHARED/*

# set default max log size to 1 MiB
LOGSIZE=${LOGSIZE:-$[1 << 20]}

if [ -f /home/setenv.sh ]; then
    source /home/setenv.sh
fi

echo "================="
echo "Start hopper fuzz at $HOPPER_RESULTS_DIR with timeout $TIMEOUT"
timeout $TIMEOUT hopper fuzz $HOPPER_RESULTS_DIR | \
    multilog n2 s$LOGSIZE "$SHARED/log"

chown -R hopper:hopper $SHARED/log
if [ -f "$SHARED/log/current" ]; then
    cat "$SHARED/log/current"
fi

echo "================="
echo "Fuzzing done! Start translating the results..."
cd $SHARED/
mkdir -p queue/ crashes/ violated_crashes/

#for queue in queue crashes; do
#    path=$HOPPER_RESULTS_DIR/$queue
#    for result in $path/*; do
#        filename=$(basename $result)
#        ${HOPPER_RESULTS_DIR}/bin/hopper-translate \
#            --input $result \
#            --output "${SHARED}/${queue}/${filename}.c" \
#            --header $HEADER
#    done
#    chown -R hopper:hopper ${SHARED}/${queue}
#done

set +e

# Translate queue files
for result in $HOPPER_RESULTS_DIR/queue/*; do
    filename=$(basename $result)
    ${HOPPER_RESULTS_DIR}/bin/hopper-translate \
        --input $result \
        --output "${SHARED}/queue/${filename}.c" \
        --header $HEADER
done
#cp -r $HOPPER_RESULTS_DIR/queue/ ${SHARED}/

# Translate crash files based on sanitizer results
for result in $HOPPER_RESULTS_DIR/crashes/*; do
    filename=$(basename $result)
    /home/hopper_sanitize.py $HOPPER_RESULTS_DIR $result
    if [ $? -ne 0 ]; then
        out_dir="violated_crashes"
        cp $result ${SHARED}/${out_dir}/
    else
        out_dir="crashes"
        cp $result ${SHARED}/${out_dir}/
        #${HOPPER_RESULTS_DIR}/bin/hopper-translate \
        #    --input $result \
        #    --output "${SHARED}/${out_dir}/${filename}.c" \
        #    --header $HEADER
    fi
done

cp -r $HOPPER_RESULTS_DIR/misc ${SHARED}/

chown -R hopper:hopper ${SHARED}/

echo "Campaign terminated at $(date '+%F %R')"

