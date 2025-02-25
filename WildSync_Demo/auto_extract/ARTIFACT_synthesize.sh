#!/bin/bash

PROJ=vorbis
FUNC=ov_time_seek
SOURCE_HARNESS=./vorbis_fuzzer.cc
EXTERNAL_FILE=external.cpp

INPUT_JSON=pair.json


./do_work.py --debug \
    --input_json $INPUT_JSON \
    -o generated.cc \
    --lib_symbols ./vorbis_symbols.txt \
    $EXTERNAL_FILE $SOURCE_HARNESS
