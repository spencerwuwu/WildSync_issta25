#!/bin/bash -e

# load the configuration file (captainrc)
rc=$1

set -a
source "$rc"
set +a

echo "INFO: Parsing LCOV file for coverage information..."
./parse_lcov.py $WORKDIR
echo ""

echo "INFO: Generating coverage report..."
echo "-----------------------------------------------------------"
./calculate_parsed_lcov.py

echo "-----------------------------------------------------------"
echo "INFO: Parsing crash list"
./parse_libfuzzer_logs.py $WORKDIR
