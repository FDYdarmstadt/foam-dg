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

//#include <Python.h>

#include "BoSSScpp.h"

#include "dgCFD.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    //Py_Initialize();
    char exedir[] = "/home/flori/foam/foam-dg/src/discontinuousGalerkin/BoSSSwrapper/";
    BoSSS::Globals::Init(exedir);
    BoSSS::Application::ExternalBinding::Initializer MyInit;
    MyInit.BoSSSInitialize();


#   include "setRootCase.H"

#   include "createTime.H"
#   include "createPolyMesh.H"
#   include "createDgMesh.H"

#   include "createFields.H"

    /*
    PyObject* pName = PyUnicode_FromString("mypython");
    //printf("pSame = %i\n", pName);
	PyObject* pModule = PyImport_Import(pName);
    //printf("pModule = %i\n", pModule);

	if(pModule)
	{
		PyObject* pFunc = PyObject_GetAttrString(pModule, "TrottelFunktion");
		if(pFunc && PyCallable_Check(pFunc))
		{
			PyObject* pValue = PyObject_CallObject(pFunc, NULL);

			//printf_s("C: getInteger() = %ld\n", PyLong_AsLong(pValue));
		}
		else
		{
			printf("ERROR: function getInteger()\n");
		}

	}
	else
	{
		printf("ERROR: Module not imported\n");
	}
    */

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    Info << "\n\n Hello from Flörian" << endl;
    Info << "\nCalculating temperature distribution\n" << endl;


    while(runTime < runTime.endTime())
    {
    runTime++;


    Info << "Time: " << runTime.timeName() << nl << endl;

    // Testing matrix operations
        dgScalarMatrix TEqn(T, T.dimensions()/dimTime);

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
