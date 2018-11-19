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

#include "IOobject.H"
#include "dictionary.H"
#include "dgMesh.H"
#include "GeometricField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::dgPatchField<Type>::dgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false),
    patchType_(word::null)
{
//    Info << "DG PATCH FIELD 1 " << this->patch().name() << endl;
}


template<class Type>
Foam::dgPatchField<Type>::dgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary& dict
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false),
    patchType_(dict.lookupOrDefault<word>("patchType", word::null))
{
//    Info << "DG PATCH FIELD 2" << endl;

    if (dict.found("value"))
    {
        dgPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
//    else if (!valueRequired)
//    {
//        dgPatchField<Type>::operator=(pTraits<Type>::zero);
//    }
    else
    {
        FatalIOErrorIn
        (
            "dgPatchField<Type>::dgPatchField"
            "("
            "const dgPatch& p,"
            "const DimensionedField<Type, cellMesh>& iF"
            "const dictionary& dict"
            ")",
            dict
        ) << "Essential entry 'value' missing"
          << exit(FatalIOError);
    }

    Info << "VALUE OF DIRI FIELD: " << *this << endl;
}


template<class Type>
Foam::dgPatchField<Type>::dgPatchField
(
    const dgPatchField<Type>& ptf
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(ptf.internalField_),
    updated_(false),
    patchType_(ptf.patchType_)
{
//    Info << "DG PATCH FIELD 3" << endl;
}


template<class Type>
Foam::dgPatchField<Type>::dgPatchField
(
    const dgPatchField<Type>& ptf,
    const DimensionedField<Type, cellMesh>& iF
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(iF),
    updated_(false),
    patchType_(ptf.patchType_)
{
//    Info << "DG PATCH FIELD 4 " << this->patch().name() << endl;
}


template<class Type>
Foam::dgPatchField<Type>::dgPatchField
(
    const dgPatchField<Type>& ptf,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary& dict
)
:
    Field<Type>(ptf),
    patch_(ptf),
    internalField_(iF),
    updated_(false),
    patchType_(dict.lookupOrDefault<word>("patchType", word::null))
{
//    Info << "DG PATCH FIELD 5" << endl;

    if (dict.found("value"))
    {
        dgPatchField<Type>::operator=
        (
            Field<Type>("value", dict, ptf.size())
        );
    }
//    else if (!valueRequired)
//    {
//        dgPatchField<Type>::operator=(pTraits<Type>::zero);
//    }
    else
    {
        FatalIOErrorIn
        (
            "dgPatchField<Type>::dgPatchField"
            "("
            "const dgPatch& p,"
            "const DimensionedField<Type, cellMesh>& iF"
            "const dictionary& dict"
            ")",
            dict
        ) << "Essential entry 'value' missing"
          << exit(FatalIOError);
    }

    Info << "VALUE OF DIRI FIELD: " << *this << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const Foam::objectRegistry& Foam::dgPatchField<Type>::db() const
{
    return internalField_.db();
}


template<class Type>
void Foam::dgPatchField<Type>::write(Ostream& os) const
{
    os.writeKeyword("type") << type() << token::END_STATEMENT << nl;

    if (patchType_.size())
    {
        os.writeKeyword("patchType") << patchType_
            << token::END_STATEMENT << nl;
    }
}


template<class Type>
template<class EntryType>
void Foam::dgPatchField<Type>::writeEntryIfDifferent
(
    Ostream& os,
    const word& entryName,
    const EntryType& value1,
    const EntryType& value2
) const
{
    if (value1 != value2)
    {
        os.writeKeyword(entryName) << value2 << token::END_STATEMENT << nl;
    }
}


template<class Type>
void Foam::dgPatchField<Type>::check(const dgPatchField<Type>& ptf) const
{
    if (&patch_ != &(ptf.patch_))
    {
        FatalErrorIn("PatchField<Type>::check(const dgPatchField<Type>&)")
            << "different patches for dgPatchField<Type>s"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

//template<class Type>
//void Foam::dgPatchField<Type>::operator=
//(
//    const UList<Type>& ul
//)
//{
//    Field<Type>::operator=(ul);
//}


template<class Type>
void Foam::dgPatchField<Type>::operator=
(
    const dgPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator=(ptf);
}


template<class Type>
void Foam::dgPatchField<Type>::operator=
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


template<class Type>
void Foam::dgPatchField<Type>::operator==
(
    const dgPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf);
}


template<class Type>
void Foam::dgPatchField<Type>::operator==
(
    const Field<Type>& tf
)
{
    Field<Type>::operator=(tf);
}


template<class Type>
void Foam::dgPatchField<Type>::operator==
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Foam::Ostream& Foam::operator<<(Ostream& os, const dgPatchField<Type>& ptf)
{
    ptf.write(os);

    os.check("Ostream& operator<<(Ostream&, const dgPatchField<Type>&");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#   include "newDgPatchField.C"

// ************************************************************************* //
