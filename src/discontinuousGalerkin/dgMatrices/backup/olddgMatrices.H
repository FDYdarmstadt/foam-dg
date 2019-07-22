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

Class
    faMatrix

Description
    A special block matrix type for the Discontinuous Galerkin method
    of discretisation

\*---------------------------------------------------------------------------*/

#ifndef dgMatrices_H
#define dgMatrices_H

#include "dgFieldTypes.H"
#include "VectorN.H"
#include "TensorN.H"
#include "dgMatrix.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


--> TREBAO BIH UBACITI BASE CLASS IZMEDU BLOCKLDU I DGMATRIX KOJA RADI ADD TO
SELECTION TABLE

typedef dgMatrix<dgScalar, 0> dgZeroScalarMatrix;
typedef dgMatrix<dgScalar, 1> dgOneScalarMatrix;
typedef dgMatrix<dgScalar, 2> dgTwoScalarMatrix;
typedef dgMatrix<dgScalar, 3> dgThreeScalarMatrix;
typedef dgMatrix<dgScalar, 4> dgFourScalarMatrix;
//typedef dgMatrix<dgVector> dgVectorMatrix;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
