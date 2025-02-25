#!/bin/bash -e

BASE="$(cd "$(dirname "${BASH_SOURCE[0]}")/../" >/dev/null 2>&1 && pwd)"

demopath="${BASE}/WildSync_Demo/"

# Install deps and WildSync
pip install "numpy<2" setuptools
pushd ${demopath}/comex
pip install -r requirements-dev.txt
popd
pushd ${demopath}/auto_extract
pip install -r requirements-dev.txt
popd

hopperpath="${BASE}/Evaluation/Sec_5-6_Comparison_Table4/"
pushd ${hopperpath}/hopper_docker/
./build.sh
popd

pip install prettytable
