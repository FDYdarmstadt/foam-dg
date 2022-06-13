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
//#include "blockLduSolvers.H"

#include "BlockMsrMatrix.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class dgMatrix;

template<class Type>
Ostream& operator<<(Ostream&, const dgMatrix<Type>&);


/*---------------------------------------------------------------------------*\
                         Class dgMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class dgMatrix
:
    public BlockLduMatrix<VectorN<scalar, Type::coeffLength> >
//    public BlockLduMatrix<Type>
{
public:

    // Public typedefs

        //- Subtype
        typedef VectorN<scalar, Type::coeffLength> SubType;

        //- BlockMatrix type
        typedef BlockLduMatrix<VectorN<scalar, Type::coeffLength> > MatrixType;

        //- BoSSSBlockMatrix type
        typedef ilPSP::LinSolvers::BlockMsrMatrix BoSSSMatrixType;

        //- source type
        typedef ilPSP::LinSolvers::BlockMsrMatrix BoSSSSourceType;

        //- Field type
        typedef DgGeometricField<Type, dgPatchField, cellMesh> FieldType;

        //- Solver type
        typedef BlockLduSolver<VectorN<scalar, Type::coeffLength> > SolverType;
//        typedef BlockLduSolver<MatrixType> SolverType;

        //- Solver performance type
        typedef BlockSolverPerformance<VectorN<scalar, Type::coeffLength> >
            SolverPerfType;


private:

    // Private data

        // Reference to GeometricField<Type, dgPatchField, cellMesh>
        const DgGeometricField<Type, dgPatchField, cellMesh>& psi_;
       // GeometricField<Type, dgPatchField, cellMesh>& psi_;

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
            dgMatrix<Type>& dgMat_;

            //- Solver
            autoPtr<SolverType> solver_;


    public:

        // Constructors

            dgSolver(dgMatrix<Type>& dgMat, autoPtr<SolverType> sol)
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
            const DgGeometricField<Type, dgPatchField, cellMesh>&,
            const dimensionSet&
        );

        //- Construct as copy
        dgMatrix(const dgMatrix<Type>&);


    //- Destructor
    virtual ~dgMatrix();


    // Member functions

        // Access

            const DgGeometricField<Type, dgPatchField, cellMesh>& psi() const
            // DgGeometricField<Type, dgPatchField, cellMesh>& psi() const
            {
                return psi_;
            }

            const dimensionSet& dimensions() const
            {
                return dimensions_;
            }

            Field<Type>& source()
            {
                return source_;
            }

            const Field<Type>& source() const
            {
                return source_;
            }


    // Operations

            //- Relax matrix (for steady-state solution).
            //  alpha = 1 : diagonaly equal
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

        void operator=(const dgMatrix<Type>&);
        void operator=(const tmp<dgMatrix<Type> >&);

        void negate();

        void operator+=(const dgMatrix<Type>&);
        void operator+=(const tmp<dgMatrix<Type> >&);

        void operator-=(const dgMatrix<Type>&);
        void operator-=(const tmp<dgMatrix<Type> >&);

        // Note: matrix-field operations are not allowed.
        // Su, Sp, SuSp operators to be used instead to handle integration.
        // HJ, 10/Mar/2017
        void operator*=(const dimensionedDgScalar&);


    // Ostream operator

        friend Ostream& operator<< <Type>
        (
            Ostream&,
            const dgMatrix<Type>&
        );
};


// * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * //

template<class Type>
void checkMethod
(
    const dgMatrix<Type>&,
    const dgMatrix<Type>&,
    const char*
);

//- Solve returning the solution statistics given convergence tolerance,
//  Solver controls read Istream
template<class Type>
BlockSolverPerformance<Type> solve(const dgMatrix<Type>&, Istream&);


//- Solve returning the solution statistics given convergence tolerance,
//  deleting temporary matrix after solution.
//  Solver controls read Istream
template<class Type>
BlockSolverPerformance<Type> solve(const tmp<dgMatrix<Type> >&, Istream&);


//- Solve returning the solution statistics given convergence tolerance
//  Solver controls read dgSolution
template<class Type>
BlockSolverPerformance<Type> solve(dgMatrix<Type>&);


//- Solve returning the solution statistics given convergence tolerance,
//  deleting temporary matrix after solution.
//  Solver controls read dgSolution
template<class Type>
BlockSolverPerformance<Type> solve(const tmp<dgMatrix<Type> >&);


// * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * //

template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const dgMatrix<Type>&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const tmp<dgMatrix<Type> >&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const dgMatrix<Type>&,
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const tmp<dgMatrix<Type> >&,
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const dgMatrix<Type>&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const tmp<dgMatrix<Type> >&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const dgMatrix<Type>&,
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const tmp<dgMatrix<Type> >&,
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const dgMatrix<Type>&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const tmp<dgMatrix<Type> >&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const dgMatrix<Type>&,
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const tmp<dgMatrix<Type> >&,
    const tmp<dgMatrix<Type> >&
);

template<class Type>
tmp<dgMatrix<Type> > operator*
(
    const dimensioned<scalar>&,
    const dgMatrix<Type>&
);

template<class Type>
tmp<dgMatrix<Type> > operator*
(
    const dimensioned<scalar>&,
    const tmp<dgMatrix<Type> >&
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
