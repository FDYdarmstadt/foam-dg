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

Application
    dgaplacianFoam

Description
    Solves a Laplace equation using the Discontinuous Galerkin Method

Author
    Hrvoje Jasak.  All rights reserved.

\*---------------------------------------------------------------------------*/

#include "dgCFD.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"

#   include "createTime.H"
#   include "createPolyMesh.H"
#   include "createDgMesh.H"
#   include "createFvMesh.H"

#   include "createFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    const dgBase& polynomials = dgMesh.polynomials();

    forAll (fvMesh.C(), cellI)
    {
//        vector coord = fvMesh.C()[cellI];
        vector coord (0, 0, 0);

        scalarField polyEval = polynomials.evaluate(coord);

        forAll (polyEval, modI)
        {
            Tvol[cellI] +=
            T[cellI][modI]*polyEval[modI];//*dgMesh.cellScaleCoeffs()[cellI];

//            Info<< "Cell: " << cellI << ", modI: " << modI << ", polyEval: "
//                << polyEval[modI] << ", T:" << T[cellI][modI]
//                << ", calculated: " << Tvol[cellI] << endl;
        }
    }

    // SAMO U OVAJ DG-based solver ubacim da mogu raditi volScalarField i
    // evaluiram u cell centreima

    Tvol.write();

    Info<< "Tvol: " << Tvol.internalField() << nl << endl;

    return 0;
}


// ************************************************************************* //
