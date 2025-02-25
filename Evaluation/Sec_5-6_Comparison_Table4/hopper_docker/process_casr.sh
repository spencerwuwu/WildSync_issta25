#!/bin/bash

rm -rf /hopper_out/*

mkdir -p /home/crash_build/casr

cd /home
DEBUG=1 ./hopper_casr.py /home/crash_build/hopper-builds /home/crash_build/

mv /home/crash_build/* /hopper_out

chown -R hopper:hopper /hopper_out
