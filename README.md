# OpenFOAM-BoSSS Bridge

This is an wrapper library to make discontinuous Galerkin (DG) 
funcionality of the BoSSS code available in OpenFOAM.
It also serves as a demonstrater of how to interface between the C++ code in OpenFAOM and the c# code of BoSSS


## Prerequisites

- modified version of `foam-extend4.1`,  see https://github.com/FDYdarmstadt/foam-extend-for-dg
- A current .NET 10 BoSSS checkout from https://github.com/FDYdarmstadt/BoSSS
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

A pre-built Docker image with a working installation of the full software
stack from the Mono-based paper release is also available; see
`Docker-Readme.md` in this repository for details. It allows reproducing the
results of the accompanying publication without a manual build.




The historical Docker tags described there use the previous Mono bridge and do
not contain this .NET 10 migration. Building the current source requires the
steps above. For direct `wmake` calls, first source `src/dotnet-host.sh`;
`Allwmake` discovers the native hosting pack automatically. Set
`DOTNET_HOST_NATIVE` if your SDK is installed in a nonstandard location.
