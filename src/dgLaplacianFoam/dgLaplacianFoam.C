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
    dgLaplacianFoam

Description
    Solves a Laplace equation using the Discontinuous Galerkin Method.
    Uses the Implementation in the BoSSS library.

Authors:
    Hrvoje Jasak.  All rights reserved.
    Florian Kummer. 

\*---------------------------------------------------------------------------*/

#include "BoSSScpp.h"
#include "FindBoSSSLibraries.h"

#include "dgCFD.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

    Smart_Init();
    BoSSS::Application::ExternalBinding::Initializer MyInit;
    MyInit.BoSSSInitialize();

#   include "createTime.H"
#   include "createPolyMesh.H"
#   include "createDgMesh.H"

#   include "createFields.H"



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    Info << "\nCalculating temperature distribution\n" << endl;

    // mono_profiler_install_gc_moves();

    const Foam::dgMesh& meeehsch = T.mesh();//.GetBoSSSobject();
    BoSSS::Foundation::Grid::OpenFOAMGrid* bosssMesh = meeehsch.GetBoSSSobject();
    bosssMesh->TestMethod(44);

    T.GetBoSSSobject();

    T.SyncToBoSSS();
    // T.dgMesh()

    // dgScalar scl = T[0];
    // Info << scl << endl;
    Info << "boundary: " << meeehsch.boundary()[1].type() << nl << endl;
    Info << "boundary: " << meeehsch.boundary()[2].type() << nl << endl;

    Info << "patch: " << T.boundaryField() << nl << endl;
    Info << "patchtype: " << T.boundaryField()[0].type() << nl << endl;
    Info << "patchtype: " << T.boundaryField()[1].type() << nl << endl;
    Info << "patchtype: " << T.boundaryField()[2].type() << nl << endl;

    // T.boundaryField()[0].writeEntry("value", Info);
    // (T.boundaryField()[0])[0];

    Info << T.boundaryField()[0][0] << nl << endl;
    Info << (*(T.boundaryField()[0].dictionaryConstructorTablePtr_)).found("boundaryField") << nl << endl;
    T.boundaryField()[0].dictionaryConstructorTablePtr_->printInfo(Info);
    // T.boundaryField()[0].
    // Info << "patch value: " << T.boundaryField()[0].patch().name() << nl << endl;

    // auto ioobj = IOobject("T", runTime.timeName(), mesh, IOobject::MUST_READ,
    //                     IOobject::AUTO_WRITE);
    // auto dict = Foam::dictionary(ioobj);
    // Info << dict.subDict["boundaryField"];

    // Info << "patchtype: " << typeid(T.boundaryField()[0]).name() << nl << endl;
    // Info << "patchtype: " << T.boundaryField()[0].value << nl << endl;
    while(runTime < runTime.endTime())
    {
        runTime++;

        Info << "Time: " << runTime.timeName() << nl << endl;

        // Testing matrix operations
        //dgScalarMatrix TEqn(T, T.dimensions()/dimTime);

        cellScalarField Tintegral = dgc::volumeIntegrate(T);

        cellScalarField T1 = dgc::dgLaplacian(T);

        dgScalarMatrix Te
        (
            dgm::dgLaplacian(T)
        )
        ;

        // Info << "Hello again from dgLaplacianFoam" << endl;
        // BoSSS::Application::ExternalBinding::OpenFoamMatrix *bo = Te.GetBoSSSobject();
        // if (bo == NULL) {
        //   Info << "bo == NULL" << endl;
        // } else {
        //   Info << "bo != NULL" << endl;
        //   double InputReadBuffer[100];
        //   bo->GetBlock(0, 0, InputReadBuffer);
        //   Info << "InputReadBuffer: " << endl;
        //   for (int i = 0; i < 100; i++) {
        //   Info << InputReadBuffer[i] << " ";
        // }
        // Info << endl;
        // }
        // Info << Te << endl;
        // T.SyncFromBoSSS();

        Te.solveBoSSS();
        // Te.solve();

        T.SyncFromBoSSS();
        Te.AllowGC();
        runTime.write();

#       include "infoOut.H"

    }

    return 0;
}


// ************************************************************************* //
