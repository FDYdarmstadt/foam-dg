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

#include "dgBase.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(dgBase, 0);
defineRunTimeSelectionTable(dgBase, dictionary);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//void dgBase::setConstants() const
//{
    // Create polynomials object
//    polynomialsPts_ = new dgPolynomials(readLabel(lookup("polynomialOrder")));

//}

//// The constants are set depending on the quadrature order
//
//    if (quadratureOrder_ == 0)
//    {
//        // ERROR - unable 0th order
//    }
//    if (quadratureOrder_ == 1)
//    {
//        gaussWeights_[0] = 5.0/9.0;
//
//        gaussPoints_[0] = - sqrt(3.0/5.0);
//    }
//    if (quadratureOrder_ == 2)
//    {
//        gaussWeights_[0] = 5.0/9.0;
//        gaussWeights_[1] = 8.0/9.0;
//
//        gaussPoints_[0] = - sqrt(3.0/5.0);
//        gaussPoints_[1] = 0;
//    }
//    if (quadratureOrder_ == 3)
//    {
//        gaussWeights_[0] = 5.0/9.0;
//        gaussWeights_[1] = 8.0/9.0;
//        gaussWeights_[2] = 5.0/9.0;
//
//        gaussPoints_[0] = - sqrt(3.0/5.0);
//        gaussPoints_[1] = 0;
//        gaussPoints_[2] = sqrt(3.0/5.0);
//    }
//    if (quadratureOrder_ == 4)
//    {
//        gaussWeights_[0] = 5.0/9.0;
//        gaussWeights_[1] = 8.0/9.0;
//        gaussWeights_[2] = 5.0/9.0;
//        gaussWeights_[3] = 5.0/9.0;
//
//        gaussPoints_[0] = - sqrt(3.0/5.0);
//        gaussPoints_[1] = 0;
//        gaussPoints_[2] = sqrt(3.0/5.0);
//        gaussPoints_[3] = sqrt(3.0/5.0);
//    }
//    if (quadratureOrder_ == 5)
//    {
//        gaussWeights_[0] = 5.0/9.0;
//        gaussWeights_[1] = 8.0/9.0;
//        gaussWeights_[2] = 5.0/9.0;
//        gaussWeights_[3] = 5.0/9.0;
//        gaussWeights_[4] = 5.0/9.0;
//
//        gaussPoints_[0] = - sqrt(3.0/5.0);
//        gaussPoints_[1] = 0;
//        gaussPoints_[2] = sqrt(3.0/5.0);
//        gaussPoints_[3] = sqrt(3.0/5.0);
//        gaussPoints_[4] = sqrt(3.0/5.0);
//    }
//    if (quadratureOrder_ == 6)
//    {
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
//    }
//
//}


//void dgBase::calcGaussPtsEval() const
//{
//    // Local coordinates (reference element)
//    const scalarField& gaussCoords = gaussPoints();
////    const scalarField& gaussWeights = this->gaussWeights();
//
//    // Evaluate in -1, 1 and gauss points
//    gaussPtsEvalPtr_ = new PtrList<scalarField>(gaussCoords.size() + 2);
//
//    // Go over all Gauss points for the whole mesh and add value*weight
//    forAll(*gaussPtsEvalPtr_, ptI)
//    {
//        scalarField polyEval(gaussCoords.size(), 0.0);
//
//        // Evaluate in -1
//        if (ptI == 0)
//        {
//            polyEval = evaluate(vector(-1,0,0));
//        }
//        // Evaluate in 1
//        else if (ptI == (gaussPtsEvalPtr_->size() - 1))
//        {
//            polyEval = evaluate(vector(1,0,0));
//        }
//        // Evaluate in Gauss points
//        else
//        {
//            // Calculate modal values in given coordinate (Gaussian point
//            // coordinate)
//            polyEval = evaluate(vector(gaussCoords[ptI - 1],0,0));
//
////            forAll (polyEval, modI)
////            {
////                polyEval[modI] *= gaussWeights[ptI];
////            }
//        }
//
//        gaussPtsEvalPtr_->set
//        (
//            ptI,
//            new scalarField (polyEval)
//        );
//    }
//}
//
//
//void dgBase::calcGaussPtsGradEval() const
//{
//    // Local coordinates (reference element)
//    const scalarField& gaussCoords = gaussPoints();
////    const scalarField& gaussWeights = this->gaussWeights();
//
//    gaussPtsGradEvalPtr_ = new PtrList<scalarField>(gaussCoords.size() + 2);
//    // Go over all Gaussian points for the whole mesh and add value*weight
//    forAll(*gaussPtsGradEvalPtr_, ptI)
//    {
//        // This is of size dgScalar.size()
//        scalarField polyEval(gaussCoords.size(), 0.0);
//
//        // Evaluate in -1
//        if (ptI == 0)
//        {
//            polyEval = gradEvaluate(vector(-1,0,0));
//        }
//        // Evaluate in 1
//        else if (ptI == (gaussPtsGradEvalPtr_->size() - 1))
//        {
//            polyEval = gradEvaluate(vector(1,0,0));
//        }
//        // Evaluate in Gauss points
//        else
//        {
//            // Calculate modal values in given coordinate (Gaussian point
//            // coordinate)
//            polyEval = gradEvaluate(vector(gaussCoords[ptI - 1],0,0));
//
////            forAll (polyEval, modI)
////            {
////                polyEval[modI] *= gaussWeights[ptI];
////            }
//        }
//
//        gaussPtsGradEvalPtr_->set
//        (
//            ptI,
//            new scalarField (polyEval)
//        );
//    }
//}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
dgBase::dgBase
(
    const polyMesh& mesh
)
:
    IOdictionary
    (
        IOobject
        (
            "dgDict",
            mesh.time().system(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    polyMesh_(mesh),
    length_(dgOrder::length),//dgOrder.length()),
    polynomialsPtr_(NULL)
//    gaussWeights_(1, 0.0),
//    gaussPoints_(1, 0.0),
//    gaussWeights_(quadratureOrder_, 0.0),
//    gaussPoints_(quadratureOrder_, 0.0),
//    polynomials_(length_),
//    gaussPtsEvalPtr_(NULL),//gaussPoints_.size()),
//    gaussPtsGradEvalPtr_(NULL)//gaussPoints_.size())
{
//    setConstants();

    // Create polynomials object
    polynomialsPtr_ = new dgPolynomials(readLabel(lookup("polynomialOrder")) + 1);
}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

autoPtr<dgBase> dgBase::New
(
    const polyMesh& mesh
)
{
    Info<< " NEW SELECTOR DGBASE " << endl;

    const dictionary& dict =
    IOdictionary
    (
        IOobject
        (
            "dgDict",
            mesh.time().system(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    Info<< " Reading quadrature rule and order: " << endl;

    const word quadratureRule(dict.lookup("quadratureRule"));
    const label quadratureOrder(readLabel(dict.lookup("quadratureOrder")));
    Info<< " Rule and order read in. Finding in iterator quadrature rule: " << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(quadratureRule);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
            (
            "dgBase::dgBase::New\n"
            "(\n"
            "   const polyMesh& mesh,\n"
            ")"
            )   << "Unknown quadrature type "
                << quadratureRule << endl << endl
                << "Valid quadrature types are: " << endl
                << dictionaryConstructorTablePtr_->toc()
                << exit(FatalError);
    }

    Info<< " Returning a constructor: " << endl;
    return autoPtr<dgBase>(cstrIter()(mesh, quadratureOrder));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template<class Type>
//const field<Type>& dgBase::evaluate
const scalarField dgBase::evaluate
(
    const vector localCoords
) const
{
    return polynomialsPtr_->evaluate(localCoords);
}


const scalarField dgBase::gradEvaluate
(
    const vector localCoords
) const
{
    return polynomialsPtr_->gradEvaluate(localCoords);
}


void dgBase::test()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
