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
#include "VectorN.H"
#include "tensor.H"
// #include "Field.H"
// #include "CoeffField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class DgScalar Declaration
\*---------------------------------------------------------------------------*/

template <class Cmpt, int length>
inline void expandScalar(VectorN<Cmpt, length>& result, const Cmpt& v)
{
    result = Vector<Cmpt>(v, v, v);
}

template <class Cmpt, int length>
inline void expandScalar(TensorN<Cmpt, length>& result, const Cmpt& v)
{
    result = Tensor<Cmpt>
    (
        v,                    pTraits<Cmpt>::zero,     pTraits<Cmpt>::zero,
        pTraits<Cmpt>::zero,  v,                       pTraits<Cmpt>::zero,
        pTraits<Cmpt>::zero,  pTraits<Cmpt>::zero,     v
    );
}


template<class Cmpt, int length>
class DgScalar
:
   public UList<Cmpt>
   // public VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>
    // public VectorN<Cmpt, length>
{

//        scalarField gaussPoints_;
//
//        scalarField gaussWeights_;

public:

    // Member constants

        enum
        {
            rank = 0, // Rank of DgScalar is 0
            coeffLength = length, // Number of components in this DgScalar
        };


    // Static data members

        // static string cmptType;
        // static string cmptType = typeid(Cmpt).name();
        static const char* const typeName;
        static const char* componentNames[];
        static const DgScalar zero;
        static const DgScalar one;
        static const DgScalar max;
        static const DgScalar min;
        // static const int nComponents = coeffLength;
        // static const DgScalar v_;


    // Constructors

        //- Construct null
        inline DgScalar();

        //- Construct given VectorN of the same rank
        inline DgScalar
        (
            const UList<Cmpt>&
           // const VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>&
            // const VectorN<Cmpt, length>&
        );

        //- Construct given component value.  Special use only!
        explicit inline DgScalar(const Cmpt& vx);

        //- Construct from Istream
        inline DgScalar(Istream&);


    // Member Functions

        // bool operator==(DgScalar s2);
        // bool operator!=(DgScalar s2);
        // inline const Cmpt& operator()(int i);
        // inline Cmpt& operator[](int i) const;
        // bool operator<<(DgScalar s1);
        //    scalarField gaussPoints();
        //
        //    scalarField gaussWeights();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DgScalarTemplateI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
