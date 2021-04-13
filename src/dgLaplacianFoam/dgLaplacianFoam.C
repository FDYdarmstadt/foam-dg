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

#include "dgCFD.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

    //char bossslibdir[] = "/home/flori/foam/foam-dg/src/discontinuousGalerkin/BoSSSwrapper/";
    char bossslibdir[] = "/mnt/c/Users/flori/Documents/BoSSS-bgfx/public/src/L4-application/ExternalBinding.CodeGen/bin/Release/";
    BoSSS::Globals::Init(bossslibdir);
    BoSSS::Application::ExternalBinding::Initializer MyInit;
    MyInit.BoSSSInitialize();

#   include "createTime.H"
#   include "createPolyMesh.H"
#   include "createDgMesh.H"

#   include "createFields.H"



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    Info << "\n\n Hello from Flörian" << endl;
    Info << "\nCalculating temperature distribution\n" << endl;


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
        );

        Te.solve();

        runTime.write();

#       include "infoOut.H"

    }

    return 0;
}


// ************************************************************************* //
