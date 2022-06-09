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

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
BlockSolverPerformance<VectorN<scalar, Type::coeffLength> >
dgMatrix<Type>::solve(const dictionary& solverControls)
{
    Info << "DG MATRIX SOLVE" << endl;

    if (debug)
    {
        Info<< "dgMatrix<Type>::solve(const dictionary&) : "
               "solving dgMatrix<Type>"
            << endl;
    }

    Field<Type> source = this->source();

    Field<Type>& psiI = const_cast<Field<Type>&>(psi_.internalField());

    Field<SubType> intField(psi_.internalField().size());
    Field<SubType> sourceField(source.size());

    forAll(intField, cellI)
    {
        intField[cellI] = psi_.internalField()[cellI];
        sourceField[cellI] = source[cellI];
    }

//    Info << "SOLVER PERFORMANCE" << endl;

    // Solver call
    SolverPerfType solverPerf =
        SolverType::New
        (
            psi_.name(),
//            static_cast<typename dgMatrix<Type>::MatrixType&>(*this),
            *this,
            solverControls
        )->solve(intField, sourceField);
//        )->solve(psi_.internalField(), this->source());

//    Info << "SOLVER PRINT" << endl;
    solverPerf.print();

//    psi().correctBoundaryConditions();

    Info << "SOLVER OUT" << endl;

    forAll(intField, cellI)
    {
        psiI[cellI] = intField[cellI];
    }

    return solverPerf;
}


template<class Type>
typename dgMatrix<Type>::SolverPerfType
dgMatrix<Type>::dgSolver::solve()
{
    return solve
    (
        dgMat_.psi().mesh().solutionDict().solverDict
        (
            dgMat_.psi().name()
        )
    );
}


template<class Type>
typename dgMatrix<Type>::SolverPerfType
dgMatrix<Type>::solve()
{
    return solve
    (
        this->psi().mesh().solutionDict().solverDict
        (
            this->psi().name()
        )
    );
}


// Return the matrix residual
template<class Type>
tmp<Field<Type> > dgMatrix<Type>::residual() const
{
    return MatrixType::residual(psi_, source_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
