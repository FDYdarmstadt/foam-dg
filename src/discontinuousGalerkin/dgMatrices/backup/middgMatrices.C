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

#include "dgMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

//defineTemplateTypeNameAndDebug(dgZeroScalarMatrix, 0);
//defineTemplateTypeNameAndDebug(dgOneScalarMatrix, 0);
//defineTemplateTypeNameAndDebug(dgTwoScalarMatrix, 0);
//defineTemplateTypeNameAndDebug(dgThreeScalarMatrix, 0);
//defineTemplateTypeNameAndDebug(dgFourScalarMatrix, 0);


//BUG!! defineTemplateTypeNameAndDebug(dgVectorMatrix, 0);

defineRunTimeSelectionTable(dgMatrices<Type>, dgMatrices);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
dgMatrices<Type>::dgMatrices
(
//    const GeometricField<Type, dgPatchField, cellMesh>& psi,
//    const dimensionSet& ds
)
:
{}


template<class Type>
dgMatrices<Type>::dgMatrices(const dgMatrices<Type>& dgm)
:
{}



// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
autoPtr<dgMatrices<Type> > dgMatrices<Type>::New
(
    const GeometricField<Type, dgPatchField, cellMesh>& psi,
    const dimensionSet& ds,
    const int& blockSize
)
{
    word matrixName;

    if (blockSize == 0)
    {
        matrixName = "dgZeroScalarMatrix";
    }
    else if (blockSize == 1)
    {
        matrixName = "dgOneScalarMatrix";
    }
    else if (blockSize == 2)
    {
        matrixName = "dgTwoScalarMatrix";
    }
    else if (blockSize == 3)
    {
        matrixName = "dgThreeScalarMatrix";
    }
    else if (blockSize == 4)
    {
        matrixName = "dgFourScalarMatrix";
    }

    dgMatricesConstructorTable::iterator cstrIter =
            dgMatricesConstructorTablePtr_->find(matrixName);

    if (cstrIter == dgMatricesConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "dgMatrices::New(const volVectorField&, "
            "const surfaceScalarField&, transportModel&)"
        )   << "Unknown dgMatrices size " << blockSize
            << endl << endl
            << "Valid dgMatrices sizes are :" << endl
            << dgMatricesConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<dgMatrices>
    (
        cstrIter()(psi, ds)
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
