#!/bin/bash

rm constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh

cp 0/fvMesh/fvC.orig 0/fvMesh/fvC
cp 0/fvMesh/fvU.orig 0/fvMesh/fvU
# Specify the droplet center
# The example test case here is 2D; we give x- and z- coordinate of the center point:
cx=0.0
cz=0.0

# the radius of droplet
radius=0.5e-3

# the interface width (it should be the same as the one specified in constant/phaseFieldProperties)
epsilon=1e-5

# the shear rate:
# shearRate=89.235
shearRate=0.89235
# shearRate=8900.235



funkySetFields -time 0 -field fvMesh/fvC -keepPatches -expression  "tanh((-sqrt(sqr(pos().x-$cx)+sqr(pos().z-$cz))+$radius )/(sqrt(2)*$epsilon))"

funkySetFields -time 0 -field fvMesh/fvU -keepPatches -expression  "pos().z*vector($shearRate,0,0)"

funkySetFields -time 0 -field C -keepPatches -expression  "tanh((-sqrt(sqr(pos().x-$cx)+sqr(pos().z-$cz))+$radius )/(sqrt(2)*$epsilon))"

funkySetFields -time 0 -field U -keepPatches -expression  "pos().z*vector($shearRate,0,0)"
