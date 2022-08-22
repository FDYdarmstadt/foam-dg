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

#include "BoSSScpp.h"

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

        //- Field type
        typedef DgGeometricField<Type, dgPatchField, cellMesh> FieldType;

        //- Solver type
        typedef BlockLduSolver<VectorN<scalar, Type::coeffLength> > SolverType;
//        typedef BlockLduSolver<MatrixType> SolverType;

        //- Solver performance type
        typedef BlockSolverPerformance<VectorN<scalar, Type::coeffLength> >
            SolverPerfType;

private:

        BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssObject_ = NULL;

        bool blockGC = true;

    // Private data

        // Reference to GeometricField<Type, dgPatchField, cellMesh>
        const DgGeometricField<Type, dgPatchField, cellMesh>& psi_;
       // GeometricField<Type, dgPatchField, cellMesh>& psi_;


        //- Dimension set
        dimensionSet dimensions_;

        //- Source term
        Field<Type> source_;

        // BoSSS::Application::ExternalBinding::OpenFoamMatrix bosssObjectValue_ = BoSSS::Application::ExternalBinding::OpenFoamMatrix(psi_.bosssObject_);
        // double* buffer_;

        // Private member functions

      public:
        //- Solver class returned by the solver function
        class dgSolver {
          // Private data

          //- Reference to matrix
          dgMatrix<Type> &dgMat_;

          //- Solver
          autoPtr<SolverType> solver_;

        public:
          // Constructors

          dgSolver(dgMatrix<Type> &dgMat, autoPtr<SolverType> sol)
              : dgMat_(dgMat), solver_(sol) {}

          // Member functions

          //- Solve returning the solution statistics.
          //  Solver controls read from dictionary
          SolverPerfType solve(const dictionary &);

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
        // dgMatrix
        // (
        //     const DgGeometricField<Type, dgPatchField, cellMesh>&,
        //     DgGeometricField<dgVector, dgPatchField, cellMesh>,
        //     const dimensionSet&
        // );

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

        // DgGeometricField<dgVector, dgPatchField, cellMesh> U_;
            // DgGeometricField<dgVector, dgPatchField, cellMesh>& U__()
            // {
            //     return U_;
            // }

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

            BoSSS::Application::ExternalBinding::OpenFoamMatrix* GetBoSSSobject() {
                if(bosssObject_ == NULL) {
                    //const DgGeometricField<Type, dgPatchField, cellMesh>& dgField = psi();
                    //const DgGeometricField<Type, dgPatchField, cellMesh>* pF = &dgField;
                    //const BoSSS::Application::ExternalBinding::OpenFoamDGField* p =
                    BoSSS::Application::ExternalBinding::OpenFoamDGField* p = psi().bosssObject_;
                    BoSSS::Foundation::Grid::OpenFOAMGrid* grd = psi().dgmesh_.GetBoSSSobject();
                    // BoSSS::Application::ExternalBinding::OpenFoamDGField* U = U__().bosssObject_;
                    // BoSSS::Application::ExternalBinding::OpenFoamDGField* U = U_.bosssObject_;
                    // if (U == NULL){
                        bosssObject_ = new BoSSS::Application::ExternalBinding::OpenFoamMatrix(grd, p);
                    // } else {
                    //     bosssObject_ = new BoSSS::Application::ExternalBinding::OpenFoamMatrix(grd, p, U);
                    // }
                    // bosssMonoObject_ = bosssObject_->_GetMonoObject();
                    // auto bosssObjectVal_ = BoSSS::Application::ExternalBinding::OpenFoamMatrix(p);
                    // bosssObject_ = &bosssObjectVal_;
                }
                // BoSSS::Application::ExternalBinding::OpenFoamDGField* p = psi().bosssObject_;
                // bosssObject_ = new BoSSS::Application::ExternalBinding::OpenFoamMatrix(p);
                // bosssObject_->AccBlock(0, 0, 1.0, buffer_);
                return bosssObject_;
                // return BoSSS::Application::ExternalBinding::OpenFoamMatrix::_FromMonoObject(bosssMonoObject_);
                // return &bosssObjectValue_;
            }
            void SetBoSSSobject(BoSSS::Application::ExternalBinding::OpenFoamMatrix* bo) {
                // if (buffer_ == NULL){
                //     int size = bo->GetNoOfRowsInBlock(0) * bo->GetNoOfColsInBlock(0);
                //     // int size = 100;
                //     Info << "size is " << size << endl;
                //     buffer_ = new double[size]; // TODO auto determine size
                // }
                // if(bosssObject_ == NULL) {
                // gchandle_ = mono_gchandle_new(bo->_GetMonoObject(), true);

                if(bosssObject_ == NULL) {
                    // const DgGeometricField<Type, dgPatchField, cellMesh>& dgField = psi();
                    // const DgGeometricField<Type, dgPatchField, cellMesh>* pF = &dgField;
                    // // const BoSSS::Application::ExternalBinding::OpenFoamDGField* p =
                    // BoSSS::Application::ExternalBinding::OpenFoamDGField* p = psi().bosssObject_;
                    Info << "Setting bosssobject (none already exists)" << endl;
                    // BoSSS::Application::ExternalBinding::OpenFoamMatrix bosssObjectValue_ = *bo;
                    // bosssObject_ = &bosssObjectValue_;
                    // BoSSS::Application::ExternalBinding::OpenFoamDGField* p = psi().bosssObject_;
                    // BoSSS::Application::ExternalBinding::OpenFoamMatrix bosssObjectVal_ = BoSSS::Application::ExternalBinding::OpenFoamMatrix(p) ;
                    // bo->GetBlock(0, 0, buffer_);
                    // bosssObject_ = BoSSS::Application::ExternalBinding::OpenFoamMatrix::_FromMonoObject(boc);
                    // bosssObjectVal_.AccBlock(0,0,1,InputReadBuffer);
                    // bosssObject_ = &bosssObjectVal_;
                    // bosssMonoObject_ = mono_object_clone(bo->_GetMonoObject());
                    bosssObject_ = bo;
                    // BoSSS::Application::ExternalBinding::OpenFoamMatrix bosssObjectTmp_(*bo);
                    // bosssObject_ = &bosssObjectTmp_;
                }
                else {
                    Info << "Setting bosssobject even though one exists already" << endl;
                    // BoSSS::Application::ExternalBinding::OpenFoamMatrix bosssObjectValue_ = *bo;
                    // bosssObject_ = &bosssObjectValue_;
                    // bo->GetBlock(0, 0, buffer_);
                    // MonoObject* boc = mono_object_clone(bo->_GetMonoObject());
                    bosssObject_ = bo;
                    // BoSSS::Application::ExternalBinding::OpenFoamMatrix bosssObjectTmp_(*bo);
                    // bosssObject_ = &bosssObjectTmp_;
                    // bosssObject_ = BoSSS::Application::ExternalBinding::OpenFoamMatrix::_FromMonoObject(boc);
                    // bosssMonoObject_ = bo->_GetMonoObject();
                    // bosssMonoObject_ = mono_object_clone(bo->_GetMonoObject());
                }
                // gchandle_ = mono_gchandle_new(bosssObject_->_GetMonoObject(), true);
                // gchandle_ = bo->_MonoGCHandle;
                // Info << "gchandle: " << gchandle_ << endl;

            }

        void AllowGC(){
            blockGC = false;
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

            // Data transfer: OpenFOAM --> BoSSS
            void SyncToBoSSS();

            // Data transfer: BoSSS --> OpenFOAM 
            void SyncFromBoSSS();

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

            //- solve within BoSSS, solution is stored in psi_
            void solveBoSSS();

            void print(Ostream& os);

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
