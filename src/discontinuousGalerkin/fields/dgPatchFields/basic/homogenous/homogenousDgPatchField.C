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

#include "homogenousDgPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
homogenousDgPatchField<Type>::homogenousDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(p, iF)
{
//    Info << "HOMOGENOUS 1" << endl;
}


template<class Type>
homogenousDgPatchField<Type>::homogenousDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary&
)
:
    dgPatchField<Type>(p, iF)
{
//    Info << "HOMOGENOUS 2" << endl;
}


template<class Type>
homogenousDgPatchField<Type>::homogenousDgPatchField
(
    const homogenousDgPatchField& zgpf
)
:
    dgPatchField<Type>(zgpf)
{
//    Info << "HOMOGENOUS 3" << endl;
}


template<class Type>
homogenousDgPatchField<Type>::homogenousDgPatchField
(
    const homogenousDgPatchField& zgpf,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(zgpf, iF)
{
//    Info << "HOMOGENOUS 4" << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template<class Type>
//tmp<dgScalarField> homogenousDgPatchField<Type>::valueInternalCoeffs
//(
////    const tmp<dgScalarField>& dsf
//) const
//{
//    return valueInternalCoeffs();
//}
//
//
//template<class Type>
//tmp<dgScalarField> homogenousDgPatchField<Type>::valueBoundaryCoeffs
//(
////    const tmp<dgScalarField>& dsf
//) const
//{
////    return ;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
