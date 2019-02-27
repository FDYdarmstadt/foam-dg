/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
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
    Foam::fv::dgDdtScheme

Description
    Abstract base class for dgDdt schemes.

SourceFiles
    dgDdtScheme.C

\*---------------------------------------------------------------------------*/

#ifndef dgDdtScheme_H
#define dgDdtScheme_H

#include "tmp.H"
#include "dimensionedType.H"
#include "cellFieldsFwd.H"
//#include "volFieldsFwd.H"
//#include "surfaceFieldsFwd.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "dimensionedDgScalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class Type>
class dgMatrix;

class dgMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                           Class dgDdtScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class dgDdtScheme
:
    public refCount
{

protected:

    // Protected data

        const dgMesh& mesh_;


    // Private Member Functions

        //- Disallow copy construct
        dgDdtScheme(const dgDdtScheme&);

        //- Disallow default bitwise assignment
        void operator=(const dgDdtScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            dgDdtScheme,
            Istream,
            (const dgMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        dgDdtScheme(const dgMesh& mesh)
        :
            mesh_(mesh)
        {}

        //- Construct from mesh and Istream
        dgDdtScheme(const dgMesh& mesh, Istream&)
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return a pointer to a new dgDdtScheme created on freestore
        static tmp<dgDdtScheme<Type> > New
        (
            const dgMesh& mesh,
            Istream& schemeData
        );


    // Destructor

        virtual ~dgDdtScheme();


    // Member Functions

        //- Return mesh reference
        const dgMesh& mesh() const
        {
            return mesh_;
        }
//
//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcDdt
//        (
//            const dimensioned<Type>&
//        ) = 0;
//
//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcDdt
//        (
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;
//
//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcDdt
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;
//
//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcDdt
//        (
//            const volScalarField&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;
//
        virtual tmp<dgMatrix<Type> > dgmDdt
        (
            const GeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;

        virtual tmp<dgMatrix<Type> > dgmDdt
        (
            const dimensionedDgScalar&,
            const GeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;

        virtual tmp<dgMatrix<Type> > dgmDdt
        (
            const cellScalarField&,
            const GeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;
//
//
//        typedef GeometricField
//        <
//            typename flux<Type>::type,
//            fvsPatchField,
//            surfaceMesh
//        > fluxFieldType;
//
//        tmp<surfaceScalarField> dgcDdtPhiCoeff
//        (
//            const GeometricField<Type, dgPatchField, cellMesh>& U,
//            const fluxFieldType& phi,
//            const fluxFieldType& phiCorr
//        );
//
//        tmp<surfaceScalarField> dgcDdtPhiCoeff
//        (
//            const GeometricField<Type, dgPatchField, cellMesh>& U,
//            const fluxFieldType& phi
//        );
//
//        virtual tmp<fluxFieldType> dgcDdtPhiCorr
//        (
//            const volScalarField& rA,
//            const GeometricField<Type, dgPatchField, cellMesh>& U,
//            const fluxFieldType& phi
//        ) = 0;
//
//        tmp<surfaceScalarField> dgcDdtPhiCoeff
//        (
//            const volScalarField& rho,
//            const GeometricField<Type, dgPatchField, cellMesh>& rhoU,
//            const fluxFieldType& phi
//        );
//
//        virtual tmp<fluxFieldType> dgcDdtPhiCorr
//        (
//            const volScalarField& rA,
//            const volScalarField& rho,
//            const GeometricField<Type, dgPatchField, cellMesh>& U,
//            const fluxFieldType& phi
//        ) = 0;
//
//
//        // Member functions for the new time consistent formulation
//        virtual tmp<fluxFieldType> dgcDdtConsistentPhiCorr
//        (
//            const GeometricField<Type, fvsPatchField, surfaceMesh>& faceU,
//            const GeometricField<Type, dgPatchField, cellMesh>& U,
//            const surfaceScalarField& rAUf
//        ) = 0;
//
//
//        virtual tmp<surfaceScalarField> meshPhi
//        (
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeDgDdtTypeScheme(SS, Type)                                          \
                                                                               \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                                                               \
dgDdtScheme<Type>::addIstreamConstructorToTable<SS<Type> >                       \
    add##SS##Type##IstreamConstructorToTable_;


#define makeDgDdtScheme(SS)                                                    \
                                                                               \
makeDgDdtTypeScheme(SS, dgScalar)                                                \
                                                                               \
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgDdtScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
