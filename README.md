# OpenFOAM-BoSSS Bridge

This is an wrapper library to make discontinuous Galerkin (DG) 
funcionality of the BoSSS code available in OpenFOAM.
It also serves as a demonstrater of how to interface between the C++ code in OpenFAOM and the c# code of BoSSS


## Prerequisites

- modified version of `foam-extend4.1`,  see https://github.com/FDYdarmstadt/foam-extend-for-dg
- `BoSSS`, `premaster`-banch from https://github.com/FDYdarmstadt/BoSSS

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
stack (`foam-extend4.1`, `mono`, `BoSSS`) is also available; see
`Docker-Readme.md` in this repository for details. It allows reproducing the
results of the accompanying publication without a manual build.



