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
    length_(readLabel(lookup("polynomialOrder")) + 1),
    polynomialsPtr_(NULL),
    quadratureOrder_(readLabel(lookup("quadratureOrder")))
{
    // Create polynomials object
    polynomialsPtr_ = new dgPolynomials(readLabel(lookup("polynomialOrder")) + 1);


    Info<< "Quadrature order: " << quadratureOrder_ << ", polynomial order: " <<
    length_ - 1 << endl;
}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

autoPtr<dgBase> dgBase::New
(
    const polyMesh& mesh
)
{
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
scalarField dgBase::evaluate
(
    const vector localCoords
) const
{
    return polynomialsPtr_->evaluate(localCoords);
}


scalar dgBase::evaluate
(
    const dgScalar value,
    const vector localCoords
) const
{
    return polynomialsPtr_->evaluate(value, localCoords);
}


vector dgBase::evaluate
(
    const dgVector value,
    const vector localCoords
) const
{
    return polynomialsPtr_->evaluate(value, localCoords);
}


scalarField dgBase::gradEvaluate
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
