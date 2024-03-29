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
    Foam::dgScalar

Description
    Discontinuous Galerkin form of a scalar.

Author
    Hrvoje Jasak.  All rights reserved.

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef dgScalar_H
#define dgScalar_H

#include "scalar.H"
#include "DgScalarTemplate.H"
#include "dgOrder.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    typedef DgScalar<scalar, dgOrder::length> dgScalar;
//    typedef VectorN<scalar, dgOrder::length> dgScalar;


// template<>
// pTraits<dgScalar>::pTraits(const dgScalar& p) {}

// template<>
// int pTraits<dgScalar>::cmptType = 0;
//- Specify data associated with dgScalar type is contiguous
template<>
inline bool contiguous<dgScalar>() {return true;}

//template<>
//scalar valueLocal
//(
//    const vector& localCoordinate
//);
//
//template<>
//scalar valueGlobal
//(
//    const vector& globalCoordinate,
//    const label& cellIndex
//);



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
