#!/bin/bash

rm -rf constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh

cp 0/C.orig 0/C
cp 0/U.orig 0/U

# Specify the droplet center
# The example test case here is 2D; we give x- and z- coordinate of the center point:
cx=0.0
cz=0.0

# the radius of droplet
radius=0.5


# the shear rate:
shearRate=0.01



funkySetFields -time 0 -field C -keepPatches -expression  "tanh((-sqrt(sqr(pos().x-$cx)+sqr(pos().z-$cz))+$radius )*(sqrt(2)*20))"

funkySetFields -time 0 -field U -keepPatches -expression  "pos().z*vector($shearRate,0,0)"
