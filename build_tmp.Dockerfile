ARG parallelzone_rtag
ARG utilities_rtag
ARG btag

FROM ghcr.io/nwchemex-ci-test/release_parallelzone:$parallelzone_rtag
FROM ghcr.io/nwchemex-ci-test/release_utilities:$utilities_rtag

FROM ghcr.io/nwchemex-ci-test/base_pluginplay:$btag

LABEL maintainer="NWChemEx-Project" \
      description="Building environment for PluginPlay based on the base and release images of the dependent repos."

WORKDIR /

COPY --from=0 /install /install

COPY --from=1 /install /install

ENV PATH="`pwd`/install:${PATH}"
