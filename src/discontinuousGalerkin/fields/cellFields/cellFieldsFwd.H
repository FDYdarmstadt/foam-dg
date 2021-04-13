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
    cellFields

Description
    Discontinuous Galerkin cell fields

SourceFiles
    cellFields.C


\*---------------------------------------------------------------------------*/

#ifndef cellFieldsFwd_H
#define cellFieldsFwd_H

#include "dgFieldTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class cellMesh;

template<class Type>
class dgPatchField;

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;

template<class Type, template<class> class PatchField, class GeoMesh>
class DgGeometricField;


typedef GeometricField<dgScalar, dgPatchField, cellMesh> preCellScalarField;
typedef GeometricField<dgVector, dgPatchField, cellMesh> preCellVectorField;

typedef DgGeometricField<dgScalar, dgPatchField, cellMesh> cellScalarField;
typedef DgGeometricField<dgVector, dgPatchField, cellMesh> cellVectorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
