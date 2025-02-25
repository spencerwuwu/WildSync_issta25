
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/mxml_hopper
ENV BASE=/home/mxml/

ENV HEADER=${BASE}/mxml.h
ENV SHARED_LIB=${BASE}/libmxml4.so
ENV LIB_PATH="${BASE}"

####################


WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/michaelrsweet/mxml
RUN git -C mxml checkout d650587449d9d43e2703674d9fcd04217ab53b4e

WORKDIR mxml

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

