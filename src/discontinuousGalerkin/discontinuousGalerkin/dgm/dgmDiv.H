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
    Calculate the matrix for the dgDiv of the field.

SourceFiles
    dgmDiv.C

\*---------------------------------------------------------------------------*/

#ifndef dgmDiv_H
#define dgmDiv_H

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
//    template<class Type>
//    tmp<dgMatrix<Type> > dgDiv
//    (
////        const GeometricField<dgVector, dgPatchField, cellMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );

    template<class Type>
    tmp<dgMatrix<Type> > dgDiv
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


//    template<class Type>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const zeroField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const zeroField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );


    template<class Type>
    tmp<dgMatrix<Type> > dgDiv
    (
        const geometricOneField&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type>
    tmp<dgMatrix<Type> > dgDiv
    (
        const geometricOneField&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const dimensioned<GType>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );

    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgDiv
    (
        const dimensioned<GType>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

// WE USE THIS:
    template<class Type>//, class GType>
    tmp<dgMatrix<Type> > dgDiv
    (
//        const dimensionedScalar&,
        const DgGeometricField<dgVector, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgDiv
    (
        const DgGeometricField<GType, dgPatchField, cellMesh>&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgDiv
    (
        const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<dgMatrix<Type> > dgDiv
    (
        const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );


//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );
//
//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
//
//    template<class Type, class GType>
//    tmp<dgMatrix<Type> > dgDiv
//    (
//        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgmDiv.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
