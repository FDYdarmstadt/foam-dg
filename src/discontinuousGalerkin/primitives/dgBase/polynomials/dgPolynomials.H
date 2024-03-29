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
    Foam::dgPolynomials

Description
    Definition of a Discontinuous Galerkin Polynomials

Author
    Gregor Cvijetic.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef dgPolynomials_H
#define dgPolynomials_H

#include "dgOrder.H"
#include "primitiveFields.H"
#include "dgScalar.H"
#include "dgVector.H"
//#include "IOdictionary.H"
//#include "polyMesh.H"
//#include "dgMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                          Class dgPolynomials Declaration
\*---------------------------------------------------------------------------*/

class dgPolynomials
{
    // Private data

//    const polyMesh& polyMesh_;

    //- Number of modes taken into account
    const label length_;


public:

    // Constructors

        //- Construct from dgOrder
        dgPolynomials
        (
            const label order
        );


    // Member functions

        //- Return vector of modes evaluated in local coordinates
        scalarField evaluate
        (
            const vector localCoords
        ) const;

        scalar evaluate
        (
            const dgScalar,
            const vector localCoords
        ) const;

        vector evaluate
        (
            const dgVector,
            const vector localCoords
        ) const;

        //- Return vector of gradient of modes evaluated in local coordinates
        scalarField gradEvaluate
        (
            const vector localCoords
        ) const;


    // Access functions

        label size() const
        {
            return length_;
        }

        label order() const
        {
            return length_;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
