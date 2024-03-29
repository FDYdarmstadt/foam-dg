/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
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
    CahnHilliardFoam

\*---------------------------------------------------------------------------*/

#include "BoSSScpp.h"
#include "FindBoSSSLibraries.h"

#include "dgCFD.H"
#include "fvCFD.H"
#include "fvBlockMatrix.H"
#include "pimpleControl.H"

#include "GeometricField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

  Smart_Init();
  BoSSS::Application::ExternalBinding::Initializer MyInit;
  MyInit.BoSSSInitialize();

#   include "createTime.H"

    Foam::Info
        << "Create mesh for time = "
        << runTime.timeName() << Foam::nl << Foam::endl;

    Foam::fvMesh mesh
    (
        Foam::IOobject
        (
            Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            Foam::IOobject::MUST_READ
        )
    );
// #   include "createPolyMesh.H"
#   include "createDgMesh.H"
    dgMesh.finVolMesh_ = &mesh;
#   include "pimpleControl.H"

    pimpleControl pimple(*dgMesh.finVolMesh());

#   include "readGravitationalAcceleration.H"
#   include "initContinuityErrs.H"
#   include "createFields.H"
// #   include "correctPhi.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nRunning\n" << endl;
    Info << "Time = " << runTime.timeName() << nl << endl;

    while (runTime.loop())
    {
        Info << "Time = " << runTime.timeName() << nl << endl;

        // Pressure-velocity corrector
        while (pimple.loop())
        {
#           include "CEqn.H"
#           include "UEqn.H"

            // --- PISO loop
            while (pimple.correct())
            {
                #include "pEqn.H"

            //   solve(UEqn == fvc::reconstruct(
            //                     (fvc::interpolate(Psi) * fvc::snGrad(C) -
            //                      ghf * fvc::snGrad(rho) - fvc::snGrad(pd)) *
            //                     dgMesh.finVolMesh()->magSf()));

            }

#           include "continuityErrs.H"

        }

        // #include "writeEnergyAndMass.H"
        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
