/*---------------------------------------------------------------------------*\
  =========                   |
  \\      /   F ield          | foam-extend: Open Source CFD
   \\    /    O peration      |
    \\  /     A nd            | For copyright notice see file Copyright
     \\/      M anipulation   |
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
    testFadField

Description
    Test primitive fields with Discontinuous Galerkin variables

\*---------------------------------------------------------------------------*/

#include "dgCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    // Set field given length
    dgScalarField dgf1(3);

    // Set field given length and initial value
    dgScalarField dgf2(3, dgScalar(4));

    dgf2[2][2] = 3;

//    dgScalarField dgf25(3, 4);

    Info<< "dgf2, length 3 value 4 : " << dgf2 << endl;

//    dgPolynomials poly(3);
//    Info << poly.gaussPoints() << endl;

    // Mesh needed for this
//    scalarField integrate = dgc::volumeIntegrate(dgf2);
//    Info << "Integral value: " <<  integrate << endl;

    dgVectorField tfVector
    (
        3,
        dgVector(dgScalar(1), dgScalar(2), dgScalar(3))
    );

//    Info << "tfVector: " << tfVector << endl;


    dgScalar c(0);
    dgScalar d = c;
    d[1] = 1;
    d[2] = 1;
    d[0] = 1;
//    d[4] = 4;
//    d[5] = 5;
//    d[6] = 6;
//    d[7] = 7;
//    d[8] = 8;
//    d[9] = 9;

    dgScalarField dgf3(5, d);

    Info << nl << "SCALRA D " << d << endl;
//    scalarField integrate2 = dgc::volumeIntegrate(dgf3);
//    Info << "Integral value: " <<  integrate2 << endl;

    return 0;
}


// ************************************************************************* //
