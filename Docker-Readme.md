# Readme: `foam-dg` Docker image

This file gives a short overview on the Docker image 
`floriankummer81/foamdg5`.
The intention of this container is enable interested readers
to reproduce numerical results from the study _Coupling OpenFOAM with BoSSS, a discontinuous Galerkin solver written in C#_, by Klingenberg et. al.

This study describes a multi-phase flow solver which combines 
a finite volume flow solver with a phase field model discretized by means of 
a discontinuous Galerkin (DG) method.
For the implementation of the DG-related algorithms,
`foam-dg` interfaces to the BoSSS library (https://git.rwth-aachen.de/kummer/bosss-public).


The source-code of `foam-dg` can be found at https://github.com/FDYdarmstadt/foam-dg/.
Compiling and executing requires the careful setup of several other packages, which has proven to be somewhat error-prone in the past. Therefore, to simplify reproducing the results, we provide a "reference system" in the form of a properly set up Docker container.
In detail, the following requirements would have to be installed:
 - The discontinuous Galerkin (DG) library BoSSS (https://git.rwth-aachen.de/kummer/bosss-public)
   must be installed and set up,
   together with its on pre-conditions.
   These include Mono, Dotnet as well as the runtime library of BoSSS itself.
 - A source distribution of `foam-extend-4.1` (https://github.com/visiblehawk/foam-extend-4.1),
   against which `foam-dg` compiles

## Quick Start to run the Docker container

1. Download or import the `floriankummer81/foamdg5` Docker image; From a local tar-file as:
```bash
docker load -i foamdg5.tar
```
resp., pull it from the online source:
```
docker pull floriankummer81/foamdg5:19feb24
```
2. Start the Docker container form the image
```bash
docker run -it floriankummer81/foamdg5 /bin/bash
```
3. Enter the respective `foam-dg`-directory 
   containing the 
   droplet-in-shear-flow configuration:
```bash
cd /root/foam/foam-dg/run/dropletInShearFlowBig
```
4. Source foam-extend environment variables
```bash
fe41
```
5. Execute the the the combined solver 
   (Cahn-Hilliard phase field model in DG, Navier-Stokes in finite volume)
```bash
CahnHilliardFoam
```

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

