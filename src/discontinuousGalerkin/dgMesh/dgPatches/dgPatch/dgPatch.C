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

\*---------------------------------------------------------------------------*/

#include "dgPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "dgBoundaryMesh.H"
#include "dgMesh.H"
#include "mapPolyMesh.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(dgPatch, 0);
    defineRunTimeSelectionTable(dgPatch, polyPatch);
    addToRunTimeSelectionTable(dgPatch, dgPatch, polyPatch);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dgPatch::dgPatch(const polyPatch& p, const dgBoundaryMesh& bm)
:
    polyPatch_(p),
    boundaryMesh_(bm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::dgPatch::~dgPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::unallocLabelList& Foam::dgPatch::faceCells() const
{
    return polyPatch_.faceCells();
}


const Foam::vectorField& Foam::dgPatch::faceCentres() const
{
    return boundaryMesh().mesh().mesh().faceCentres();
}


const Foam::vectorField& Foam::dgPatch::faceAreas() const
{
    return boundaryMesh().mesh().mesh().faceAreas();
}


void Foam::dgPatch::movePoints()
{}


void Foam::dgPatch::updateMesh(const mapPolyMesh&)
{}


// ************************************************************************* //
