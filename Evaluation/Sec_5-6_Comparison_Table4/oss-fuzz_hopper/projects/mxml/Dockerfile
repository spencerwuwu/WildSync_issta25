
FROM gcr.io/oss-fuzz-base/base-builder

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/michaelrsweet/mxml
RUN git -C mxml checkout d650587449d9d43e2703674d9fcd04217ab53b4e

WORKDIR $SRC/mxml

COPY build.sh $SRC/
COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/mxml/hopper_results/
