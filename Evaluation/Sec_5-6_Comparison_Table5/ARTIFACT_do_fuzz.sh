#!/bin/bash -e

if [ -z $1 ]; then
    echo "Usage: $0 <experiment_name>"
    exit 1
fi

# load the configuration file (captainrc)
rc=$1

set -a
source "$rc"
set +a
mkdir -p ${WORKDIR}

./run.sh $rc | tee ${WORKDIR}/run.log
sudo chmod -R 777 ${WORKDIR}/
./do_coverage.sh $rc | tee ${WORKDIR}/coverage.log

