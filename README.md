# OpenFOAM-BoSSS Bridge

## Prerequisites

- `foam-extend4.1`
- `mono`
- optional: `BoSSS`, which is only required you would like to develop on the `BoSSS`-side

## Running instructions

To use this code, follow these instructions:

- clone this repository:
```
mkdir ~/foam/
git clone https://git.rwth-aachen.de/bosss1/foam-dg.git ~/foam/foam-dg/
```

- source `foam-extend`'s `bashrc`, i.e. something like (`foam-extend` users may already have defined a bash alias for this)
```
source ~/foam/foam-extend-4.1/etc/bashrc
```

- export the `$FOAM_DG_ROOT`-variable (you may also want to put this in your `~/.bashrc` or wherever you set your bash variables):
```
export FOAM_DG_ROOT=~/foam/foam-dg/ # adjust according to where you cloned this repository
```

- run the provided `src/Allwmake`-script
```
cd $FOAM_DG_ROOT/src
./Allwmake
```

- try running one of the test cases, e.g.
```
cd $FOAM_DG_ROOT/run/Tests/dropletInShearFlowSmall/
CahnHilliardFoam
```
This test case is very coarse and does not provide any interesting physics. It only serves to verify that the code can be executed without runtime errors. Nevertheless, it can serve as a template for more interesting test cases.
