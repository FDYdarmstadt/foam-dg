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
    Foam::dgOrder

Description
    Definition of a Discontinuous Galerkin Order

Author
    Hrvoje Jasak.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef dgOrder_H
#define dgOrder_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                          Class dgOrder Declaration
\*---------------------------------------------------------------------------*/

class dgOrder
{
public:

    // Member constants

    enum
    {
        dim = 3,         // Dimensionality of space
        rank = 0,        // Rank of Scalar is 0
        nComponents = 1, // Number of components in Scalar
        order = 2,       // DG order.  Order of discretisation = order + 1
        // length = order + 1
        length = (order + 3)*(order + 2)*(order + 1)/6  // Coeffs
        // length = 10
    };
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
