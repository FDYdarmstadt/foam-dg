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
    Foam::dgBase

Description
    Definition of a Discontinuous Galerkin Polynomials

Author
    Gregor Cvijetic.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef dgBase_H
#define dgBase_H

#include "dgOrder.H"
#include "primitiveFields.H"
#include "IOdictionary.H"
#include "polyMesh.H"
#include "dgPolynomials.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                          Class dgBase Declaration
\*---------------------------------------------------------------------------*/

class dgBase
:
    public  IOdictionary
{
    // Private data

    const polyMesh& polyMesh_;
//    const dgMesh& mesh_;

    //- Number of modes taken into account
    const label length_;

    mutable dgPolynomials* polynomialsPtr_;
//    const label quadratureOrder_;

//    const word quadratureRule_;

    //- Weights for Gauss integration points
//    scalarField gaussWeights_;

    //- Location of Gauss integration points (local CS)
//    scalarField gaussPoints_;
//    const vectorField gaussPoints_;

//    quadrature& quadrature_;

//    polynomial& polynomials_;

    // Ptr list with size(number of Gauss points) with evaluated modes in
    // corresponding Gauss points, i.e. for 2 Gauss points and 3 modes, PtrList
    // size is 2, scalarField size is 3, in total 6
//    mutable PtrList<scalarField>* gaussPtsEvalPtr_;

//    mutable PtrList<scalarField>* gaussPtsGradEvalPtr_;


protected:

//        virtual void setConstants() const;
//
//        //- Evaluate polynomials in Gauss points
//        virtual void calcGaussPtsEval() const;
//
//        //- Evaluate gradient of polynomials in Gauss points
//        virtual void calcGaussPtsGradEval() const;


public:

    //- Runtime type information
    TypeName("dgBase");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            dgBase,
            dictionary,
            (
                const polyMesh& mesh,
                const label quadratureOrder
            ),
            (mesh, quadratureOrder)
        );

    // Constructors

        //- Construct from dgOrder
        dgBase
        (
//            const dgMesh& mesh
            const polyMesh& mesh
//            const label
//            const dgOrder& dgOrder
        );

    // Selectors

        //- Return an autoPtr to the selected dgBase (quadrature type)
        static autoPtr<dgBase> New
        (
            const polyMesh& mesh
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

        virtual const PtrList<scalarField>& gaussPtsEval() const = 0;

        virtual const PtrList<scalarField>& gaussPtsGradEval() const = 0;

        //- Modal functions evaluated in gauss points, without gauss weights
        virtual const PtrList<scalarField> wtdGaussEval() const = 0;

        //- Gradient of modal functions evaluated in gauss points, without gauss
        // weights
        virtual const PtrList<scalarField> wtdGaussGradEval() const = 0;


    // Access functions

        //- Return Gauss weights for reference element
        virtual const scalarField& gaussWeights() const = 0;

        //- Return Gauss points positions for reference element (local CS)
        virtual const scalarField& gaussPoints() const = 0;

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


        const dgPolynomials& polynomials()
        {
            return *polynomialsPtr_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#ifdef NoRepository
//#   include "dgBase.C"
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
