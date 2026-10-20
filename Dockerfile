# Build floriankummer81/foamdg7 with ./docker-build.sh.
# Only foam-dg is mounted at runtime; foam-extend and BoSSS are built here.
FROM ubuntu:26.04

ARG DEBIAN_FRONTEND=noninteractive
ARG USERNAME=bosss
ARG UID=1000
ARG GID=1000
ARG FOAM_EXTEND_REPO=https://github.com/FDYdarmstadt/foam-extend-for-dg.git
ARG FOAM_EXTEND_REF=master

SHELL ["/bin/bash", "-o", "pipefail", "-c"]
# The bridge and distributed BoSSS native libraries currently target Linux x64.
RUN test "$(dpkg --print-architecture)" = amd64

RUN apt-get update \
	&& echo "path-include=/usr/share/doc/libprrte-dev/help-*" >> /etc/dpkg/dpkg.cfg.d/excludes \
	&& apt-get install -y --no-install-recommends \
		ca-certificates \
		curl \
		git \
		gnupg \
		libgssapi-krb5-2 \
		libicu-dev \
		libopenmpi-dev \
		libprrte-dev \
		libssl-dev \
		libstdc++6 \
		libpmix-bin \
		openmpi-bin \
		openmpi-common \
		octave \
		gnuplot \
		openssh-client \
		sudo \
		wget \
		nano \
		zlib1g \
	&& MULTIARCH="$(dpkg -L libprrte3 2>/dev/null | sed -n 's|^/usr/lib/\([^/]*\)/libprrte\.so.*|\1|p' | head -n1)" \
	&& if [ -z "$MULTIARCH" ]; then MULTIARCH="$(uname -m)-linux-gnu"; fi \
	&& PRTE_HELP_DIR="/usr/lib/$MULTIARCH/prrte3/share/prte" \
	&& if [ ! -f "$PRTE_HELP_DIR/help-prun.txt" ]; then \
		if ! ls /usr/share/doc/libprrte-dev/help-*.txt* >/dev/null 2>&1; then \
			echo "ERROR: PRRTE help files not found in libprrte-dev; check dpkg path-include and LP#2155666" >&2; \
			exit 1; \
		fi; \
		mkdir -p "$PRTE_HELP_DIR" \
		&& cp /usr/share/doc/libprrte-dev/help-*.txt* "$PRTE_HELP_DIR/" \
		&& for help_file in "$PRTE_HELP_DIR"/*.gz; do \
			[ ! -f "$help_file" ] || gzip -df "$help_file"; \
		done; \
	fi \
	&& rm -rf /var/lib/apt/lists/*

RUN mpiexec --version

# GCC 12 keeps the foam-extend build consistent with the validated environment.
# libfl-dev is required by the generated STL parser even with no recommends.
RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		build-essential \
		gcc \
		g++ \
		gcc-12 \
		g++-12 \
		gfortran \
		flex \
		libfl-dev \
		bison \
		cmake \
		m4 \
		libreadline-dev \
		libncurses-dev \
		libxt-dev \
		zlib1g-dev \
		python3 \
		python3-numpy \
		python3-scipy \
		python3-matplotlib \
	&& rm -rf /var/lib/apt/lists/*

RUN gcc --version && g++ --version && gcc-12 --version && g++-12 --version && flex --version && bison --version

# Install .NET 10, including its native hosting pack for nethost/hostfxr.
RUN curl -fsSL https://dot.net/v1/dotnet-install.sh -o /tmp/dotnet-install.sh \
	&& chmod +x /tmp/dotnet-install.sh \
	&& /tmp/dotnet-install.sh --channel 10.0 --install-dir /usr/share/dotnet --no-path \
	&& ln -sfn /usr/share/dotnet/dotnet /usr/bin/dotnet \
	&& rm /tmp/dotnet-install.sh

ENV DOTNET_ROOT=/usr/share/dotnet
ENV PATH=/usr/share/dotnet:${PATH}

# Non-root user by default (recommended for OpenMPI usage and safer dev workflows).
# The base image may already have a user/group with the host's IDs (e.g. ubuntu
# at 1000:1000). Reuse that group and rename/move that user when necessary.
RUN if ! getent group "${GID}" >/dev/null; then \
		groupadd --gid "${GID}" "${USERNAME}"; \
	fi \
	&& if getent passwd "${UID}" >/dev/null; then \
		existing_user="$(getent passwd "${UID}" | cut -d: -f1)" \
		&& if [ "$existing_user" != "${USERNAME}" ]; then \
			usermod --login "${USERNAME}" "$existing_user"; \
		fi \
		&& usermod --gid "${GID}" --home "/home/${USERNAME}" --move-home --shell /bin/bash "${USERNAME}"; \
	else \
		useradd --uid "${UID}" --gid "${GID}" -m -s /bin/bash "${USERNAME}"; \
	fi \
	&& usermod -aG sudo ${USERNAME} \
	&& echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME} \
	&& chmod 0440 /etc/sudoers.d/${USERNAME}


ENV HOME=/home/${USERNAME}
ENV WM_MPLIB=SYSTEMOPENMPI WM_CC=gcc-12 WM_CXX=g++-12
ENV FOAM_DG_ROOT=/home/${USERNAME}/foam/foam-dg
ENV BOSSS_SOURCE=/home/${USERNAME}/bosss
ENV BOSSS_INSTALL=/home/${USERNAME}/BoSSS-Install
ENV OMP_NUM_THREADS=1

USER ${USERNAME}
WORKDIR /home/${USERNAME}
RUN mkdir -p /home/${USERNAME}/foam
RUN git clone --depth 1 --branch ${FOAM_EXTEND_REF} ${FOAM_EXTEND_REPO} /home/${USERNAME}/foam/foam-extend-4.1

# Upstream contains the GCC fixes; no source patching is performed.
RUN bash -lc ' \
	set -e; \
	set -o pipefail; \
	export WM_MPLIB=SYSTEMOPENMPI; \
	export WM_CC=gcc-12; \
	export WM_CXX=g++-12; \
	source /home/'"${USERNAME}"'/foam/foam-extend-4.1/etc/bashrc; \
	cd "$WM_PROJECT_DIR"; \
	./Allwmake 2>&1 | tee /home/'"${USERNAME}"'/foam-extend-build.log; \
	echo "=== verifying expected build outputs ==="; \
	test -f "$FOAM_LIBBIN/libfoam.so"; \
	test -f "$FOAM_LIBBIN/libfiniteVolume.so"; \
	test -x "$FOAM_APPBIN/blockMesh"; \
	test -x "$FOAM_APPBIN/icoFoam"; \
	echo "All expected foam-extend outputs present." \
	'

# Build the generator and the coupled solver's managed dependency graph.
# This value refreshes only the BoSSS clone/build layers on each helper invocation.
ARG BOSSS_CACHE_BUST=manual
RUN echo "BoSSS source refresh: ${BOSSS_CACHE_BUST}" \
    && git clone --depth 1 --branch premaster --single-branch \
        https://github.com/FDYdarmstadt/BoSSS.git "$BOSSS_SOURCE"
RUN test -f "$BOSSS_SOURCE/src/L4-application/ExternalBinding.CodeGen/NativeGlobals.cpp.txt" \
    || { echo 'BoSSS premaster does not yet contain the .NET hosting migration; retry after the public merge.' >&2; exit 1; }
RUN dotnet build "$BOSSS_SOURCE/src/L4-application/ExternalBinding.CodeGen/ExternalBinding.CodeGen.csproj" \
        -c Release -p:ExcludeNativeExports=true \
    && dotnet publish "$BOSSS_SOURCE/src/L4-application/ExternalBinding/ExternalBinding.csproj" \
        -c Release --self-contained false -o "$HOME/bosss-published"

# docker-build.sh stages the supplied native distribution into the build context.
COPY --chown=${UID}:${GID} bosss-native/ /home/${USERNAME}/BoSSS-Install/bin/native/linux/amd64-openmpi/
RUN test -f "$BOSSS_INSTALL/bin/native/linux/amd64-openmpi/libBoSSSnative_omp.so" \
    && test -f "$BOSSS_INSTALL/bin/native/linux/amd64-openmpi/libBoSSSnative_seq.so" \
    && test -f "$BOSSS_INSTALL/bin/native/linux/amd64-openmpi/libBoSSSnative_mpi.so" \
    && cd "$BOSSS_INSTALL" \
    && find bin/native -type f -exec sha256sum {} + | LC_ALL=C sort -k2 > "$HOME/bosss-native.sha256"
RUN printf 'foam-extend=%s\nBoSSS=%s\nBoSSS-branch=premaster\n.NET=%s\n' \
        "$(git -C "$HOME/foam/foam-extend-4.1" rev-parse HEAD)" \
        "$(git -C "$BOSSS_SOURCE" rev-parse HEAD)" "$(dotnet --version)" > "$HOME/foamdg-VERSION" \
    && mkdir -p "$FOAM_DG_ROOT"

COPY --chown=${UID}:${GID} docker/foamdg-env.sh /home/${USERNAME}/.foamdg-env.sh
COPY docker/foamdg-entrypoint.sh /usr/local/bin/foamdg-entrypoint
COPY docker/foamdg-build.sh /usr/local/bin/foamdg-build
RUN echo '[ -f "$HOME/.foamdg-env.sh" ] && . "$HOME/.foamdg-env.sh"' >> "$HOME/.bashrc" \
    && echo '[ -f "$HOME/.foamdg-env.sh" ] && . "$HOME/.foamdg-env.sh"' >> "$HOME/.profile"
# Enable automatic startup only after the remaining build-time shell work.
ENV BASH_ENV=/home/${USERNAME}/.foamdg-env.sh

WORKDIR /home/${USERNAME}/foam/foam-dg
ENTRYPOINT ["foamdg-entrypoint"]
CMD ["bash", "-l"]
