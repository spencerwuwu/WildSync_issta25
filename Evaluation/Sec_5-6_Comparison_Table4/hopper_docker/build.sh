#!/bin/bash
docker build -t oss-fuzz-hopper --build-arg USER_ID=$(id -u $USER) --build-arg GROUP_ID=$(id -g $USER) .

