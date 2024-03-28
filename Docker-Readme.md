# Readme: `foam-dg` Docker image

This file gives a short overview on the Docker image 
`floriankummer81/foamdg6:28mar24`.
The intention of this container is enable interested readers
to reproduce numerical results from the study _Coupling OpenFOAM with BoSSS, a discontinuous Galerkin solver written in C#_, by Klingenberg et. al.

The respective source code is available at:
[![DOI](https://zenodo.org/badge/223185451.svg)](https://zenodo.org/doi/10.5281/zenodo.8386723)

This study describes a multi-phase flow solver which combines 
a finite volume flow solver with a phase field model (Cahn-Hilliard) discretized by means of 
a discontinuous Galerkin (DG) method.
For the implementation of the DG-related algorithms,
`foam-dg` interfaces to the BoSSS library (https://git.rwth-aachen.de/kummer/bosss-public).

The two numerical results of the study
which are part of the Docker image and the Git repository, are
 - the simulation of a droplet in shear flow.
 - a 1D convergence study, 
   demonstrating a third order spatial convergence of the phase field solver

The reproduction of these results using the Docker container is described below.

The source-code of `foam-dg` can be found at https://github.com/FDYdarmstadt/foam-dg/.
Compiling and executing requires the careful setup of several other packages, which has proven to be somewhat error-prone in the past. Therefore, to simplify reproducing the results, we provide a "reference system" in the form of a properly set up Docker container.
In detail, the following requirements would have to be installed:
 - The discontinuous Galerkin (DG) library BoSSS (https://git.rwth-aachen.de/kummer/bosss-public)
   must be installed and set up,
   together with its on pre-conditions.
   These include Mono, Dotnet as well as the runtime library of BoSSS itself.
 - A source distribution of `foam-extend-4.1` (https://github.com/visiblehawk/foam-extend-4.1),
   against which `foam-dg` compiles

## Quick Start to the droplet-in-shear-flow example in the container

The droplet-in-shear flow represents the main example 
in the paper, c.f. Figure 4.
The full numerical solution for each timestep is written as tecplot files.
Here the plotting facilities of BoSSS are uses since these support plotting 
of the high-order DG field date with sub-cell accuracy.
The `.plt` files can be renamed into a meaningful sequence using
the python script  `rename-plots.py`.
A further important result of this computation is the deformation Parameter,
which describes the droplet deformation, details are given in the manuscript.
It is written to standard output in every timestep;
The respective lines read e.g as 
`Deformation Parameter: 0.00106771156159383`.

Using the Docker container, the results can be reproduced by the following steps:

1. Download or import the `floriankummer81/foamdg6` Docker image; From a local tar-file as:
```bash
docker load -i foamdg6.tar
```
resp., pull it from the online source:
```
docker pull floriankummer81/foamdg6:28mar24
```
2. Start the Docker container form the image
```bash
docker run -it --user 999 floriankummer81/foamdg6 /bin/bash
```
3. Enter the respective `foam-dg`-directory 
   containing the 
   droplet-in-shear-flow configuration:
```bash
cd /home/myuser/foam/foam-dg/run/dropletInShearFlowBig
```
4. Source foam-extend environment variables
```bash
fe41
```
5. Execute the the combined solver `CahnHilliardFoam`
   (Cahn-Hilliard phase field model in DG, Navier-Stokes in finite volume)
```bash
./Allclean
./Allrun
```

## 1D convergence study

The case can be run in analog fashion to the droplet-in-sher-flow described above.
The respective `Allclean` and `Allrun` scripts can be found in the directory
` /home/myuser/foam/foam-dg/run/Tests/convAnalysis1D`.

The case itself consists of three sub-cases `coarse`, `medium` and `fine`,
which are used to produce the convergence plot.
The `Allrun` script in `convAnalysis1D` finally calls the Python 3 script
`main.py`, which controlls the execution of the sub-cases and
performs the experimental convergence analysis

At the end of `Allrun`, the measured convergence rate is written out,
a value of approx 3.0 is expected (e.g 2.99003255306427).

Furthermore, the files `conv1D.png` and `convergencePlot1D.png` are produced,
which resemble Figures 2 and 3 in the manuscript, respectively.

## Compiling and Running `foam-dg`

This section gives a quick installation and compilation instruction for
`foam-dg` on Ubunutu 20.04 LTS, for which instructions for the installation of foam-extend exist.
The procedure should work similar on other Linux distributions.

1. Install OpenMPI
```bash
  apt-get update && apt-get install openmpi-common
```
2. Installation of foam-extend 4.1:
   this is not covered here, extensive resources are available on-line.
   **Note: To work correctly with BoSSS, OpenFOAM needs to be linked against the system Open MPI,
   not the Open MPI provided with OpenFOAM; Therefore, 
   one has to define `export WM_MPLIB=SYSTEMOPENMPI`
   prior to sourcing the OpenFOAM shell environment.**    
   In below, 
   we assume the foam-extend root directory to be `$HOME/foam/foam-extend-4.1`.
   For the installation in the reference Docker container, 
   the following instructions were used:
   https://openfoamwiki.net/index.php/Installation/Linux/foam-extend-4.1/Ubuntu
3. Installation of `Mono`:
```bash
   apt-get update && apt-get install mono-complete
```  
4. Installation of `dotnet`: BoSSS requires at least Dotnet 6.0.
   Installation instructions for Ubunutu 20.04 LTS, can be found at the Microsoft homepage: https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu-2004
   (Remarks: since BoSSS has been migrated to the new-style project files,
   it cannot be compiled using Mono tools. Therefore, Dotnet is required.
   However, all BoSSS libraries which are used by `foam-dg`
   are compiled for the `netstandard2.0` target, i.e., they are compatible with both,
   Dotnet and Mono.)
5. Installation of BoSSS native libraries: obtain the 
   [latest installer](https://kummer.pages.rwth-aachen.de/bosss-public/BoSSS-setup-latest.run) 
   for native libraries on Linux.
   This installer will unpack the native libraries, namely the files 
   `libBoSSSnative_seq.so`, `libBoSSSnative_omp.so` and `libBoSSSnative_mpi.so`.
   Set the `BOSSS_INSTALL` environment variable to point to the 
   directory created in the previous step.
   The files `libBoSSSnative*.so` should be in the directory `$BOSSS_INSTALL/bin/native/linux/amd64-openmpi`.
6. Optional: Compilation of BoSSS.
   We assume the public version of BoSSS to be located at `cd $HOME/BoSSS-master/public`.
   The output libraries, which `foam-dg` use, are expected to be located at
   `$FOAM_DG_ROOT/src/discontinuousGalerkin/BoSSSwrapper/rel/`.
   The best option to compile BoSSS and to deploy the BoSSS binaries in the respective location is `dotnet publish`:
```bash
   cd $HOME/BoSSS-master/public/src/L4-application/ExternalBinding
   dotnet publish ./ExternalBinding.csproj -c Release -o $FOAM_DG_ROOT/src/discontinuousGalerkin/BoSSSwrapper/rel/
```
7. Export the `FOAM_DG_ROOT`-variable, which points to the 
   root directory of the `foam-dg` repository.
```bash
   export FOAM_DG_ROOT=$HOME/foam/foam-dg
```   
8. Compile `foam-dg`:
```bash
   cd $FOAM_DG_ROOT/src
   ./Allwmake
```
9. Enter the case folder for the droplet-in-shear-flow configuration and execute the 
   `CahnHilliardFoam` solver:
```bash
   cd $FOAM_DG_ROOT/run/dropletInShearFlowBig
   CahnHilliardFoam
```

## Miscellaneous information

* The docker image itself is derived from `ubuntu:20.04`
* When executing the Docker container, the user `myuser` (`uid=999`, group `myusergroup`)
  should be used; all settings are prepared for this user.
* The `.bashrc` of `myuser` defines the BoSSS-specific 
  enviroment variables: `BOSSS_INSTALL=/root/BoSSS-install`
  and `FOAM_DG_ROOT=/root/foam/foam-dg/`
  which are required by BoSSS to find the native code libraries for BoSSS and 
  the managed libraries called from OpenFOAM.
* OpenFOAM is compiled against system Open MPI, 
  since the BoSSS native libraries (`libBoSSSnative_mpi.so`) 
  also links against the system MPI library.
  Therefore, in the `.bashrc` we define `export WM_MPLIB=SYSTEMOPENMPI`

