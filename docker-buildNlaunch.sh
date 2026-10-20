#!/usr/bin/env bash
# Build the image, then compile the mounted checkout and launch a shell.
set -euo pipefail
repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
if [[ ${1:-} == --help ]]; then
    echo "Usage: $0 [docker run options...]"
    echo "Build options go in docker-build.sh; IMAGE_NAME selects the image tag."
    exit 0
fi
"$repo_dir/docker-build.sh"
docker run -it --rm --platform linux/amd64 \
    --name "${CONTAINER_NAME:-foamdg7}" \
    --mount "type=bind,source=$repo_dir,target=/home/bosss/foam/foam-dg" \
    -w /home/bosss/foam/foam-dg \
    "$@" "${IMAGE_NAME:-floriankummer81/foamdg7:latest}" \
    bash -c 'foamdg-build && exec bash -l'
