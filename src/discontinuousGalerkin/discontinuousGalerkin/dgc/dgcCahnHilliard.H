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

InNamespace
    Foam::dgc

Description
    Calculate the dgCahnHilliard of the given field.

SourceFiles
    dgcCahnHilliard.C

\*---------------------------------------------------------------------------*/


#ifndef dgcCahnHilliard_H
#define dgcCahnHilliard_H

#include "cellFieldsFwd.H"
//#include "surfaceFieldsFwd.H"
#include "dimensionedDgTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace dgc functions Declaration
\*---------------------------------------------------------------------------*/

namespace dgc
{
    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const DgGeometricField<VType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const word&
    );

//    template<class Type>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const dimensionedScalar&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const DgGeometricField<VType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&
    );


//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const dimensioned<GType>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const word&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const DgGeometricField<VType, dgPatchField, cellMesh>&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&
    );

// WE USE THIS:
    template<class Type>//, class GType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const DgGeometricField<dgVector, dgPatchField, cellMesh>&,
        const DgGeometricField<dgScalar, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh>>&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh>>&,
        const word&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const DgGeometricField<VType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const word&
    );

    template<class Type, class GType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<GType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const DgGeometricField<VType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&
    );

    template<class Type, class VType>
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
    (
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >&,
        const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >&
    );


//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const tmp<GeometricField<Type, dgPatchField, cellMesh> >&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const tmp<GeometricField<Type, dgPatchField, cellMesh> >&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const tmp<GeometricField<Type, dgPatchField, cellMesh> >&
//    );
//
//    template<class Type, class GType>
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const tmp<GeometricField<Type, dgPatchField, cellMesh> >&
//    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgcCahnHilliard.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
