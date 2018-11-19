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
    homogenousDgPatchField

Description
    Homogenous Discontinuous Galerkin patch field

Author
    Hrvoje Jasak.  All rights reserved

SourceFiles
    homogenousDgPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef homogenousDgPatchField_H
#define homogenousDgPatchField_H

#include "dgPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class homogenousDgPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class homogenousDgPatchField
:
    public dgPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("homogenous");


    // Constructors

        //- Construct from patch and internal field
        homogenousDgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Construct from patch, internal field and dictionary
        homogenousDgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&,
            const dictionary&
        );

        //- Construct as copy
        homogenousDgPatchField
        (
            const homogenousDgPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<dgPatchField<Type> > clone() const
        {
            return tmp<dgPatchField<Type> >
            (
                new homogenousDgPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        homogenousDgPatchField
        (
            const homogenousDgPatchField<Type>&,
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
                new homogenousDgPatchField<Type>(*this, iF)
            );
        }


    //- Destructor
    virtual ~homogenousDgPatchField()
    {}


    // Member functions

        // Evaluation functions

//            virtual tmp<dgScalarField> valueInternalCoeffs
//            (
////                const tmp<dgScalarField>&
//            ) const;

            virtual tmp<dgScalarField> valueBoundaryCoeffs
            (
//                const tmp<dgScalarField>&
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "homogenousDgPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
