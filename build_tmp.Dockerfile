ARG parallelzone_rtag
ARG utilities_rtag
ARG btag

FROM ghcr.io/nwchemex-ci-test/release_parallelzone:$compiler
FROM ghcr.io/nwchemex-ci-test/release_utilities:$compiler

FROM ghcr.io/nwchemex-ci-test/base_pluginplay:$btag

LABEL maintainer="NWChemEx-Project" \
      description="Building environment for PluginPlay based on the base and dpendent release images."

WORKDIR /

COPY --from=0 /install /install

COPY --from=1 /install /install

ENV PATH="`pwd`/install:${PATH}"
