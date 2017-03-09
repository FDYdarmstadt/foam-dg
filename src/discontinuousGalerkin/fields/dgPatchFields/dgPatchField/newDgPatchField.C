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


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class Type>
Foam::tmp<Foam::dgPatchField<Type> > Foam::dgPatchField<Type>::New
(
    const word& patchFieldType,
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF
)
{
    if (debug)
    {
        Info<< "dgPatchField<Type>::New(const word&, const dgPatch&, "
               "const DimensionedField<Type, cellMesh>&) : patchFieldType="
            << patchFieldType
            << endl;
    }

    typename patchConstructorTable::iterator cstrIter =
        patchConstructorTablePtr_->find(patchFieldType);

    if (cstrIter == patchConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "dgPatchField<Type>::New(const word&, const dgPatch&, "
            "const DimensionedField<Type, cellMesh>&)"
        )   << "Unknown patch field type " << patchFieldType
            << " for field " << iF.name() << " on patch " << p.name()
            << nl << nl
            << "Valid patchField types are :" << endl
            << patchConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    typename patchConstructorTable::iterator patchTypeCstrIter =
        patchConstructorTablePtr_->find(p.type());

    if (patchTypeCstrIter != patchConstructorTablePtr_->end())
    {
        return patchTypeCstrIter()(p, iF);
    }
    else
    {
        return cstrIter()(p, iF);
    }
}


template<class Type>
Foam::tmp<Foam::dgPatchField<Type> > Foam::dgPatchField<Type>::New
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary& dict
)
{
    word patchFieldType(dict.lookup("type"));

    if (debug)
    {
        Info<< "dgPatchField<Type>::New(const dgPatch&, "
               "const DimensionedField<Type, cellMesh>&, "
               "const dictionary&) : patchFieldType = "  << patchFieldType
            << endl;
    }

    typename dictionaryConstructorTable::iterator cstrIter
        = dictionaryConstructorTablePtr_->find(patchFieldType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalIOErrorIn
            (
                "dgPatchField<Type>::New(const dgPatch&, "
                "const DimensionedField<Type, cellMesh>&, "
                "const dictionary&)",
                dict
            )   << "Unknown patch field type " << patchFieldType
                << " for field " << iF.name() << " on patch " << p.name()
                << " for patch type " << p.type() << endl << endl
                << "Valid patch field types are :" << endl
                << dictionaryConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }
    }

    if
    (
       !dict.found("patchType")
     || word(dict.lookup("patchType")) != p.type()
    )
    {
        typename dictionaryConstructorTable::iterator patchTypeCstrIter
            = dictionaryConstructorTablePtr_->find(p.type());

        if
        (
            patchTypeCstrIter != dictionaryConstructorTablePtr_->end()
         && patchTypeCstrIter() != cstrIter()
        )
        {
            FatalIOErrorIn
            (
                "dgPatchField<Type>::New(const dgPatch&, "
                "const DimensionedField<Type, cellMesh>&, "
                "const dictionary&)",
                dict
            )   << "Inconsistent patch and patchField types for field "
                << iF.name() << " on patch " << p.name() << "\n"
                << "    patch type " << p.type()
                << " and patchField type " << patchFieldType
                << exit(FatalIOError);
        }
    }

    return cstrIter()(p, iF, dict);
}


// ************************************************************************* //
