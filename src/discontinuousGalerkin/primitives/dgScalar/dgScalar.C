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

#include "dgScalar.H"
//#include "dgPolynomials.H"
//#include "dgMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<>
//scalar dgScalar::valueLocal
//(
//    const vector& localCoordinate
//)
//{
//    scalar value = 0;
//    // Scalar list or field?
////    scalarField polyEval = dgPolynomials.evaluate(localcoords);
//
//    forAll (*this, I)
//    {
//        // Not sure about this code design
////        value += polyEval[I]*(*this)[I];
//    }
//
//    return value;
//}
//
//template<>
//scalar dgScalar::valueGlobal
//(
//    const vector& globalCoordinate,
//    const label& cellIndex
//)
//{
//    scalar value = 0;
////    vector localCoords = dgMesh.globalToLocal(globalCoordinate, cellIndex);
//
////    scalar value = dgPolynomials.evaluate(localCoords) & *this;
//
//    return value;
//}


// Access

//scalarList dgScalar::gaussWeights()
//{
//    dgPolynomials polynomial();
//
//    return polynomial().gaussWeights();
//}
//
//scalarList dgScalar::gaussPoints()
//{
//    dgPolynomials polynomial();
//
//    return polynomial().gaussPoints();
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// ************************************************************************* //
