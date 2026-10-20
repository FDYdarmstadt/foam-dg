#!/usr/bin/env bash
# Compile the mounted checkout and open a shell using an existing image.
set -euo pipefail
repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
if [[ ${1:-} == --help ]]; then
    echo "Usage: $0 [docker run options...]"
    echo "Uses IMAGE_NAME (default: floriankummer81/foamdg7:latest) without rebuilding it."
    exit 0
fi
echo "Launching ${CONTAINER_NAME:-foamdg7} from ${IMAGE_NAME:-floriankummer81/foamdg7:latest}; compiling the mounted foam-dg checkout."
exec docker run -it --rm --platform linux/amd64 \
    --name "${CONTAINER_NAME:-foamdg7}" \
    --mount "type=bind,source=$repo_dir,target=/home/bosss/foam/foam-dg" \
    -w /home/bosss/foam/foam-dg \
    "$@" "${IMAGE_NAME:-floriankummer81/foamdg7:latest}" \
    bash -c 'foamdg-build && exec bash -l'
