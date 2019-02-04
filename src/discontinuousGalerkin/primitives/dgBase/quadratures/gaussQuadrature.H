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
    Foam::gaussQuadrature

Description
    Definition of quadrature rules

Author
    Gregor Cvijetic.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef gaussQuadrature_H
#define gaussQuadrature_H

#include "dgBase.H"
//#include "primitiveFields.H"
//#include "IOdictionary.H"
//#include "polyMesh.H"
//#include "dgMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                          Class gaussQuadrature Declaration
\*---------------------------------------------------------------------------*/

class gaussQuadrature
:
    public  dgBase
{
    // Private data

    //- Number of modes taken into account
//    const label length_;

    const label quadratureOrder_;

    //- Weights for Gauss integration points
    scalarField gaussWeights_;

    //- Location of Gauss integration points (local CS)
    scalarField gaussPoints_;

    // Ptr list with size(number of Gauss points) with evaluated modes in
    // corresponding Gauss points, i.e. for 2 Gauss points and 3 modes, PtrList
    // size is 2, scalarField size is 3, in total 6
    mutable PtrList<scalarField>* gaussPtsEvalPtr_;

    mutable PtrList<scalarField>* gaussPtsGradEvalPtr_;


protected:

        void setConstants();

        //- Evaluate polynomials in Gauss points
        void calcGaussPtsEval() const;

        //- Evaluate gradient of polynomials in Gauss points
        void calcGaussPtsGradEval() const;


public:

    //- Runtime type information
    TypeName("gaussQuadrature");

    // Constructors

        //- Construct from gaussQuadrature
        gaussQuadrature
        (
            const polyMesh& mesh,
            const label quadratureOrder
        );


    // Member functions

        virtual const PtrList<scalarField>& gaussPtsEval() const
        {
            if (!gaussPtsEvalPtr_)
            {
                calcGaussPtsEval();
            }

            return *gaussPtsEvalPtr_;
        }

        virtual const PtrList<scalarField>& gaussPtsGradEval() const
        {
            if (!gaussPtsGradEvalPtr_)
            {
                calcGaussPtsGradEval();
            }

            return *gaussPtsGradEvalPtr_;
        }

        //- Modal functions evaluated in gauss points, without gauss weights
        virtual const PtrList<scalarField> wtdGaussEval() const;

        //- Gradient of modal functions evaluated in gauss points, without gauss
        // weights
        virtual const PtrList<scalarField> wtdGaussGradEval() const;


    // Access functions

        //- Return Gauss weights for reference element
        virtual const scalarField& gaussWeights() const
        {
            return gaussWeights_;
        }

        //- Return Gauss points positions for reference element (local CS)
        virtual const scalarField& gaussPoints() const
        {
            return gaussPoints_;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
