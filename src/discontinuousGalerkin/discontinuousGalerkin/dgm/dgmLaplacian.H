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
    Foam::dgm

Description
    Calculate the matrix for the dgLaplacian of the field.

SourceFiles
    dgmLaplacian.C

\*---------------------------------------------------------------------------*/

#ifndef dgmLaplacian_H
#define dgmLaplacian_H

#include "cellFieldsFwd.H"
//#include "surfaceFieldsFwd.H"
#include "dgMatrices.H"
#include "geometricOneField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace dgm functions Declaration
\*---------------------------------------------------------------------------*/

namespace dgm
{
    template<class Type>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


//    template<class Type>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const zeroField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const zeroField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );


    template<class Type>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const geometricOneField&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const geometricOneField&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const dimensioned<GType>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );

    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const dimensioned<GType>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

// WE USE THIS:
    template<class Type>//, class GType>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const dimensionedScalar&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const DgGeometricField<GType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgLaplacian
    (
        const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
//
//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgLaplacian
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgmLaplacian.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
