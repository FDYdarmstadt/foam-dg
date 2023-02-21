#!/bin/bash

rm -rf constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh

cp 0/fvMesh/fvC.orig 0/fvMesh/fvC
cp 0/fvMesh/fvU.orig 0/fvMesh/fvU
# Specify the droplet center
# The example test case here is 2D; we give x- and z- coordinate of the center point:
cx=1e-3
cz=0.0

# the radius of droplet
radius=0.5

# the interface width (it should be the same as the one specified in constant/phaseFieldProperties)
# epsilon=1e-5

# the shear rate:
shearRate=8.9235



funkySetFields -time 0 -field fvMesh/fvC -keepPatches -expression  "tanh((-sqrt(sqr(pos().x-$cx)+sqr(pos().z-$cz))+$radius )*(sqrt(2)))"


funkySetFields -time 0 -field fvMesh/fvU -keepPatches -expression  "pos().z*vector($shearRate,0,0)"
