ARG btag

FROM ghcr.io/nwchemex-ci-test/base_parallelzone:$btag

RUN    apt-get update \
        && DEBIAN_FRONTEND=noninteractive apt-get install -y \
                libboost-all-dev \
        && apt-get clean \
        && rm -rf /var/lib/apt/lists/*

WORKDIR /

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

LABEL maintainer="NWChemEx-Project" \
      description="Basic building environment (without dependent repos) for PluginPlay based on the ParallelZone base image." \
      libfort_version=${libfort_version}
