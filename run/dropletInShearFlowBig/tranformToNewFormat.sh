#!/usr/bin/env sh

function reorganizeZeroDir()
{
    cd 0
    cp c c.bak || echo
    cp u u.bak || echo
    cp psi psi.bak || echo
    mv C c
    mv U u
    cp fvMesh/* .
    mv fvC C
    mv fvU U
    mv fvC.orig C.orig
    mv fvU.orig U.orig
    mv Phi psi
    cp ~/Documents-work/programming/foam-dg/foam-dg/run/dropletInShearFlowBig/0/Psi .
    cd ..
}

function reorganizeSystemDir()
{
    cd system/
    sed -i 's/Cholesky/DIC/g' fvSolution
    sed -i 's/CU/cu/g' dgSchemes
    sed -i 's/fvC/C/g' sampleDict
    cp fvSolution fvMesh/
    cp fvSchemes fvMesh/
    cd ..
}

function reorganizeMainDir()
{
    sed -i 's/fvMesh\/fvC/C/g' setFields.sh
    sed -i 's/fvMesh\/fvU/U/g' setFields.sh
}

if [ -f "0/c" ]; then
    echo "Skipping reorganization of 0-directory because it was already reorganized."
else
    reorganizeZeroDir
fi


# echo "Reorganizing zero dir."
# reorganizeZeroDir
echo "Reorganizing system dir."
reorganizeSystemDir

reorganizeMainDir
