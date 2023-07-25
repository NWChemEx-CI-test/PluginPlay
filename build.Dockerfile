ARG parallelzone_rtag
ARG utilities_rtag
ARG parallelzone_btag

FROM ghcr.io/nwchemex-ci-test/release_parallelzone:$parallelzone_rtag

FROM ghcr.io/nwchemex-ci-test/release_utilities:$utilities_rtag

FROM ghcr.io/nwchemex-ci-test/build_parallelzone:$parallelzone_btag

LABEL maintainer="NWChemEx-Project" \
      description="Basic building environment for PluginPlay based on the ubuntu 20.04 image."

RUN    apt-get update \
        && DEBIAN_FRONTEND=noninteractive apt-get install -y \
                libboost-all-dev \
        && apt-get clean \
        && rm -rf /var/lib/apt/lists/*

WORKDIR /

COPY --from=0 /install /install

COPY --from=1 /install /install

ARG libfort_version=0.4.2

ENV INSTALL_PATH=../install

RUN wget --no-check-certificate --content-disposition https://codeload.github.com/seleznevae/libfort/tar.gz/refs/tags/v${libfort_version} \
    && tar -zxf libfort-${libfort_version}.tar.gz \
    && cd libfort-${libfort_version} \
    && cmake -GNinja -H. -Bbuild -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} \
    && cmake --build build \
    && cmake --build build --target install \
    && cd .. \
    && rm -rf libfort-${libfort_version} libfort-${libfort_version}.tar.gz

ENV PATH="`pwd`/install:${PATH}"
