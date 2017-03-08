/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.0
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Description

\*---------------------------------------------------------------------------*/

#include "dgBoundaryMesh.H"
#include "dgMesh.H"
#include "primitiveMesh.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(dgBoundaryMesh, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::dgBoundaryMesh::addPatches(const polyBoundaryMesh& basicBdry)
{
    setSize(basicBdry.size());

    // Set boundary patches
    dgPatchList& Patches = *this;

    forAll(Patches, patchI)
    {
        Patches.set(patchI, dgPatch::New(basicBdry[patchI], *this));
    }
}




// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from polyBoundaryMesh
Foam::dgBoundaryMesh::dgBoundaryMesh
(
    const dgMesh& mesh,
    const polyBoundaryMesh& basicBdry
)
:
    dgPatchList(basicBdry.size()),
    mesh_(mesh)
{
    addPatches(basicBdry);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::dgMesh& Foam::dgBoundaryMesh::mesh() const
{
    return mesh_;
}


Foam::lduInterfacePtrsList Foam::dgBoundaryMesh::interfaces() const
{
    lduInterfacePtrsList interfaces(size());

    forAll (interfaces, patchI)
    {
        if (isA<lduInterface>(this->operator[](patchI)))
        {
            interfaces.set
            (
                patchI,
               &refCast<const lduInterface>(this->operator[](patchI))
            );
        }
    }

    return interfaces;
}


void Foam::dgBoundaryMesh::movePoints()
{
    dgPatchList& patches = *this;

    forAll (patches, patchI)
    {
        patches[patchI].movePoints();
    }
}


void Foam::dgBoundaryMesh::updateMesh(const mapPolyMesh& mpm)
{
    dgPatchList& patches = *this;

    forAll(patches, patchI)
    {
        patches[patchI].updateMesh(mpm);
    }
}


// ************************************************************************* //
