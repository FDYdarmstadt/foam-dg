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
}


template<class Type>
homogenousDgPatchField<Type>::homogenousDgPatchField
(
    const homogenousDgPatchField& zgpf
)
:
    dgPatchField<Type>(zgpf)
{
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
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template<class Type>
//FieldField<Field, scalar> homogenousDgPatchField<Type>::valueInternalCoeffs
//(
////    const tmp<dgScalarField>& dsf
//) const
//{
//    return
//    (
//        new (this->size(), pTraits<dgScalar>::zero)
//    );
//}
//
//
//template<class Type>
//tmp<dgScalarField> homogenousDgPatchField<Type>::valueBoundaryCoeffs
//(
////    const tmp<dgScalarField>& dsf
//) const
//{
//
//    const dgMesh& mesh = this->patch().boundaryMesh().mesh();
//
//    tmp<dgScalarField> tsourceCoeffs
//        (
//            new dgScalarField(mesh.size(), dgScalar(0.0))
//        );
//    dgScalarField& sourceCoeffs = tsourceCoeffs();
//
//    Field<Type> dsf = *this;
//
//    forAll (sourceCoeffs, faceI)
//    {
//        forAll (sourceCoeffs[faceI], modI)
//        {
//            // THIS SHOULD BE CHECKED - OK FOR ZEROGRADIENT
//            sourceCoeffs[faceI][modI] = 0.0;//mag(dsf[faceI][modI]);
//        }
//    }
//
//    return tsourceCoeffs;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
