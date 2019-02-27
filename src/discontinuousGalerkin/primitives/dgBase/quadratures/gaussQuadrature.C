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

#include "gaussQuadrature.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(gaussQuadrature, 0);
addToRunTimeSelectionTable
(
    dgBase,
    gaussQuadrature,
    dictionary
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void gaussQuadrature::setConstants()
{
// The constants are set depending on the quadrature order

    if (quadratureOrder_ == 0)
    {
        // ERROR - unable 0th order
    }
    if (quadratureOrder_ == 1)
    {
        Info<< "Deploying first order quadrature rule." << endl;

        gaussWeights_[0] = 2;

        gaussPoints_[0] = 0;
    }
    if (quadratureOrder_ == 2)
    {
        Info<< "Deploying second order quadrature rule." << endl;

        gaussWeights_[0] = 1.0;
        gaussWeights_[1] = 1.0;

        gaussPoints_[0] = sqrt(3.0/5.0);
        gaussPoints_[1] = - sqrt(3.0/5.0);
    }
    if (quadratureOrder_ == 3)
    {
        Info<< "Deploying third order quadrature rule." << endl;

        gaussWeights_[0] = 5.0/9.0;
        gaussWeights_[1] = 8.0/9.0;
        gaussWeights_[2] = 5.0/9.0;

        gaussPoints_[0] = - sqrt(3.0/5.0);
        gaussPoints_[1] = 0;
        gaussPoints_[2] = sqrt(3.0/5.0);
    }
    if (quadratureOrder_ == 4)
    {
        Info<< "Deploying fourth order quadrature rule." << endl;

        gaussWeights_[0] = (18.0 + sqrt(30.0))/36.0;
        gaussWeights_[1] = (18.0 + sqrt(30.0))/36.0;
        gaussWeights_[2] = (18.0 - sqrt(30.0))/36.0;
        gaussWeights_[3] = (18.0 - sqrt(30.0))/36.0;

        gaussPoints_[0] = sqrt(3.0/7.0 - 2.0/7.0*sqrt(6.0/5.0));
        gaussPoints_[1] = - sqrt(3.0/7.0 - 2.0/7.0*sqrt(6.0/5.0));
        gaussPoints_[2] = sqrt(3.0/7.0 + 2.0/7.0*sqrt(6.0/5.0));
        gaussPoints_[3] = - sqrt(3.0/7.0 + 2.0/7.0*sqrt(6.0/5.0));
    }
    if (quadratureOrder_ == 5)
    {
        Info<< "Deploying fifth order quadrature rule." << endl;

        gaussWeights_[0] = (322.0 + 13.0*sqrt(70.0))/900.0;
        gaussWeights_[1] = (322.0 + 13.0*sqrt(70.0))/900.0;
        gaussWeights_[2] = 128.0/225.0;
        gaussWeights_[3] = (322.0 - 13.0*sqrt(70.0))/900.0;
        gaussWeights_[4] = (322.0 - 13.0*sqrt(70.0))/900.0;

        gaussPoints_[0] = 1.0/3.0*sqrt(5.0 - 2.0*sqrt(10.0/7.0));
        gaussPoints_[1] = - 1.0/3.0*sqrt(5.0 - 2.0*sqrt(10.0/7.0));
        gaussPoints_[2] = 0;
        gaussPoints_[3] = 1.0/3.0*sqrt(5.0 + 2.0*sqrt(10.0/7.0));
        gaussPoints_[4] = - 1.0/3.0*sqrt(5.0 + 2.0*sqrt(10.0/7.0));
    }
    if (quadratureOrder_ > 5)
    {
        Info<< "Quadrature order higher than 5 not implemented." << endl;

//        Info<< "Deploying sixth order quadrature rule." << endl;
//
//        gaussWeights_[0] = 5.0/9.0;
//        gaussWeights_[1] = 8.0/9.0;
//        gaussWeights_[2] = 5.0/9.0;
//        gaussWeights_[3] = 5.0/9.0;
//        gaussWeights_[4] = 5.0/9.0;
//        gaussWeights_[5] = 5.0/9.0;
//
//        gaussPoints_[0] = - sqrt(3.0/5.0);
//        gaussPoints_[1] = 0;
//        gaussPoints_[2] = sqrt(3.0/5.0);
//        gaussPoints_[3] = sqrt(3.0/5.0);
//        gaussPoints_[4] = sqrt(3.0/5.0);
//        gaussPoints_[5] = sqrt(3.0/5.0);
    }

}


void gaussQuadrature::calcGaussPtsEval() const
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


void gaussQuadrature::calcGaussPtsGradEval() const
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

gaussQuadrature::gaussQuadrature
(
    const polyMesh& mesh,
    const label quadratureOrder
)
:
    dgBase(mesh),
//    length_(dgOrder::length),//dgOrder.length()),
    quadratureOrder_(quadratureOrder),
    gaussWeights_(quadratureOrder, 0.0),
    gaussPoints_(quadratureOrder, 0.0),
    gaussPtsEvalPtr_(NULL),//gaussPoints_.size()),
    gaussPtsGradEvalPtr_(NULL)//gaussPoints_.size())
{
    setConstants();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


const PtrList<scalarField> gaussQuadrature::wtdGaussEval() const
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


const PtrList<scalarField> gaussQuadrature::wtdGaussGradEval() const
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
//Info<< "EXIT WTD GRAD GAUSS EVAL" << endl;

    return wtdGaussPts;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
