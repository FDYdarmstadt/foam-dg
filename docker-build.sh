#!/usr/bin/env bash
# Build the image; native libraries are copied into it, never mounted at runtime.
set -euo pipefail
repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
# Replace this URL when the public installer is available again, or override
# BOSSS_INSTALL_URL in the environment. Used only when BOSSS_INSTALL is unset.
BOSSS_INSTALL_URL=${BOSSS_INSTALL_URL:-https://kummer.pages.rwth-aachen.de/bosss-public/BoSSS-setup-latest.run}
if [[ ${1:-} == --help ]]; then
    echo "Usage: [BOSSS_INSTALL=/path/to/BoSSS-Install] $0 [docker build options...]"
    echo "Without BOSSS_INSTALL, downloads the installer from BOSSS_INSTALL_URL."
    echo "IMAGE_NAME defaults to floriankummer81/foamdg7:latest. Builds Linux amd64."
    exit 0
fi
build_context=$(mktemp -d)
trap 'rm -rf -- "$build_context"' EXIT
if [[ -n ${BOSSS_INSTALL:-} ]]; then
    native_install=$BOSSS_INSTALL
else
    installer="$build_context/BoSSS-setup.run"
    echo "BOSSS_INSTALL is unset; downloading $BOSSS_INSTALL_URL"
    curl --fail --location --silent --show-error "$BOSSS_INSTALL_URL" -o "$installer"
    if [[ $(head -c 2 "$installer") != '#!' ]]; then
        echo "The download is not a BoSSS shell installer. Set BOSSS_INSTALL or update BOSSS_INSTALL_URL." >&2
        exit 2
    fi
    native_install="$build_context/BoSSS-Install"
    bash "$installer" --noexec --target "$native_install"
fi
native_dir="$native_install/bin/native/linux/amd64-openmpi"
for library in libBoSSSnative_seq.so libBoSSSnative_omp.so libBoSSSnative_mpi.so; do
    if [[ ! -f "$native_dir/$library" ]]; then
        echo "Missing $native_dir/$library. Set BOSSS_INSTALL to a valid BoSSS native installation." >&2
        exit 2
    fi
done
cp "$repo_dir/Dockerfile" "$build_context/Dockerfile"
cp -a "$repo_dir/docker" "$build_context/docker"
mkdir "$build_context/bosss-native"
# Dereference distribution symlinks so the image does not depend on host paths.
cp -aL "$native_dir/." "$build_context/bosss-native/"
# Keep downloaded installer payloads and managed binaries out of the context.
if [[ -z ${BOSSS_INSTALL:-} ]]; then
    rm -rf -- "$installer" "$native_install"
fi
docker build --platform linux/amd64 -t "${IMAGE_NAME:-floriankummer81/foamdg7:latest}" \
    --build-arg UID="$(id -u)" --build-arg GID="$(id -g)" \
    --build-arg BOSSS_CACHE_BUST="${BOSSS_CACHE_BUST:-$(date -u +%Y%m%dT%H%M%SZ)}" \
    "$@" "$build_context"
