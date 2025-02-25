
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libass_hopper
ENV BASE=/home/libass/

ENV HEADER=${BASE}/libass/ass.h
ENV SHARED_LIB=${BASE}/libass/.libs/libass.so
ENV LIB_PATH="${BASE}:${BASE}/libass/"

####################

RUN apt-get update && apt-get install -y make autoconf automake libtool pkg-config libfontconfig1-dev libfreetype-dev libfribidi-dev python3-pip && \
    pip3 install meson==0.55.0 ninja

RUN apt-get install -y libharfbuzz-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/libass/libass.git
RUN git -C libass checkout 649a7c2e1fc6f4188ea1a89968560715800b883d

#RUN git clone --no-checkout https://github.com/harfbuzz/harfbuzz.git
#RUN git -C harfbuzz checkout 3c8d448fa1174b743963fbc5061a585a255e880b

WORKDIR libass
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

