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
    dirichletDgPatchField

Description
    Dirichlet Discontinuous Galerkin patch field

Author
    Hrvoje Jasak.  All rights reserved

SourceFiles
    dirichletDgPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef dirichletDgPatchField_H
#define dirichletDgPatchField_H

#include "dgPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class dirichletDgPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class dirichletDgPatchField
:
    public dgPatchField<Type>
{
    // Private data

        //- Penalty factor Eta pointer
        mutable scalarField* etaPtr_;

        //- Calculate penalty factor
        void calcEta();


public:

    //- Runtime type information
    TypeName("dirichlet");


    // Constructors

        //- Construct from patch and internal field
        dirichletDgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Construct from patch, internal field and dictionary
        dirichletDgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&,
            const dictionary&
        );

        //- Construct as copy
        dirichletDgPatchField
        (
            const dirichletDgPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<dgPatchField<Type> > clone() const
        {
            return tmp<dgPatchField<Type> >
            (
                new dirichletDgPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        dirichletDgPatchField
        (
            const dirichletDgPatchField<Type>&,
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
                new dirichletDgPatchField<Type>(*this, iF)
            );
        }


    //- Destructor
    virtual ~dirichletDgPatchField()
    {}


    // Member functions

        // Access

            //- Return true if this patch field fixes a value.
            virtual bool fixesValue() const
            {
                return true;
            }

        //-Write
        virtual void write(Ostream&) const;

        // Evaluation functions

//            virtual tmp<dgScalarField> valueInternalCoeffs
            virtual FieldField<Field, scalar> valIntCoeffsLaplace
            (
//                const tmp<dgScalarField>&
            ) const;

            virtual tmp<dgScalarField> valBouCoeffsLaplace
            (
//                const tmp<dgScalarField>&
            ) const;


//            virtual tmp<dgScalarField> valueInternalCoeffs
            virtual FieldField<Field, scalar> valIntCoeffsDiv
            (
                const dgPatchField<dgVector>&
            ) const;

            virtual tmp<dgScalarField> valBouCoeffsDiv
            (
//                const tmp<dgScalarField>&
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dirichletDgPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
