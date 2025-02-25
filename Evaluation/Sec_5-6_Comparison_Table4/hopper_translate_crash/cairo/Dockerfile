
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/cairo_hopper
ENV BASE="/home/cairo/"
ENV LIB_PATH="${BASE}/src:${BASE}/boilerplate/:${BASE}/perf/:${BASE}/src/win32"

ENV DEPS_LDD=${BASE}/build/
ENV HEADER=${BASE}/src/cairo.h
ENV SHARED_LIB=${BASE}/build/src/libcairo.so

####################

RUN apt-get update && \
    apt-get install -y python3-pip gtk-doc-tools libffi-dev autotools-dev libtool gperf vim
RUN apt-get install -y libglib2.0-dev libfreetype-dev
RUN pip3 install -U meson==1.3.0 ninja packaging

WORKDIR /home
USER hopper:hopper

#RUN git clone --no-checkout git://git.sv.nongnu.org/freetype/freetype2.git
#RUN git -C freetype2 checkout f42ce25563b73fed0123d18a2556b9ba01d2c76b
#RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/glib
#RUN git -C glib checkout 308b06d3523106daa3b4828928fa7772b24defe7
RUN git clone --no-checkout https://gitlab.freedesktop.org/cairo/cairo.git
RUN git -C cairo checkout bbbc68e6195ba75bcb3ef71a4be8082aa6619862

WORKDIR /home/cairo

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

COPY --chown=hopper:hopper ${TARGET}/setenv.sh /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
