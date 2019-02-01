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
#include "IOdictionary.H"
#include "polyMesh.H"
//#include "dgMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                          Class dgPolynomials Declaration
\*---------------------------------------------------------------------------*/

class dgPolynomials
:
    public  IOdictionary
{
    // Private data

    const polyMesh& polyMesh_;
//    const dgMesh& mesh_;

    //- Number of modes taken into account
    const label length_;

    const label quadratureOrder_;

    const word quadratureRule_;

    //- Weights for Gauss integration points
    scalarField gaussWeights_;

    //- Location of Gauss integration points (local CS)
    scalarField gaussPoints_;
//    const vectorField gaussPoints_;

    quadrature quadrature_;

    // Ptr list with size(number of Gauss points) with evaluated modes in
    // corresponding Gauss points, i.e. for 2 Gauss points and 3 modes, PtrList
    // size is 2, scalarField size is 3, in total 6
    mutable PtrList<scalarField>* gaussPtsEvalPtr_;

    mutable PtrList<scalarField>* gaussPtsGradEvalPtr_;


protected:

        //- Evaluate polynomials in Gauss points
        void calcGaussPtsEval() const;

        //- Evaluate gradient of polynomials in Gauss points
        void calcGaussPtsGradEval() const;


public:

    // Constructors

        //- Construct from dgOrder
        dgPolynomials
        (
//            const dgMesh& mesh
            const polyMesh& mesh
//            const label
//            const dgOrder& dgOrder
        );


    // Member functions

        //- Return vector of modes evaluated in local coordinates
        const scalarField evaluate
        (
            const vector localCoords
        ) const;

        //- Return vector of gradient of modes evaluated in local coordinates
        const scalarField gradEvaluate
        (
            const vector localCoords
        ) const;

        const PtrList<scalarField>& gaussPtsEval() const
        {
            if (!gaussPtsEvalPtr_)
            {
                calcGaussPtsEval();
            }

            return *gaussPtsEvalPtr_;
        }

        const PtrList<scalarField>& gaussPtsGradEval() const
        {
            if (!gaussPtsGradEvalPtr_)
            {
                calcGaussPtsGradEval();
            }

            return *gaussPtsGradEvalPtr_;
        }

        //- Modal functions evaluated in gauss points, without gauss weights
        PtrList<scalarField> wtdGaussEval();

        //- Gradient of modal functions evaluated in gauss points, without gauss
        // weights
        PtrList<scalarField> wtdGaussGradEval();


    // Access functions

        //- Return Gauss weights for reference element
        const scalarField& gaussWeights() const
        {
            return gaussWeights_;
        }

        //- Return Gauss points positions for reference element (local CS)
        const scalarField& gaussPoints() const
        {
            return gaussPoints_;
        }

        void test();
//        {}

        label size() const
        {
            return length_;
        }

        label order() const
        {
            return dgOrder::order;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
