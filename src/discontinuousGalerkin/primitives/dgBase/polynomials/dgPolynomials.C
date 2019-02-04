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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dgPolynomials::dgPolynomials
(
    const label order
)
:
    length_(order)//dgOrder.length()),
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template<class Type>
//const field<Type>& dgPolynomials::evaluate
const scalarField dgPolynomials::evaluate
(
    const vector localCoords
) const
{
    scalar lc = localCoords.component(vector::X);

//    field<Type> value(length_, pTraits<Type>::zero);
    scalarField value(length_, 0.0);

    // First two are simple and base for the loop
    value[0] = 1;
    value[1] = lc;

//    value[0] = 1/2;
//    value[1] = lc*Foam::sqrt(3.0)/2;

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
//                Foam::sqrt(5.0)/4*(3.0*sqr(lc) - 1.0);
        }
        else
        {
            // Do nothing
        }
    }

//    Info << nl<<  "GSUM: " << gSum(value) << endl;

    return value;
}


const scalarField dgPolynomials::gradEvaluate
(
    const vector localCoords
) const
{
    scalar lc = localCoords.component(vector::X);


    scalarField value(length_, 0.0);
    scalarField grad(length_, 0.0);

    // First two are simple and base for the loop
    value[0] = 1.0;
    value[1] = lc;

    grad[0] = 0.0;
    grad[1] = 1.0;
//    grad[1] = Foam::sqrt(3.0)/2;

    // this should not overwrite 0 and 1
    forAll(value, vI)
    {
        // vI = n - 1 (n used for notation in equations), but we start from
        // third mode as first two are already defined
        label i = vI + 2;

        // Analytical form for calculating gradient of modes
        if ((vI + 2) < value.size())
        {
            // Modes
            value[i] =
                1.0/(i)*((2.0*i-1)*lc*value[i-1]);// - (i-1)*value[i-2]);
            // Gradient of modes
            grad[i] =
                1.0/(i)*((2.0*i-1)*(lc*grad[i-1] + value[i-1]) - (i-1)*grad[i-2]);
//                3.0*Foam::sqrt(5.0)/2.0*lc;
        }
        else
        {
            // Do nothing
        }
    }

//    Info << nl<<  "GSUM: " << gSum(value) << endl;

    return grad;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
