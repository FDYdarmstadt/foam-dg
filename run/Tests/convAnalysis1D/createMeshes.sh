#!/usr/bin/env sh

cd coarse
rm -r constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh
cd ..

cd medium
rm -r constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh
cd ..

cd fine
rm -r constant/polyMesh/*
cp blockMeshDict constant/polyMesh/
blockMesh
cd ..

