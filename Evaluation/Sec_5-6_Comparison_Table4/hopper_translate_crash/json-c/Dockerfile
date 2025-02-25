
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/json-c_hopper
ENV LIB_PATH="/home/json-c/:/home/json-c/build/"

ENV DEPS_LDD=/home/json-c/build
ENV HEADER=/home/json-c/build/json.h
ENV SHARED_LIB=/home/json-c/build/libjson-c.so

####################

RUN apt-get update && apt-get install -y make cmake

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/json-c/json-c.git json-c
RUN git -C json-c checkout 0bc2dd5e60666ebfd2600914c3842bf585d8839b
WORKDIR json-c

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
