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
    Foam::fv::EulerDdtScheme

Description
    Basic first-order Euler implicit/explicit ddt using only the current and
    previous time-step values.

SourceFiles
    EulerDdtScheme.C

\*---------------------------------------------------------------------------*/

#ifndef EulerDdtScheme_H
#define EulerDdtScheme_H

#include "dgDdtScheme.H"
#include "dimensionedDgScalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                       Class EulerDdtScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class EulerDdtScheme
:
    public dgDdtScheme<Type>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        EulerDdtScheme(const EulerDdtScheme&);

        //- Disallow default bitwise assignment
        void operator=(const EulerDdtScheme&);


public:

    //- Runtime type information
    TypeName("Euler");


    // Constructors

        //- Construct from mesh
        EulerDdtScheme(const dgMesh& mesh)
        :
            dgDdtScheme<Type>(mesh)
        {}

        //- Construct from mesh and Istream
        EulerDdtScheme(const dgMesh& mesh, Istream& is)
        :
            dgDdtScheme<Type>(mesh, is)
        {}


    // Member Functions

//        //- Return mesh reference
//        const dgMesh& mesh() const
//        {
//            return dg::dgDdtScheme<Type>::mesh();
//        }
//
//        tmp<GeometricField<Type, dgPatchField, dgMesh> > dgcDdt
//        (
//            const dimensioned<Type>&
//        );
//
//        tmp<GeometricField<Type, dgPatchField, dgMesh> > dgcDdt
//        (
//            const GeometricField<Type, dgPatchField, dgMesh>&
//        );
//
//        tmp<GeometricField<Type, dgPatchField, dgMesh> > dgcDdt
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, dgMesh>&
//        );
//
//        tmp<GeometricField<Type, dgPatchField, dgMesh> > dgcDdt
//        (
//            const volScalarField&,
//            const GeometricField<Type, dgPatchField, dgMesh>&
//        );
//
        tmp<dgMatrix<Type> > dgmDdt
        (
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        );

        tmp<dgMatrix<Type> > dgmDdt
        (
            const dimensionedDgScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        );

        tmp<dgMatrix<Type> > dgmDdt
        (
            const cellScalarField&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        );
//
//        typedef typename ddtScheme<Type>::fluxFieldType fluxFieldType;
//
//        tmp<fluxFieldType> dgcDdtPhiCorr
//        (
//            const volScalarField& rA,
//            const GeometricField<Type, dgPatchField, dgMesh>& U,
//            const fluxFieldType& phi
//        );
//
//        tmp<fluxFieldType> dgcDdtPhiCorr
//        (
//            const volScalarField& rA,
//            const volScalarField& rho,
//            const GeometricField<Type, dgPatchField, dgMesh>& U,
//            const fluxFieldType& phi
//        );
//
//
//        // Member functions for the new time consistent formulation
//        tmp<fluxFieldType> dgcDdtConsistentPhiCorr
//        (
//            const GeometricField<Type, fvsPatchField, surfaceMesh>& faceU,
//            const GeometricField<Type, dgPatchField, dgMesh>& U,
//            const surfaceScalarField& rAUf
//        );
//
//
//        tmp<surfaceScalarField> meshPhi
//        (
//            const GeometricField<Type, dgPatchField, dgMesh>&
//        );
//};
//
//
//template<>
//tmp<surfaceScalarField> EulerDdtScheme<scalar>::dgcDdtPhiCorr
//(
//    const volScalarField& rA,
//    const volScalarField& U,
//    const surfaceScalarField& phi
//);
//
//
//template<>
//tmp<surfaceScalarField> EulerDdtScheme<scalar>::dgcDdtPhiCorr
//(
//    const volScalarField& rA,
//    const volScalarField& rho,
//    const volScalarField& U,
//    const surfaceScalarField& phi
//);
//
//
//template<>
//tmp<surfaceScalarField> EulerDdtScheme<scalar>::dgcDdtConsistentPhiCorr
//(
//    const surfaceScalarField& faceU,
//    const volScalarField& U,
//    const surfaceScalarField& rAUf
//);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "EulerDdtScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
