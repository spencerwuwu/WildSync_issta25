#!/bin/bash -eu



export LDFLAGS=$CFLAGS
./configure
make
