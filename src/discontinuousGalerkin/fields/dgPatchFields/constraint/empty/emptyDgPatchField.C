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

#include "emptyDgPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
emptyDgPatchField<Type>::emptyDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(p, iF)
{}


template<class Type>
emptyDgPatchField<Type>::emptyDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary& dict
)
:
    dgPatchField<Type>(p, iF)
{
    if (!isType<emptyDgPatch>(p))
    {
        FatalIOErrorIn
        (
            "emptyDgPatchField<Type>::emptyDgPatchField\n"
            "(\n"
            "    const dgPatch& p,\n"
            "    const Field<Type>& field,\n"
            "    const dictionary& dict\n"
            ")\n",
            dict
        )   << "\n    patch type '" << p.type()
            << "' not constraint type '" << typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }
}


template<class Type>
emptyDgPatchField<Type>::emptyDgPatchField
(
    const emptyDgPatchField<Type>& ptf
)
:
    dgPatchField<Type>
    (
        ptf.patch(),
        ptf.dimensionedInternalField()
    )
{}


template<class Type>
emptyDgPatchField<Type>::emptyDgPatchField
(
    const emptyDgPatchField<Type>& ptf,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(ptf.patch(), iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
