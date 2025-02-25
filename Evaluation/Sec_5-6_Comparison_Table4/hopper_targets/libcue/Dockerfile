
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libcue_hopper
ENV BASE=/home/libcue/

#ENV HEADER=${BASE}/libtiff/tiff.h
ENV HEADER=${BASE}/libcue.h
ENV SHARED_LIB=${BASE}/build/libcue.so
ENV LIB_PATH="${BASE}/"

####################

RUN apt-get update && apt-get install -y make autoconf automake libtool bison flex

WORKDIR /home
USER hopper:hopper


RUN git clone --no-checkout https://github.com/lipnitsk/libcue.git libcue
RUN cd libcue && git checkout 7176a1faccecbfe2d4cca2f776177439ca49cad2

WORKDIR libcue

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]


