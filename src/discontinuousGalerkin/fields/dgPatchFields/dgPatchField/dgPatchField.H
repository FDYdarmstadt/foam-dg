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
    Foam::dgPatchField

Description
    Abstract base class for Discontinuous Galerkin patch fields

    Note that the field does not carry values

SourceFiles
    dgPatchField.C
    newDgPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef dgPatchField_H
#define dgPatchField_H

#include "tmp.H"
#include "dgPatch.H"
#include "DimensionedField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes

class dictionary;
class cellMesh;

// Forward declaration of friend functions and operators

template<class Type>
class dgPatchField;

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;

template<class Type>
Ostream& operator<<(Ostream&, const dgPatchField<Type>&);


/*---------------------------------------------------------------------------*\
                        Class dgPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class dgPatchField
:
    public refCount
{
    // Private data

        //- Reference to patch
        const dgPatch& patch_;

        //- Reference to internal field
        const DimensionedField<Type, cellMesh>& internalField_;

        //- Update index used so that updateCoeffs is called only once during
        //  the construction of the matrix
        bool updated_;

        //- Optional patch type, used to allow specified boundary conditions
        //  to be applied to constraint patches by providing the constraint
        //  patch type as 'patchType'
        word patchType_;


public:

    typedef dgPatch Patch;

    //- Runtime type information
    TypeName("dgPatchField");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            dgPatchField,
            patch,
            (
                const dgPatch& p,
                const DimensionedField<Type, cellMesh>& iF
            ),
            (p, iF)
        );

        declareRunTimeSelectionTable
        (
            tmp,
            dgPatchField,
            dictionary,
            (
                const dgPatch& p,
                const DimensionedField<Type, cellMesh>& iF,
                const dictionary& dict
            ),
            (p, iF, dict)
        );


    // Constructors

        //- Construct from patch and internal field
        dgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Construct from patch, internal field and dictionary
        dgPatchField
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&,
            const dictionary&
        );

        //- Construct as copy
        dgPatchField(const dgPatchField<Type>&);

        //- Construct and return a clone
        virtual tmp<dgPatchField<Type> > clone() const
        {
            return tmp<dgPatchField<Type> >(new dgPatchField<Type>(*this));
        }

        //- Construct as copy setting internal field reference
        dgPatchField
        (
            const dgPatchField<Type>&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<dgPatchField<Type> > clone
        (
            const DimensionedField<Type, cellMesh>& iF
        ) const
        {
            return tmp<dgPatchField<Type> >(new dgPatchField<Type>(*this, iF));
        }


    // Selectors

        //- Return a pointer to a new patchField created on freestore given
        //  patch and internal field
        //  (does not set the patch field values)
        static tmp<dgPatchField<Type> > New
        (
            const word&,
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&
        );

        //- Return a pointer to a new patchField created on freestore
        //  from dictionary
        static tmp<dgPatchField<Type> > New
        (
            const dgPatch&,
            const DimensionedField<Type, cellMesh>&,
            const dictionary&
        );


    //- Destructor
    virtual ~dgPatchField<Type>()
    {}


    // Member functions

        // Access

            //- Return size
            virtual label size() const
            {
                return patch_.size();
            }
    
            //- Return local objectRegistry
            const objectRegistry& db() const;

            //- Return patch
            const dgPatch& patch() const
            {
                return patch_;
            }

            //- Return dimensioned internal field reference
            const DimensionedField<Type, cellMesh>&
            dimensionedInternalField() const
            {
                return internalField_;
            }

            //- Return internal field reference
            const Field<Type>& internalField() const
            {
                return internalField_;
            }

            //- Return true if this patch field fixes a value.
            //  Needed to check if a level has to be specified while solving
            //  Poissons equations.
            virtual bool fixesValue() const
            {
                return false;
            }

            //- Return true if this patch field is coupled
            virtual bool coupled() const
            {
                return false;
            }

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            //  Sets Updated to true
            virtual void updateCoeffs()
            {
                updated_ = true;
            }

            //- Initialise the evaluation of the patch field
            virtual void initEvaluate
            (
                const Pstream::commsTypes commsType = Pstream::blocking
            )
            {}

            //- Evaluate the patch field, sets Updated to false
            virtual void evaluate
            (
                const Pstream::commsTypes commsType = Pstream::blocking
            );


        // I-O

            //- Write
            virtual void write(Ostream&) const;

            //- Helper function to write the keyword and entry only if the
            //  values are not equal. The value is then output as value2
            template<class EntryType>
            void writeEntryIfDifferent
            (
                Ostream& os,
                const word& entryName,
                const EntryType& value1,
                const EntryType& value2
            ) const;


        // Check

            //- Check dgPatchField<Type> against given dgPatchField<Type>
            void check(const dgPatchField<Type>&) const;


    // Member operators

        virtual void operator=(const dgPatchField<Type>&){}
        virtual void operator+=(const dgPatchField<Type>&){}
        virtual void operator-=(const dgPatchField<Type>&){}
        virtual void operator*=(const dgPatchField<scalar>&){}
        virtual void operator/=(const dgPatchField<scalar>&){}

        virtual void operator+=(const Field<Type>&){}
        virtual void operator-=(const Field<Type>&){}

        virtual void operator*=(const Field<scalar>&){}
        virtual void operator/=(const Field<scalar>&){}

        virtual void operator=(const Type&){}
        virtual void operator+=(const Type&){}
        virtual void operator-=(const Type&){}
        virtual void operator*=(const scalar){}
        virtual void operator/=(const scalar){}


        // Force an assignment irrespective of form of patch

        virtual void operator==(const dgPatchField<Type>&){}
        virtual void operator==(const Field<Type>&){}
        virtual void operator==(const Type&){}


    // Ostream operator

        friend Ostream& operator<< <Type>(Ostream&, const dgPatchField<Type>&);
};


// This function is added as a hack to enable simple backward compatability
// with versions using referenceLevel in GeometricField
// HJ, 9/Mar/2017
template<class Type>
const dgPatchField<Type>
operator+
(
    const dgPatchField<Type>& ppf,
    const Type&
)
{
    return ppf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgPatchField.C"
#endif


#define addToDgPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField) \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    PatchTypeField, typePatchTypeField, patch                                 \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    PatchTypeField, typePatchTypeField, dictionary                            \
);


#define makeDgPatchTypeFieldTypeName(type)                                    \
                                                                              \
defineNamedTemplateTypeNameAndDebug(type, 0);


#define makeDgPatchFieldsTypeName(type)                                       \
                                                                              \
makeDgPatchTypeFieldTypeName(type##DgPatchScalarField);                       \
makeDgPatchTypeFieldTypeName(type##DgPatchVectorField);                       \


#define makeDgPatchTypeField(PatchTypeField, typePatchTypeField)              \
                                                                              \
defineTypeNameAndDebug(typePatchTypeField, 0);                                \
                                                                              \
addToPatchFieldRunTimeSelection                                               \
(                                                                             \
    PatchTypeField, typePatchTypeField                                        \
);


#define makeTemplateDgPatchTypeField(PatchTypeField, typePatchTypeField)      \
                                                                              \
defineNamedTemplateTypeNameAndDebug(typePatchTypeField, 0);                   \
                                                                              \
addToDgPatchFieldRunTimeSelection                                             \
(                                                                             \
    PatchTypeField, typePatchTypeField                                        \
);


#define makeDgPatchFields(type)                                               \
                                                                              \
makeTemplateDgPatchTypeField(dgPatchScalarField, type##DgPatchScalarField);   \
makeTemplateDgPatchTypeField(dgPatchVectorField, type##DgPatchVectorField);


#define makeDgPatchTypeFieldTypedefs(type)                                    \
                                                                              \
typedef type##DgPatchField<dgScalar> type##DgPatchScalarField;                \
typedef type##DgPatchField<dgVector> type##DgPatchVectorField;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
