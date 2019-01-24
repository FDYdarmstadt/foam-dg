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

#   include "createFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nCalculating temperature distribution\n" << endl;


    while(runTime < runTime.endTime())
    {
    runTime++;

    Info<< "-----------Time = " << runTime.timeName() << nl << endl;

    // Testing field I/O
//    T.write();

    // Testing matrix operations
    dgScalarMatrix TEqn(T, T.dimensions()/dimTime);

    cellScalarField Tintegral = dgc::volumeIntegrate(T);

//Info << "THREE" << Tintegral.boundaryField() << nl
//     << Tintegral.internalField() << endl;


//    forAll (Tintegral.boundaryField(), patchI)
//    {
//        Info << nl << "PATCH: " << Tintegral.boundaryField()[patchI] << endl;
//
//    }

    cellScalarField T1 = dgc::dgLaplacian(T);

//    dgScalarMatrix Te = dgm::dgLaplacian(T);

    dgScalarMatrix Te
    (
        dgm::dgLaplacian(T)
    );

//    Info << " DIAG IS : " << Te.diag() << endl;

//    Info<< "------- Timing = " << runTime.timeName() << nl << endl;
    Te.solve();

//         Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
//             << "  ClockTime = " << runTime.elapsedClockTime() << " s"
//             << nl << endl;
//     }
//    T.write();
    runTime.write();

//    Info << T << endl;
//
//    Info<< "End\n" << endl;

#       include "infoOut.H"

    }

    return 0;
}


// ************************************************************************* //
