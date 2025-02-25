
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libyang_hopper
ENV BASE=/home/libyang/

ENV HEADER=${BASE}/src/libyang.h
ENV SHARED_LIB=${BASE}/build/libyang.so
ENV LIB_PATH="${BASE}/src/:${BASE}/build/:${BASE}/build/libyang"

####################

RUN apt-get update && apt-get install -y autoconf automake libtool

RUN apt-get install -y libpcre2-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/CESNET/libyang libyang
RUN git -C libyang checkout fba28260f382d81cf8f4b91b24cd717b52324fc2

WORKDIR libyang

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

