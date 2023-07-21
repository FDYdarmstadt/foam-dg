#!/bin/bash

rm -rf constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh

# cp 0/fvMesh/fvC.orig 0/fvMesh/fvC
# cp 0/fvMesh/fvU.orig 0/fvMesh/fvU

cp 0/C.orig 0/C
cp 0/U.orig 0/U
# Specify the droplet center
# The example test case here is 2D; we give x- and z- coordinate of the center point:
cx=1e-3
cz=0.0

# the radius of droplet
radius=0.5

# the interface width (it should be the same as the one specified in constant/phaseFieldProperties)
# epsilon=1e-5

# the shear rate:
shearRate=0.01



funkySetFields -time 0 -field C -keepPatches -expression  "tanh((-sqrt(sqr(pos().x-$cx)+sqr(pos().z-$cz))+$radius )*(sqrt(2)*20))"

funkySetFields -time 0 -field U -keepPatches -expression  "pos().z*vector($shearRate,0,0)"
# funkySetFields -time 0 -field U -valuePatches "top bottom left right frontAndBack" -expression  "pos().z*vector($shearRate,0,0)"
# funkySetFields -time 0 -field U -keepPatches -expression  "pos().z*vector(1,0,0)"
# funkySetFields -time 0 -field U -keepPatches -expression  "vector(1,0,0)"
# funkySetFields -time 0 -field U -keepPatches -expression  "pos().z*vector(1,0,0)"
