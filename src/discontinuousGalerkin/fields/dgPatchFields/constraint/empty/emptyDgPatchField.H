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

Class
    Foam::emptyDgPatchField

Description
    Foam::emptyDgPatchField

SourceFiles
    emptyDgPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef emptyDgPatchField_H
#define emptyDgPatchField_H

#include "dgPatchField.H"
#include "emptyDgPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class emptyDgPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class emptyDgPatchField
:
    public dgPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(emptyDgPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        emptyDgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Construct from patch, internal field and dictionary
        emptyDgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&,
            const dictionary&
        );

        //- Construct as copy
        emptyDgPatchField
        (
            const emptyDgPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<dgPatchField<Type> > clone() const
        {
            return tmp<dgPatchField<Type> >
            (
                new emptyDgPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        emptyDgPatchField
        (
            const emptyDgPatchField<Type>&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<dgPatchField<Type> > clone
        (
            const DimensionedField<Type, cellMesh>& iF
        ) const
        {
            return tmp<dgPatchField<Type> >
            (
                new emptyDgPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return size
            virtual label size() const
            {
                return 0;
            }


        // Evaluation functions
//            virtual FieldField<Field, scalar> valueInternalCoeffs () const
//            {
//                FieldField<Field, scalar> nullField
//                    (internalField().mesh().size());

//                return fvPatchField::valueInternalCoeffs();
//                nullField;
//            }

//            virtual tmp<dgScalarField> valueBoundaryCoeffs () const
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "emptyDgPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
