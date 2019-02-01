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
    Volume integrate volField creating a volField.

    Volume integrate volField over the whole domain
    creating a dimensioned\<Type\>

SourceFiles
    dgcVolumeIntegrate.C

\*---------------------------------------------------------------------------*/


#ifndef dgcVolumeIntegrate_H
#define dgcVolumeIntegrate_H

#include "cellFieldsFwd.H"
#include "primitiveFieldsFwd.H"
#include "dgPrimitiveFieldsFwd.H"
#include "dimensionedDgTypes.H"
//#include "dimensionedTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace dgc functions Declaration
\*---------------------------------------------------------------------------*/

namespace dgc
{
    template<class Type>
    tmp<cellScalarField> volumeIntegrate
    (
        const GeometricField<Type, dgPatchField, cellMesh>&
    );

    template<class Type>
    tmp<scalarField> volumeIntegrate
    (
        const Field<Type>&,
        const polyMesh& mesh
    );

    template<class Type>
    tmp<Field<Type> > volumeIntegrate
    (
        const tmp<GeometricField<Type, dgPatchField, cellMesh> >&
    );

    // For laplacian calculation
    template<class Type>
    tmp<cellScalarField> volumeIntegrateGrad
    (
        const GeometricField<Type, dgPatchField, cellMesh>&
    );

    // For laplacian calculation
    template<class Type>
    tmp<scalarField> volumeIntegrateGrad
    (
        const Field<Type>&,
        const polyMesh& mesh
    );

    template<class Type>
    dimensioned<Type> domainIntegrate
    (
        const GeometricField<Type, dgPatchField, cellMesh>&
    );

    template<class Type>
    dimensioned<Type> domainIntegrate
    (
        const tmp<GeometricField<Type, dgPatchField, cellMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgcVolumeIntegrate.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
