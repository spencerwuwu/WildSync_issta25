
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/gdk-pixbuf_hopper
ENV BASE="/home/gdk-pixbuf/"
ENV LIB_PATH="${BASE}/:/home/glib-2.64.2/:/home/glib-2.64.2/glib/"

ENV DEPS_LDD=${BASE}/build/
ENV HEADER=${BASE}/gdk-pixbuf/gdk-pixbuf.h
ENV SHARED_LIB=${BASE}/build/gdk-pixbuf/libgdk_pixbuf-2.0.so

####################

RUN apt-get update && apt-get install -y ffmpeg python3-pip gtk-doc-tools libffi-dev
RUN pip3 install meson==0.55.3 ninja
RUN apt-get install -y libglib2.0-dev 

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/gdk-pixbuf.git
RUN git -C gdk-pixbuf checkout dda3da188c0f8800ad6bf6cd2d039737f1aeb576

# to get glib header
ADD --chown=hopper:hopper https://ftp.gnome.org/pub/gnome/sources/glib/2.64/glib-2.64.2.tar.xz /home/
RUN tar xJf glib-2.64.2.tar.xz 

WORKDIR gdk-pixbuf
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
