#!/bin/bash -eu
# - env SHARED: path to directory shared with host (to fetch results)
#
export STORAGE=/hopper_out

rm -rf $STORAGE/*

if [ -f /home/setenv.sh ]; then
    source /home/setenv.sh
fi

echo "================="
echo "Translating out crash files from $SHARED/ to $STORAGE/"

cd $STORAGE
mkdir crashes crashes_translated violated_crashes

set -x
for path in $SHARED/*; do
    round=$(basename $path)
    if [ -d $path/crashes ] && [ "$(ls -A $path/crashes/)" ]; then
        for result in $path/crashes/*; do
            filename=$(basename $result)
            cp $result "${STORAGE}/crashes/${round}_${filename}"
            ${HOPPER_RESULTS_DIR}/bin/hopper-translate \
                --input $result \
                --output "${STORAGE}/crashes_translated/${round}_${filename}.c" \
                --header $HEADER
        done
    fi
    if [ -d $path/violated_crashes ] && [ "$(ls -A $path/violated_crashes/)" ]; then
        for result in $path/violated_crashes/*; do
            filename=$(basename $result)
            cp $result "${STORAGE}/violated_crashes/${round}_${filename}"
        done
    fi
done

