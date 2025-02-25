
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libxmlb_hopper
ENV BASE=/home/libxmlb/

#ENV HEADER=${BASE}/libtiff/tiff.h
ENV HEADER=${BASE}/src/xmlb.h
ENV SHARED_LIB=${BASE}/build/src/libxmlb.so
ENV LIB_PATH="${BASE}/src/:${BASE}/build/:${BASE}/build/src/:/home/glib/:/home/glib/glib/"

####################
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        make libtool pkg-config gtk-doc-tools autotools-dev automake 

RUN pip3 install meson ninja

RUN apt-get install -y libglib2.0-dev 

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/glib
RUN git -C glib checkout 1e6c6ebaa5330802910fb60256c94fd42b390038

RUN git clone --no-checkout https://github.com/hughsie/libxmlb libxmlb
RUN git -C libxmlb checkout 34d12b333bfd81209e5d657cb2db899f003eef9c

WORKDIR libxmlb

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
