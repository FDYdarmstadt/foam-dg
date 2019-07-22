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
    dgMatrix<Type>

Description
    Discrete Galerkin matrix.

Author
    Hrvoje Jasak.  All rights reserved

Modified for DG
    Gregor Cvijetic, FMENA Zagreb. All rights reserved.

SourceFiles
    dgMatrix.C
    dgMatrixSolve.C

\*---------------------------------------------------------------------------*/

#ifndef dgMatrix_H
#define dgMatrix_H

#include "tmp.H"
#include "autoPtr.H"
#include "dimensionedDgTypes.H"
#include "cellFields.H"
#include "VectorN.H"
#include "className.H"
#include "BlockLduMatrix.H"
#include "BlockLduSolver.H"
#include "ExpandTensorNField.H"

#include "BlockSolverPerformanceVectorN.H"
#include "CoeffFieldFunctions.H"
#include "FieldFunctionsM.H"
#include "TensorNFields.H"
#include "dgMatrices.H"
//#include "blockLduSolvers.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type, int Size>
class dgMatrix;

template<class Type, int Size>
Ostream& operator<<(Ostream&, const dgMatrix<Type, Size>&);


/*---------------------------------------------------------------------------*\
                         Class dgMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type, int Size>
class dgMatrix
:
    public BlockLduMatrix<VectorN<scalar, Size> >,
    public dgMatrices
//    public BlockLduMatrix<Type>
{

public:

    // Public typedefs

        //- Subtype
        typedef VectorN<scalar, Size> SubType;

        //- BlockMatrix type
        typedef BlockLduMatrix<VectorN<scalar, Size> > MatrixType;

        //- Field type
        typedef GeometricField<Type, dgPatchField, cellMesh> FieldType;

        //- Solver type
        typedef BlockLduSolver<VectorN<scalar, Size> > SolverType;
//        typedef BlockLduSolver<MatrixType> SolverType;

        //- Solver performance type
        typedef BlockSolverPerformance<VectorN<scalar, Size> >
            SolverPerfType;


private:

    // Private data

        // Reference to GeometricField<Type, dgPatchField, cellMesh>
        const GeometricField<Type, dgPatchField, cellMesh>& psi_;
//        GeometricField<Type, dgPatchField, cellMesh>& psi_;

        //- Dimension set
        dimensionSet dimensions_;

        //- Source term
        Field<Type> source_;

    // Private member functions


public:

    //- Solver class returned by the solver function
    class dgSolver
    {
        // Private data

            //- Reference to matrix
            dgMatrix<Type, Size>& dgMat_;

            //- Solver
            autoPtr<SolverType> solver_;


    public:

        // Constructors

            dgSolver(dgMatrix<Type, Size>& dgMat, autoPtr<SolverType> sol)
            :
                dgMat_(dgMat),
                solver_(sol)
            {}


        // Member functions

            //- Solve returning the solution statistics.
            //  Solver controls read from dictionary
            SolverPerfType solve(const dictionary&);

            //- Solve returning the solution statistics.
            //  Solver controls read from dgSolution
            SolverPerfType solve();
    };


    // Public static data
    ClassName("dgMatrix");


    // Constructors

        //- Construct given a field to solve for
        dgMatrix
        (
            const GeometricField<Type, dgPatchField, cellMesh>&,
            const dimensionSet&
        );

        //- Construct as copy
        dgMatrix(const dgMatrix<Type, Size>&);


    //- Destructor
    virtual ~dgMatrix();


    // Member functions

        // Access

//            virtual const GeometricField<Type, dgPatchField, cellMesh>& psi() const
            virtual const GeometricField<dgScalar, dgPatchField, cellMesh>& psi() const
            {
                return psi_;
            }

            virtual const dimensionSet& dimensions() const
            {
                return dimensions_;
            }

            virtual Field<dgScalar>& source()
//            virtual Field<Type>& source()
            {
                return source_;
            }

            const Field<dgScalar>& source() const
//            const Field<Type>& source() const
            {
                return source_;
            }

            virtual CoeffField<dgScalar>& diag()
            {
                return BlockLduMatrix<VectorN<scalar, Size> >::diag();
            }

            virtual const CoeffField<dgScalar>& diag() const
            {
                return BlockLduMatrix<VectorN<scalar, Size> >::diag();
            }


    // Operations

            //- Relax matrix (for steady-state solution).
            //  alpha = 1 : diagonally equal
            //  alpha < 1 :    ,,      dominant
            //  alpha = 0 : do nothing
            void relax(const scalar alpha);

            //- Relax matrix (for steadty-state solution).
            //  alpha is read from controlDict
            void relax();

            //- Construct and return the solver
            //  Solver controls read from Istream
            autoPtr<dgSolver> solver(const dictionary&);

            //- Construct and return the solver
            //  Solver controls read from dgSolution
            autoPtr<dgSolver> solver();

            //- Solve returning the solution statistics.
            //  Solver controls read Istream
            SolverPerfType solve(const dictionary&);

            //- Solve returning the solution statistics.
            //  Solver controls read from dgSolution
            SolverPerfType solve();

            //- Return the matrix residual
            tmp<Field<Type> > residual() const;


    // Member operators

        void operator=(const dgMatrix<Type, Size>&);
        void operator=(const tmp<dgMatrix<Type, Size> >&);

        void negate();

        void operator+=(const dgMatrix<Type, Size>&);
        void operator+=(const tmp<dgMatrix<Type, Size> >&);

        void operator-=(const dgMatrix<Type, Size>&);
        void operator-=(const tmp<dgMatrix<Type, Size> >&);

        // Note: matrix-field operations are not allowed.
        // Su, Sp, SuSp operators to be used instead to handle integration.
        // HJ, 10/Mar/2017
        void operator*=(const dimensionedDgScalar&);


    // Ostream operator

        friend Ostream& operator<< <Type, Size>
        (
            Ostream&,
            const dgMatrix<Type, Size>&
        );
};


// * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * //

template<class Type, int Size>
void checkMethod
(
    const dgMatrix<Type, Size>&,
    const dgMatrix<Type, Size>&,
    const char*
);

//- Solve returning the solution statistics given convergence tolerance,
//  Solver controls read Istream
template<class Type, int Size>
BlockSolverPerformance<Type> solve(const dgMatrix<Type, Size>&, Istream&);


//- Solve returning the solution statistics given convergence tolerance,
//  deleting temporary matrix after solution.
//  Solver controls read Istream
template<class Type, int Size>
BlockSolverPerformance<Type> solve(const tmp<dgMatrix<Type, Size> >&, Istream&);


//- Solve returning the solution statistics given convergence tolerance
//  Solver controls read dgSolution
template<class Type, int Size>
BlockSolverPerformance<Type> solve(dgMatrix<Type, Size>&);


//- Solve returning the solution statistics given convergence tolerance,
//  deleting temporary matrix after solution.
//  Solver controls read dgSolution
template<class Type, int Size>
BlockSolverPerformance<Type> solve(const tmp<dgMatrix<Type, Size> >&);


// * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * //

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator-
(
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator-
(
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator+
(
    const dgMatrix<Type, Size>&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator+
(
    const tmp<dgMatrix<Type, Size> >&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator+
(
    const dgMatrix<Type, Size>&,
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator+
(
    const tmp<dgMatrix<Type, Size> >&,
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator-
(
    const dgMatrix<Type, Size>&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator-
(
    const tmp<dgMatrix<Type, Size> >&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator-
(
    const dgMatrix<Type, Size>&,
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator-
(
    const tmp<dgMatrix<Type, Size> >&,
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator==
(
    const dgMatrix<Type, Size>&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator==
(
    const tmp<dgMatrix<Type, Size> >&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator==
(
    const dgMatrix<Type, Size>&,
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator==
(
    const tmp<dgMatrix<Type, Size> >&,
    const tmp<dgMatrix<Type, Size> >&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator*
(
    const dimensioned<scalar>&,
    const dgMatrix<Type, Size>&
);

template<class Type, int Size>
tmp<dgMatrix<Type, Size> > operator*
(
    const dimensioned<scalar>&,
    const tmp<dgMatrix<Type, Size> >&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgMatrix.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
