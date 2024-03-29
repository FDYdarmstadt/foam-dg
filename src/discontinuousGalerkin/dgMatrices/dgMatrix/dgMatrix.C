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

#include "cellFields.H"
#include "dgMatrix.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

template<class Type>
dgMatrix<Type>::dgMatrix
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& psi,
    const dimensionSet& ds
)
:
    BlockLduMatrix<VectorN<scalar, Type::coeffLength> >(psi.mesh()),
    psi_(psi),
    dimensions_(ds),
    source_(psi.size(), pTraits<Type>::zero)
{
    Info << "CONSTRUCT MATRIX" << endl;

    if (debug)
    {
        InfoIn
        (
            "dgMatrix<Type>\n"
            "(\n"
            "    const DgGeometricField<Type, dgPatchField, cellMesh>&,\n"
            "    const dimensionSet&\n"
        )   << "constructing dgMatrix<Type> for field " << psi_.name()
            << endl;
    }

    // Update the boundary coefficients of psi without changing its event No.
    DgGeometricField<Type, dgPatchField, cellMesh>& psiRef =
       const_cast<DgGeometricField<Type, dgPatchField, cellMesh>&>(psi_);

    label currentStatePsi = psiRef.eventNo();
    psiRef.boundaryField().updateCoeffs();
    psiRef.eventNo() = currentStatePsi;
}


template<class Type>
dgMatrix<Type>::dgMatrix(const dgMatrix<Type>& dgm)
:
    BlockLduMatrix<VectorN<scalar, Type::coeffLength> >(dgm),
    psi_(dgm.psi_),
    dimensions_(dgm.dimensions_),
    source_(dgm.source_)
{
    BoSSS::Application::ExternalBinding::OpenFoamMatrix* bo = dgm.bosssObject_;
    SetBoSSSobject(bo);

    if (debug)
    {
        InfoIn("dgMatrix<Type>::dgMatrix(const dgMatrix<Type>&)")
            << "copying dgMatrix<Type> for field " << psi_.name()
            << endl;
    }
    // flux_ = dgm.flux_;
}

template <class Type> dgMatrix<Type>::~dgMatrix() {
  if (bosssObject_ != NULL && blockGC == false) {
  // if (bosssObject_ != NULL) {
    if (debug || true) {
      InfoIn("dgMatrix<Type>::~dgMatrix<Type>()")
          << "destroying dgMatrix<Type> for field " << psi_.name() << endl;
    }

    delete bosssObject_;
  }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void dgMatrix<Type>::relax(const scalar alpha)
{
    if (alpha <= 0)
    {
        return;
    }

    MatrixType::relax(psi_, source_, alpha);
}

template<class Type>
void dgMatrix<Type>::relax()
{
    if (psi_.mesh().solutionDict().relaxEquation(psi_.name()))
    {
        relax(psi_.mesh().solutionDict().equationRelaxationFactor(psi_.name()));
    }
    else
    {
        if (debug)
        {
            InfoIn("void dgMatrix<Type>::relax()")
                << "Relaxation factor for field " << psi_.name()
                << " not found.  Relaxation will not be used." << endl;
        }
    }
}

template<class Type>
void dgMatrix<Type>::SyncToBoSSS()
{
    // forAll (vf, cellI)
    // {
    //     forAll (polynomials, modI)
    //     {
    //         forAll (polynomials, modJ)
    //         {
    //             label coeff = modI*polynomials.size() + modJ;

    //             ds[cellI](modI, modJ) =
    //                 gradCoeffs[coeff]/scaleCells[cellI];
    //         }
    //     }
    // }
}

template<class Type>
void dgMatrix<Type>::SyncFromBoSSS()
{
    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
        = (*this).diag().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ls
        = (*this).lower().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& us
        = (*this).upper().asSquare();

    BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = GetBoSSSobject();

    // const labelList& neiList = psi().dgmesh().faceNeighbour();

    // diagonal
    // Go over all cells
    forAll (psi(), cellI)
    {
        int Nbosss = bosssMtx->GetNoOfRowsInBlock(cellI);
        int Mbosss = bosssMtx->GetNoOfColsInBlock(cellI);
        double* tmp = new double[Nbosss*Mbosss];
        bosssMtx->GetBlock(cellI, cellI, tmp);

        int Nfoam = dgOrder::length;
        int Mfoam = dgOrder::length;

        // Note (FK, 18apr21): sinde Nfoam and Mfoam are wrong, the following code will never capture the entire matrix
        int N = Nbosss > Nfoam ? Nfoam : Nbosss;
        int M = Nbosss > Nfoam ? Nfoam : Nbosss;

        for(int i = 0; i < N; i++) { // row lop
            for(int j = 0; j < M; j++) { // col loop
                ds[cellI](i, j) = tmp[i*Mbosss + j];
            }
        }


        delete tmp;
    }

    // Upper triangular (or lower???)
    // Go over all internal faces (only those that have nei)
    // forAll (neiList, faceI)
    // {

    //     // Nei is -1 for boundary faces
    //     const label& own = psi().dgmesh().faceOwner()[faceI];
    //     const label& nei = psi().dgmesh().faceNeighbour()[faceI];

    //     int Nbosss = bosssMtx->GetNoOfRowsInBlock(own);
    //     int Mbosss = bosssMtx->GetNoOfColsInBlock(nei);
    //     double* tmp = new double[Nbosss*Mbosss];
    //     bosssMtx->GetBlock(own, nei, tmp);

    //     int Nfoam = dgOrder::length;
    //     int Mfoam = dgOrder::length;

    //     // Note (FK, 18apr21): sinde Nfoam and Mfoam are wrong, the following code will never capture the entire matrix
    //     int N = Nbosss > Nfoam ? Nfoam : Nbosss;
    //     int M = Nbosss > Nfoam ? Nfoam : Nbosss;

    //     for(int i = 0; i < N; i++) { // row lop
    //         for(int j = 0; j < M; j++) { // col loop
    //             us[faceI](i, j) = tmp[i*Mbosss + j];
    //         }
    //     }

    //     delete tmp;
    // }

    // // lower triangular (or upper???)
    // // Go over all internal faces (only those that have nei)
    // forAll (neiList, faceI)
    // {

    //     // Nei is -1 for boundary faces
    //     const label& own = psi().dgmesh().faceOwner()[faceI];
    //     const label& nei = psi().dgmesh().faceNeighbour()[faceI];

    //     int Nbosss = bosssMtx->GetNoOfRowsInBlock(nei);
    //     int Mbosss = bosssMtx->GetNoOfColsInBlock(own);
    //     double* tmp = new double[Nbosss*Mbosss];
    //     bosssMtx->GetBlock(nei, own, tmp);

    //     int Nfoam = dgOrder::length;
    //     int Mfoam = dgOrder::length;

    //     // Note (FK, 18apr21): sinde Nfoam and Mfoam are wrong, the following code will never capture the entire matrix
    //     int N = Nbosss > Nfoam ? Nfoam : Nbosss;
    //     int M = Nbosss > Nfoam ? Nfoam : Nbosss;

    //     for(int i = 0; i < N; i++) { // row lop
    //         for(int j = 0; j < M; j++) { // col loop
    //             ds[faceI](i, j) = tmp[i*Mbosss + j];
    //         }
    //     }

    //     delete tmp;
    // }
  
}

template<class Type>
void dgMatrix<Type>::solveBoSSS()
{
    BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = GetBoSSSobject();

    bosssMtx->Solve();

    label J = psi_.dgmesh_.mesh().nCells();
    // OpenFoamDGField *bo = GetBoSSSobject();

    DgGeometricField<Type, dgPatchField, cellMesh>& psiRef =
       const_cast<DgGeometricField<Type, dgPatchField, cellMesh>&>(psi_);
    if (typeid(Type) == typeid(dgScalar)) {
      for (label j = 0; j < J; j++) {
        // int N = dgOrder::length;
        // int N = (*this)[j].size();
        int N = psiRef[j].size();
        dgScalar cellValue;
        for (int n = 0; n < N; n++) {
          cellValue[n] = bosssObject_->GetSolCoordinate(0, j, n);
          // cellValue[n] = bosssObject_->GetRHSCoordinate(0, j, n);
        }

        // Info << cellValue << endl;
        psiRef[j] = cellValue;
        // psi_[j] = cellValue;
      }
    }

    // psi_.SyncFromBoSSS();
    // ../discontinuousGalerkin/lnInclude/dgMatrix.C:271:6: 
    // error: passing ‘const Foam::DgGeometricField<Foam::DgScalar<double, 3>, Foam::dgPatchField, Foam::cellMesh>’ 
    // as ‘this’ argument discards qualifiers [-fpermissive]
    //271 |      psi().SyncFromBoSSS();
    //  |      ^~~
    // SyncFromBoSSS();
    // psiRef.SyncFromBoSSS();
    // AllowGC();

}

// template<class Type>
// OpenFoamDGField dgMatrix<Type>::GetPhi(){
//     BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = GetBoSSSobject();

//     return bosssMtx->GetPhi();
// }

template<class Type>
void dgMatrix<Type>::print(Ostream& os)
{
    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
        = (*this).diag().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ls
        = (*this).lower().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& us
        = (*this).upper().asSquare();

    // const labelList& neiList = psi().dgmesh().interfaces();

    // diagonal
    // Go over all cells
    forAll (psi(), cellI)
    {
        int Nfoam = dgOrder::length;
        int Mfoam = dgOrder::length;

        // Note (FK, 18apr21): sinde Nfoam and Mfoam are wrong, the following code will never capture the entire matrix
        int N = Nfoam;
        int M = Nfoam;

        for(int i = 0; i < N; i++) { // row lop
            for(int j = 0; j < M; j++) { // col loop
                os <<
                ds[cellI](i, j)
                    << " ";
            }
            os << endl;
        }
        os << endl;
    }

    // Upper triangular (or lower???)
    // Go over all internal faces (only those that have nei)
    // forAll (neiList, faceI)
    // {
    //     int Nfoam = dgOrder::length;
    //     int Mfoam = dgOrder::length;

    //     int N = Nfoam;
    //     int M = Nfoam;

    //     for(int i = 0; i < N; i++) { // row lop
    //         for(int j = 0; j < M; j++) { // col loop
    //             os <<
    //             us[faceI](i, j)
    //                 << endl;
    //         }
    //     }
    // }

    // // lower triangular (or upper???)
    // // Go over all internal faces (only those that have nei)
    // forAll (neiList, faceI)
    // {

    //     int Nfoam = dgOrder::length;
    //     int Mfoam = dgOrder::length;

    //     // Note (FK, 18apr21): sinde Nfoam and Mfoam are wrong, the following code will never capture the entire matrix
    //     int N = Nfoam;
    //     int M = Nfoam;

    //     for(int i = 0; i < N; i++) { // row lop
    //         for(int j = 0; j < M; j++) { // col loop
    //             os <<
    //             ds[faceI](i, j)
    //                 << endl;
    //         }
    //     }
    // }
}

// template<class Type>
// cellVectorField dgMatrix<Type>::flux(){
//     return flux_;
// }

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void dgMatrix<Type>::operator=(const dgMatrix<Type>& dgmv)
{
    if (this == &dgmv)
    {
        FatalErrorIn("dgMatrix<Type>::operator=(const dgMatrix<Type>&)")
            << "attempted to assignment to self"
            << abort(FatalError);
    }

    if (&psi_ != &(dgmv.psi_))
    {
        FatalErrorIn("dgMatrix<Type>::operator=(const dgMatrix<Type>&)")
            << "different fields"
            << abort(FatalError);
    }

    MatrixType::operator=(dgmv);
    source_ = dgmv.source_;
}


template<class Type>
void dgMatrix<Type>::operator=(const tmp<dgMatrix<Type> >& tdgmv)
{
    operator=(tdgmv());
    tdgmv.clear();
}


template<class Type>
void dgMatrix<Type>::negate()
{
    MatrixType::negate();
    source_.negate();
}


template<class Type>
void dgMatrix<Type>::operator+=(const dgMatrix<Type>& dgmv)
{
    // also add to the BoSSSMatrix
    dgmv.SyncFromBoSSS();
    cout << dgmv << endl;
    checkMethod(*this, dgmv, "+=");

    dimensions_ += dgmv.dimensions_;
    MatrixType::operator+=(dgmv);
    source_ += dgmv.source_;
    dgmv.SyncToBoSSS();

    // BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = GetBoSSSobject();
    // BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx2 = dgmv.GetBoSSSobject();

    // int rows = bosssMtx.GetNoOfRowsInBlock(0); // TODO multiple MPI processes
    // int cols = bosssMtx.GetNoOfColsInBlock(0); // TODO multiple MPI processes

    // bosssMtx.AccBlock(rows, cols, 1, &bosssMtx2);


}


template<class Type>
void dgMatrix<Type>::operator+=(const tmp<dgMatrix<Type> >& tdgmv)
{
    operator+=(tdgmv());
    tdgmv.clear();
}


template<class Type>
void dgMatrix<Type>::operator-=(const dgMatrix<Type>& dgmv)
{
    checkMethod(*this, dgmv, "+=");

    dimensions_ -= dgmv.dimensions_;
    MatrixType::operator-=(dgmv);
    source_ -= dgmv.source_;
}


template<class Type>
void dgMatrix<Type>::operator-=(const tmp<dgMatrix<Type> >& tdgmv)
{
    operator-=(tdgmv());
    tdgmv.clear();
}


template<class Type>
void dgMatrix<Type>::operator*=
(
    const dimensionedDgScalar& ds
)
{
    dimensions_ *= ds.dimensions();
    MatrixType::operator*=(ds.value());
    source_ *= ds.value();
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

template<class Type>
void checkMethod
(
    const dgMatrix<Type>& dgm1,
    const dgMatrix<Type>& dgm2,
    const char* op
)
{
    if (&dgm1.psi() != &dgm2.psi())
    {
        FatalErrorIn
        (
            "checkMethod(const dgMatrix<Type>&, const dgMatrix<Type>&)"
        )   << "incompatible fields for operation "
            << endl << "    "
            << "[" << dgm1.psi().name() << "] "
            << op
            << " [" << dgm2.psi().name() << "]"
            << abort(FatalError);
    }

    if (dimensionSet::debug && dgm1.dimensions() != dgm2.dimensions())
    {
        FatalErrorIn
        (
            "checkMethod(const dgMatrix<Type>&, const dgMatrix<Type>&)"
        )   << "incompatible dimensions for operation "
            << endl << "    "
            << "[" << dgm1.psi().name() << dgm1.dimensions()/dimArea << " ] "
            << op
            << " [" << dgm2.psi().name() << dgm2.dimensions()/dimArea << " ]"
            << abort(FatalError);
    }
}


template<class Type>
typename dgMatrix<Type>::SolverPerfType solve
(
    dgMatrix<Type>& dgm,
    Istream& solverControls
)
{
    return dgm.solve(solverControls);
}

template<class Type>
typename dgMatrix<Type>::SolverPerfType solve
(
    const tmp<dgMatrix<Type> >& tdgm,
    Istream& solverControls
)
{
    typename dgMatrix<Type>::SolverPerfType solverPerf =
        const_cast<dgMatrix<Type>&>(tdgm()).solve(solverControls);

    tdgm.clear();
    return solverPerf;
}


template<class Type>
typename dgMatrix<Type>::SolverPerfType solve(dgMatrix<Type>& dgm)
{
    return dgm.solve();
}

template<class Type>
typename dgMatrix<Type>::SolverPerfType solve(const tmp<dgMatrix<Type> >& tdgm)
{
    typename dgMatrix<Type>::SolverPerfType solverPerf =
        const_cast<dgMatrix<Type>&>(tdgm()).solve();

    tdgm.clear();
    return solverPerf;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const dgMatrix<Type>& A,
    const dgMatrix<Type>& B
)
{
    checkMethod(A, B, "+");
    tmp<dgMatrix<Type> > tC(new dgMatrix<Type>(A));
    tC() += B;
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const tmp<dgMatrix<Type> >& tA,
    const dgMatrix<Type>& B
)
{
    checkMethod(tA(), B, "+");
    tmp<dgMatrix<Type> > tC(tA.ptr());
    tC() += B;
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const dgMatrix<Type>& A,
    const tmp<dgMatrix<Type> >& tB
)
{
    checkMethod(A, tB(), "+");
    tmp<dgMatrix<Type> > tC(tB.ptr());
    tC() += A;
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator+
(
    const tmp<dgMatrix<Type> >& tA,
    const tmp<dgMatrix<Type> >& tB
)
{
    checkMethod(tA(), tB(), "+");
    tmp<dgMatrix<Type> > tC(tA.ptr());
    tC() += tB();
    tB.clear();
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const dgMatrix<Type>& A
)
{
    tmp<dgMatrix<Type> > tC(new dgMatrix<Type>(A));
    tC().negate();
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const tmp<dgMatrix<Type> >& tA
)
{
    tmp<dgMatrix<Type> > tC(tA.ptr());
    tC().negate();
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const dgMatrix<Type>& A,
    const dgMatrix<Type>& B
)
{
    checkMethod(A, B, "-");
    tmp<dgMatrix<Type> > tC(new dgMatrix<Type>(A));
    tC() -= B;
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const tmp<dgMatrix<Type> >& tA,
    const dgMatrix<Type>& B
)
{
    checkMethod(tA(), B, "-");
    tmp<dgMatrix<Type> > tC(tA.ptr());
    tC() -= B;
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const dgMatrix<Type>& A,
    const tmp<dgMatrix<Type> >& tB
)
{
    checkMethod(A, tB(), "-");
    tmp<dgMatrix<Type> > tC(tB.ptr());
    tC() -= A;
    tC().negate();
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator-
(
    const tmp<dgMatrix<Type> >& tA,
    const tmp<dgMatrix<Type> >& tB
)
{
    checkMethod(tA(), tB(), "-");
    tmp<dgMatrix<Type> > tC(tA.ptr());
    tC() -= tB();
    tB.clear();
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const dgMatrix<Type>& A,
    const dgMatrix<Type>& B
)
{
    checkMethod(A, B, "==");
    return (A - B);
}


template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const tmp<dgMatrix<Type> >& tA,
    const dgMatrix<Type>& B
)
{
    checkMethod(tA(), B, "==");
    return (tA - B);
}


template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const dgMatrix<Type>& A,
    const tmp<dgMatrix<Type> >& tB
)
{
    checkMethod(A, tB(), "==");
    return (A - tB);
}


template<class Type>
tmp<dgMatrix<Type> > operator==
(
    const tmp<dgMatrix<Type> >& tA,
    const tmp<dgMatrix<Type> >& tB
)
{
    checkMethod(tA(), tB(), "==");
    return (tA - tB);
}


template<class Type>
tmp<dgMatrix<Type> > operator*
(
    const dimensioned<scalar>& ds,
    const dgMatrix<Type>& A
)
{
    tmp<dgMatrix<Type> > tC(new dgMatrix<Type>(A));
    tC() *= ds;
    return tC;
}


template<class Type>
tmp<dgMatrix<Type> > operator*
(
    const dimensioned<scalar>& ds,
    const tmp<dgMatrix<Type> >& tA
)
{
    tmp<dgMatrix<Type> > tC(tA.ptr());
    tC() *= ds;
    return tC;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<(Ostream& os, const dgMatrix<Type>& dgm)
{
    os  << static_cast<const typename dgMatrix<Type>::MatrixType&>(dgm) << nl
        << dgm.dimensions_ << nl
        << dgm.source_ << nl
        // << dgm.internalCoeffs_ << nl
        // << dgm.boundaryCoeffs_ << endl
        ;

    tmp<dgMatrix<Type> > tdgm
    (
        dgm
    );
    dgMatrix<Type>& dgmCopy = tdgm();

    dgmCopy.print(os);

    // os.check("Ostream& operator<<(Ostream&, dgMatrix<Type>&");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * Solvers * * * * * * * * * * * * * * * * * //

#include "dgMatrixSolve.C"

// ************************************************************************* //
