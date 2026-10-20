# OpenFOAM-BoSSS Bridge

This library makes discontinuous Galerkin (DG) functionality from BoSSS
available in OpenFOAM. It demonstrates coupling C++ and C# through .NET 10
nethost/hostfxr and generated UnmanagedCallersOnly adapters.


## Docker quick start

The new `floriankummer81/foamdg7` image builds foam-extend and the public BoSSS
`premaster` branch. From this checkout, use your local BoSSS native installation:

```bash
export BOSSS_INSTALL=/path/to/BoSSS-Install
./docker-buildNlaunch.sh
```

If `BOSSS_INSTALL` is unset, the build downloads the native installer using
`BOSSS_INSTALL_URL`, defined near the top of `docker-build.sh` and overridable in
the environment. Set it to the restored homepage URL when available.

Only this `foam-dg` checkout is mounted; BoSSS lives in `~/bosss` inside the
image. The launcher generates matching bindings and compiles the mounted source.
Use `./docker-build.sh` to build only the image, or `./docker-launch.sh` to
launch an existing image without rebuilding it. See
[Docker-Readme.md](Docker-Readme.md) for native-library requirements, running an
existing image, examples and publication instructions. The public BoSSS
`premaster` merge containing the .NET hosting migration is required before the
image can build. `foamdg6` is obsolete for current source.

## Prerequisites for a manual build

- modified version of `foam-extend4.1`,  see https://github.com/FDYdarmstadt/foam-extend-for-dg
- A current .NET 10 BoSSS checkout, branch `premaster`, from https://github.com/FDYdarmstadt/BoSSS
- .NET 10 SDK (including the Linux x64 native hosting pack) and BoSSS native libraries

## Building/running instructions

To use this code, follow these instructions:

- clone this repository:
``` bash
mkdir ~/foam/
git clone https://github.com/FDYdarmstadt/foam-dg.git ~/foam/foam-dg/
```

- source `foam-extend`'s `bashrc`, i.e. something like (`foam-extend` users may already have defined a bash alias for this)
``` bash
source ~/foam/foam-extend-4.1/etc/bashrc # adjust according to where you cloned foam-extend
```

- export the `$FOAM_DG_ROOT`-variable (you may also want to put this in your `~/.bashrc` or wherever you set your bash variables):
``` bash
export FOAM_DG_ROOT=~/foam/foam-dg/ # adjust according to where you cloned this repository
```

- build and deploy matching C++ wrappers and .NET adapters (adjust both paths):
```bash
export BOSSS_INSTALL=/path/to/BoSSS-Install
bash "$FOAM_DG_ROOT/src/discontinuousGalerkin/BoSSSwrapper/createBindings.sh" /path/to/BoSSS-source Release
```
  `BOSSS_INSTALL` is the native-library installation; the script argument is the
  source checkout. The script publishes the .NET runtime configuration and all
  managed dependencies into `BoSSSwrapper/rel`. It uses nethost/hostfxr and
  generated `[UnmanagedCallersOnly]` entry points. Regenerate and rebuild both
  sides when the exported BoSSS API changes.

- run the provided `src/Allwmake`-script
``` bash
cd $FOAM_DG_ROOT/src
./Allwmake
```

- try running one of the test cases, e.g. the coarse one-dimensional
  convergence test (fast, mainly serves to verify that the code executes
  without runtime errors):
``` bash
cd $FOAM_DG_ROOT/run/Tests/convAnalysis1D/
./Allrun
```
  or the full two-dimensional droplet-in-shear-flow example (used for the
  results in the accompanying publication, available for DG polynomial degree
  k=1,2,3):
``` bash
cd $FOAM_DG_ROOT/run/dropletInShearFlowBig_p1/
./Allrun
```

For direct `wmake` calls, first source `src/dotnet-host.sh`;
`Allwmake` discovers the native hosting pack automatically. Set
`DOTNET_HOST_NATIVE` if your SDK is installed in a nonstandard location.

## Selecting the BoSSS polynomial degree

Keep OpenFOAM/C++ storage at its existing p=2 setting. Each droplet case selects
its managed order-parameter degree in `bosss/config.json`, read only by BoSSS:

```json
{"DgDgrees": {"c": 3}}
```

`DgDgrees` preserves the historical spelling. The named field constructor passes
`c` into BoSSS; additional p=3 coefficients stay in the managed field, while
OpenFOAM retains its ten native slots and exchanges means. Missing files or
unmatched fields use the supplied native degree. At startup, check the line
`BoSSS field 'c': native degree 2, managed degree 3, 20 coefficients per cell.`
Copy the entire case, including its `bosss` subdirectory, for validation.
