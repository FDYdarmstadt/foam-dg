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
    testDg

Description
    Basic tests with DG variables

Author
    Hrvoje Jasak.  All rights reserved.

\*---------------------------------------------------------------------------*/

#include "dgScalar.H"
#include "dgVector.H"
#include "dgCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "Contiguous scalar: " << contiguous<dgScalar>() << endl;
    Info<< "Contiguous vector: " << contiguous<dgVector>() << endl;
    
    dgScalar a(1);

    Info<< "a: " << a << endl;

    dgScalar b = a;
    
    Info<< "b = a : " << b << endl;

    dgScalar c = 3*a;
    
    Info<< "c = 3*a : " << c << endl;

    dgScalar d = c;
    d[1] = 1;
    d[2] = 2;
    d[3] = 3;
    d[4] = 4;
    d[5] = 5;
    d[6] = 6;
    d[7] = 7;
    d[8] = 8;
    d[9] = 9;

    Info<< "d = c into poly : " << d << endl;

    dgVector vec(a, a, a);

    Info<< "vec: " << vec << endl;
    
    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
