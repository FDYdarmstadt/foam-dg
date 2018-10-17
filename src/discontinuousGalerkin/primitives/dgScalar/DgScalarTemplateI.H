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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template <class Cmpt, int length>
const char* const DgScalar<Cmpt, length>::typeName =
    "dgScalar";

template <class Cmpt, int length>
const DgScalar<Cmpt, length> DgScalar<Cmpt, length>::zero(0);

template <class Cmpt, int length>
const DgScalar<Cmpt, length> DgScalar<Cmpt, length>::one(1);

template <class Cmpt, int length>
const DgScalar<Cmpt, length> DgScalar<Cmpt, length>::max(VGREAT);

template <class Cmpt, int length>
const DgScalar<Cmpt, length> DgScalar<Cmpt, length>::min(-VGREAT);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
template <class Cmpt, int length>
inline DgScalar<Cmpt, length>::DgScalar()
{}


// Construct given VectorSpace
template <class Cmpt, int length>
inline DgScalar<Cmpt, length>::DgScalar
(
//    const VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>& vs
    const VectorN<Cmpt, length>& vs
)
:
//    VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>(vs)
    VectorN<Cmpt, length>(vs)
{}


// Construct given Cmpts
template <class Cmpt, int length>
inline DgScalar<Cmpt, length>::DgScalar(const Cmpt& vx)
{
    // Set all components to zero
    VectorSpaceOps<DgScalar<Cmpt, length>::nComponents, 0>::eqOpS
        (*this, pTraits<Cmpt>::zero, eqOp<Cmpt>());

    // Set the zeroth (mean) component to a given value
    this->operator[](0) = vx;
}


// Construct from Istream
template <class Cmpt, int length>
inline DgScalar<Cmpt, length>::DgScalar(Istream& is)
:
//    VectorSpace<DgScalar<Cmpt, length>, Cmpt, length>(is)
    VectorN<Cmpt, length>(is)
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Cmpt, int length>
class outerProduct<Cmpt, DgScalar<Cmpt, length> >
{
public:

    typedef DgScalar<Cmpt, length> type;
};


template<class Cmpt, int length>
class outerProduct<DgScalar<Cmpt, length>, Cmpt>
{
public:

    typedef DgScalar<Cmpt, length> type;
};


template<class Cmpt, int length>
class outerProduct<DgScalar<Cmpt, length>, DgScalar<Cmpt, length> >
{
public:

    typedef DgScalar<Cmpt, length> type;
};


//- Componentwise division of scalar by DgScalar
template<class Cmpt, int length>
inline DgScalar<Cmpt, length>
operator/(const scalar s, const DgScalar<Cmpt, length>& v)
{
    DgScalar<Cmpt, length> res;
    VectorSpaceOps<DgScalar<Cmpt, length>::nComponents,0>::opSV
        (res, s, v, divideOp3<Cmpt, scalar, Cmpt>());
    return res;
}


//- Multiply components of DgScalar by DgScalar
template<class Cmpt, int length>
inline DgScalar<Cmpt, length>
cmptMultiply(const DgScalar<Cmpt, length>& v1, const DgScalar<Cmpt, length>& v2)
{
    DgScalar<Cmpt, length> res;
    VectorSpaceOps<DgScalar<Cmpt, length>::nComponents,0>::op
        (res, v1, v2, multiplyOp<Cmpt>());
    return res;
}


//- Multiply row of DgScalar with a scalar
template<class Cmpt, int length>
inline DgScalar<Cmpt, length>
scaleRow(const DgScalar<Cmpt, length>& v, const Cmpt& c)
{
    return v*c;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
