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
    if (debug)
    {
        InfoIn("dgMatrix<Type>::dgMatrix(const dgMatrix<Type>&)")
            << "copying dgMatrix<Type> for field " << psi_.name()
            << endl;
    }
}


template<class Type>
dgMatrix<Type>::~dgMatrix()
{
    if (debug)
    {
        InfoIn("dgMatrix<Type>::~dgMatrix<Type>()")
            << "destroying dgMatrix<Type> for field " << psi_.name()
            << endl;
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
    checkMethod(*this, dgmv, "+=");

    dimensions_ += dgmv.dimensions_;
    MatrixType::operator+=(dgmv);
    source_ += dgmv.source_;
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
        << dgm.internalCoeffs_ << nl
        << dgm.boundaryCoeffs_ << endl;

    os.check("Ostream& operator<<(Ostream&, dgMatrix<Type>&");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * Solvers * * * * * * * * * * * * * * * * * //

#include "dgMatrixSolve.C"

// ************************************************************************* //
