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

#include "dgPolynomials.H"
#include "dgOrder.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dgPolynomials::dgPolynomials
(
    const label
//    const dgOrder& dgOrder
)
:
    length_(dgOrder::length),//dgOrder.length()),
    gaussWeights_(3, 0.0),
    gaussPoints_(3, 0.0)
{
//    forAll(gaussWeights, gwI)
//    {
//        // It would be great to have an expression here
//        gaussWeights[gwI] =
//    }

// HARD-CODED to only 3 points for now
    gaussWeights_[0] = 5.0/9.0;
    gaussWeights_[1] = 8.0/9.0;
    gaussWeights_[2] = 5.0/9.0;

    gaussPoints_[0] = - sqrt(3.0/5.0);
    gaussPoints_[1] = 0;
    gaussPoints_[2] = sqrt(3.0/5.0);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template<class Type>
//const field<Type>& dgPolynomials::evaluate
scalarField dgPolynomials::evaluate
(
    const vector localCoords
)
{
    scalar lc = localCoords.component(vector::X);

//    field<Type> value(length_, pTraits<Type>::zero);
    scalarField value(length_, 0.0);

    // First two are simple and base for the loop
//    value[0] = pTraits<Type>::one;
    value[0] = 1;
    value[1] = lc;

    // this should not overwrite 0 and 1
    forAll(value, vI)
    {
        label i = vI + 2;

        if ((vI + 2) < value.size())
        {
            // Analytical form for calculating modes
//            value[vI+1] =
//                1.0/(vI + 1.0)*((2*vI+1)*lc*value[vI] - vI*value[vI-1]);


            value[i] =
                1.0/(i)*((2*i-1)*lc*value[i-1] - (i-1)*value[i-2]);

//            Info << "NODE " << vI << ": " << value[vI]
//                 << " for lc: " << lc << endl;
        }
        else
        {
            // Do nothing
        }
    }

    Info << nl<<  "GSUM: " << gSum(value) << endl;

    return value;
}


void dgPolynomials::test()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
