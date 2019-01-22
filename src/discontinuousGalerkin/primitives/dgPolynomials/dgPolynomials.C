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

void dgPolynomials::calcGaussPtsEval() const
{
    // Local coordinates (reference element)
    const scalarField& gaussCoords = gaussPoints();
//    const scalarField& gaussWeights = this->gaussWeights();

    // Evaluate in -1, 1 and gauss points
    gaussPtsEvalPtr_ = new PtrList<scalarField>(gaussCoords.size() + 2);

    // Go over all Gauss points for the whole mesh and add value*weight
    forAll(*gaussPtsEvalPtr_, ptI)
    {
        scalarField polyEval(gaussCoords.size(), 0.0);

        // Evaluate in -1
        if (ptI == 0)
        {
            polyEval = evaluate(vector(-1,0,0));
        }
        // Evaluate in 1
        else if (ptI == (gaussPtsEvalPtr_->size() - 1))
        {
            polyEval = evaluate(vector(1,0,0));
        }
        // Evaluate in Gauss points
        else
        {
            // Calculate modal values in given coordinate (Gaussian point
            // coordinate)
            polyEval = evaluate(vector(gaussCoords[ptI - 1],0,0));

//            forAll (polyEval, modI)
//            {
//                polyEval[modI] *= gaussWeights[ptI];
//            }
        }

        gaussPtsEvalPtr_->set
        (
            ptI,
            new scalarField (polyEval)
        );
    }
}


void dgPolynomials::calcGaussPtsGradEval() const
{
    // Local coordinates (reference element)
    const scalarField& gaussCoords = gaussPoints();
//    const scalarField& gaussWeights = this->gaussWeights();

    gaussPtsGradEvalPtr_ = new PtrList<scalarField>(gaussCoords.size() + 2);
    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(*gaussPtsGradEvalPtr_, ptI)
    {
        // This is of size dgScalar.size()
        scalarField polyEval(gaussCoords.size(), 0.0);

        // Evaluate in -1
        if (ptI == 0)
        {
            polyEval = gradEvaluate(vector(-1,0,0));
        }
        // Evaluate in 1
        else if (ptI == (gaussPtsGradEvalPtr_->size() - 1))
        {
            polyEval = gradEvaluate(vector(1,0,0));
        }
        // Evaluate in Gauss points
        else
        {
            // Calculate modal values in given coordinate (Gaussian point
            // coordinate)
            polyEval = gradEvaluate(vector(gaussCoords[ptI - 1],0,0));

//            forAll (polyEval, modI)
//            {
//                polyEval[modI] *= gaussWeights[ptI];
//            }
        }

        gaussPtsGradEvalPtr_->set
        (
            ptI,
            new scalarField (polyEval)
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dgPolynomials::dgPolynomials
(
//    const label
//    const dgOrder& dgOrder
)
:
    length_(dgOrder::length),//dgOrder.length()),
    gaussWeights_(3, 0.0),
    gaussPoints_(3, 0.0),
    gaussPtsEvalPtr_(NULL),//gaussPoints_.size()),
    gaussPtsGradEvalPtr_(NULL)//gaussPoints_.size())
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


PtrList<scalarField> dgPolynomials::wtdGaussEval()
{
    if (!gaussPtsEvalPtr_)
    {
        calcGaussPtsEval();
    }

    const scalarField& gaussWeights = this->gaussWeights();

    const PtrList<scalarField>& gaussPts = *gaussPtsEvalPtr_;
    PtrList<scalarField> wtdGaussPts = gaussPts;

    // Go over all Gauss points for the whole mesh and add value*weight
    forAll(gaussPts, ptI)
    {
        if (ptI < gaussPts.size() - 2)
        {
            // Zeroth entry is for coordinate -1
            scalarField& wtdGPI = wtdGaussPts[ptI + 1];

            forAll (wtdGPI, modI)
            {
                wtdGPI[modI] *= gaussWeights[ptI];
            }
        }
    }

    return wtdGaussPts;
}


PtrList<scalarField> dgPolynomials::wtdGaussGradEval()
{
    if (!gaussPtsGradEvalPtr_)
    {
        calcGaussPtsGradEval();
    }
    const scalarField& gaussWeights = this->gaussWeights();

    const PtrList<scalarField>& gaussPts = *gaussPtsGradEvalPtr_;
    PtrList<scalarField> wtdGaussPts = gaussPts;

    // Go over all Gauss points for the whole mesh and add value*weight
    forAll(gaussPts, ptI)
    {
        if (ptI < gaussPts.size() - 2)
        {
            scalarField& wtdGPI = wtdGaussPts[ptI + 1];

            forAll (wtdGPI, modI)
            {
                // Zeroth entry is for coordinate -1
                wtdGPI[modI] *= gaussWeights[ptI];
            }
        }
    }

    return wtdGaussPts;
}




void dgPolynomials::test()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
