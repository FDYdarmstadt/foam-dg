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
exec "$repo_dir/docker-launch.sh" "$@"
