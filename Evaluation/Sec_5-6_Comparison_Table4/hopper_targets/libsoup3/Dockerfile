
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libsoup_hopper
ENV BASE=/home/libsoup/

ENV HEADER=${BASE}/libsoup/soup.h
ENV SHARED_LIB=${BASE}/build/libsoup/libsoup-3.0.so
ENV LIB_PATH="${BASE}/:${BASE}/build:${BASE}/libsoup/:${BASE}/build/libsoup:/usr/local/include/glib-2.0/"

####################

RUN apt-get update && \
    apt-get install -y python3-pip gtk-doc-tools libffi-dev autotools-dev \
            libtool gperf libnghttp2-dev libsqlite3-dev libpsl-dev && \
    pip3 install meson ninja

ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/glib
RUN git -C glib checkout 1e6c6ebaa5330802910fb60256c94fd42b390038

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/libsoup
RUN git -C libsoup checkout 521a861524042dda0835cd7a5d4fb0bf29f2896b

COPY ${TARGET}/hopper_build.sh $SRC
RUN $SRC/hopper_build.sh

RUN chown -R hopper:hopper /home

USER hopper:hopper
WORKDIR libsoup
#COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY --chown=hopper:hopper hopper_build-addon.sh /home/
RUN echo "#!/bin/bash -eu" > /home/hopper_build.sh
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

