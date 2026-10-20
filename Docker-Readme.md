# Docker image: foamdg7

`floriankummer81/foamdg7` is the .NET 10 successor to the obsolete Mono-based
`foamdg6` images. Its [Dockerfile](Dockerfile) builds Ubuntu 26.04,
foam-extend-for-dg with GCC 12 and system OpenMPI, and the public BoSSS
**`premaster`** branch in `/home/bosss/bosss`. No foam-extend patches or AI CLI
installations are needed.

The image contains the compiled BoSSS generator, ExternalBinding and its managed
dependencies, plus the supplied BoSSS native libraries. Only the host `foam-dg`
checkout is mounted, at `/home/bosss/foam/foam-dg`. Generated wrappers, managed
runtime files, case inputs and results therefore remain in that checkout.

The public BoSSS `premaster` branch must contain the completed .NET hosting
migration before this image can build. The Dockerfile checks for it and fails
with an explicit message if the public merge has not arrived. The new Docker
Hub image is intended for publication after that build and validation succeed.

## Build and launch from source

Clone this repository and obtain a Linux x64 BoSSS native installation containing
`bin/native/linux/amd64-openmpi/libBoSSSnative_{seq,omp,mpi}.so`, including its
`dependencies` directory. The native libraries are distributed separately from
the BoSSS managed source. A defined `BOSSS_INSTALL` takes precedence and is used
without any native-library download. An invalid local installation fails with
an error rather than falling back to the network.

```bash
git clone https://github.com/FDYdarmstadt/foam-dg.git
cd foam-dg
export BOSSS_INSTALL=/absolute/path/to/BoSSS-Install
./docker-buildNlaunch.sh
```

If `BOSSS_INSTALL` is unset or empty, the build downloads and extracts the
native installer into a temporary directory. The download URL is defined by
`BOSSS_INSTALL_URL` near the top of `docker-build.sh`; edit its default when the
homepage returns, or override it for a build:

```bash
unset BOSSS_INSTALL
BOSSS_INSTALL_URL=https://your-host/BoSSS-setup-latest.run ./docker-build.sh
```

The current default points to the old homepage, which is temporarily unavailable;
use a local installation until it returns or supply a working replacement URL.
Downloaded sign-in/error pages are rejected before attempting extraction.
The build helper copies only the native library directory into the build
context and removes the downloaded installer and extracted managed files. It is baked into the image, so no host BoSSS checkout or native-library
mount is needed when running it. The shell runs as `bosss`, with the building
user's UID/GID so generated files belong to that host user.

The launcher builds the image, regenerates the C++ wrappers and .NET adapters
against the image's BoSSS source, publishes them into the mounted
`src/discontinuousGalerkin/BoSSSwrapper/rel`, builds `foam-dg`, and opens a shell.
It exits if compilation fails. Additional arguments are Docker run options:

```bash
./docker-buildNlaunch.sh --cpus=8
```

Building only the image:

```bash
./docker-build.sh
# Docker build flags are forwarded, for example:
./docker-build.sh --no-cache
```

The default tag is `floriankummer81/foamdg7:latest`. Override it with `IMAGE_NAME`
in any helper. BoSSS is cloned afresh on each image build while the
expensive foam-extend layers remain cached. Set `BOSSS_CACHE_BUST` to a stable
value to reuse the BoSSS layers too. `FOAM_EXTEND_REF` is an optional Docker build
argument; the selected revision must already contain the upstream compiler fixes.
Builds target `linux/amd64`, as required by the current native hosting pack and
BoSSS native distribution.

For a direct `docker build`, stage a context containing `Dockerfile`, `docker/`
and the contents of the native `amd64-openmpi` directory under `bosss-native/`.
`docker-build.sh` performs this staging automatically.

## Run an existing image

Launch an image you have already built with `./docker-launch.sh`, optionally
passing Docker run options, for example `./docker-launch.sh --cpus=8`.
It regenerates bindings and builds the mounted checkout before opening a shell,
without rebuilding the Docker image. `IMAGE_NAME` and `CONTAINER_NAME` can be
overridden in the environment. `docker-buildNlaunch.sh` uses this same launcher
after building the image.

After the new image has been published:

```bash
docker pull floriankummer81/foamdg7:latest
docker run -it --rm --platform linux/amd64 \
  --mount "type=bind,source=$(pwd),target=/home/bosss/foam/foam-dg" \
  floriankummer81/foamdg7:latest \
  bash -c 'foamdg-build && exec bash -l'
```

Run this from the host `foam-dg` checkout. A published image uses its publisher's
UID/GID; ensure that its `bosss` user can write to the checkout. Building locally
with the helpers matches your own UID/GID automatically.

`foamdg-build` can be rerun inside the container after source changes. Direct
commands receive the foam-extend and native-library environment through the
entrypoint; Bash shells also receive it through their startup files. For a
running container named `foamdg7`, use `docker exec -it foamdg7 bash -l`.

## Examples

Inside the container, run the one-dimensional convergence study:

```bash
cd "$FOAM_DG_ROOT/run/Tests/convAnalysis1D"
./Allrun
```

It runs the coarse, medium and fine grids and reports the convergence order
(approximately three), producing `conv1D.png` and `convergencePlot1D.png`.

Run a droplet-in-shear-flow example:

```bash
cd "$FOAM_DG_ROOT/run/dropletInShearFlowBig_p3"
./Allrun
```

The `_p1`, `_p2` and `_p3` cases select the managed BoSSS polynomial degree in
their `bosss/config.json` files; retain those subdirectories when copying cases.
The solver writes Tecplot `.plt` output and reports the deformation parameter
on standard output. These simulations can take substantial time. Results are
written to the mounted host checkout.

## Provenance and Docker Hub publication

`/home/bosss/foamdg-VERSION` records the BoSSS and foam-extend commits and .NET
SDK version. `/home/bosss/bosss-native.sha256` records the native-library file
checksums. After `foamdg-build`, `foamdg-mounted-VERSION` adds the mounted
`foam-dg` commit and any tracked modifications. The host repository is compiled
at launch rather than copied into the image.

After the public BoSSS merge, validate the build and the coupled solver before
publishing a dated release tag and `latest`, for example:

```bash
export IMAGE_NAME=floriankummer81/foamdg7:YYYYMMDD
./docker-buildNlaunch.sh
# Run the examples above inside the container, then exit.
docker tag "$IMAGE_NAME" floriankummer81/foamdg7:latest
docker login
docker push "$IMAGE_NAME"
docker push floriankummer81/foamdg7:latest
```

Use the dated tag or an image digest when recording results. Publication is a
separate step; the build helpers do not push images.
