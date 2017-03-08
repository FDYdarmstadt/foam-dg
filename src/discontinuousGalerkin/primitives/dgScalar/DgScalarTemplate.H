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
    DgScalar

Description
    Templated Discontinuous Galerkin form of a scalar of given length

SourceFiles
    DgScalarI.H

\*---------------------------------------------------------------------------*/

#ifndef DgScalar_H
#define DgScalar_H

#include "VectorSpace.H"
#include "tensor.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class DgScalar Declaration
\*---------------------------------------------------------------------------*/

template<class Cmpt, int length>
class DgScalar
:
    public VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>
{

public:

    // Member constants

        enum
        {
            rank = 0, // Rank of DgScalar is 0
            coeffLength = length // Number of components in this DgScalar
        };


    // Static data members

        static const char* const typeName;
        static const char* componentNames[];
        static const DgScalar zero;
        static const DgScalar one;
        static const DgScalar max;
        static const DgScalar min;


    // Constructors

        //- Construct null
        inline DgScalar();

        //- Construct given VectorSpace of the same rank
        inline DgScalar
        (
            const VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>&
        );

        //- Construct given component value.  Special use only!
        explicit inline DgScalar(const Cmpt& vx);

        //- Construct from Istream
        inline DgScalar(Istream&);


    // Member Functions
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DgScalarTemplateI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
