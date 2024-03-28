#!/bin/bash

rm -rf constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh

cp 0/fvMesh/fvC.orig 0/fvMesh/fvC
cp 0/fvMesh/fvU.orig 0/fvMesh/fvU
cp 0/fvMesh/fvC.orig 0/C
cp 0/fvMesh/fvU.orig 0/U

funkySetFields -time 0 -field fvMesh/fvC -keepPatches -expression  "sign(pos().x)"
funkySetFields -time 0 -field C -keepPatches -expression  "sign(pos().x)"


funkySetFields -time 0 -field fvMesh/fvU -keepPatches -expression  "vector(0,0,0)"
funkySetFields -time 0 -field U -keepPatches -expression  "vector(0,0,0)"
