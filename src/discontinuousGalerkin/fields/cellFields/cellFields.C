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

\*---------------------------------------------------------------------------*/

#include "dgMesh.H"
#include "cellFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

//defineTemplateTypeNameAndDebug(preCellScalarField::DimensionedInternalField, 0);
//defineTemplateTypeNameAndDebug(preCellVectorField::DimensionedInternalField, 0);

defineTemplateTypeNameAndDebug(preCellScalarField, 0);
defineTemplateTypeNameAndDebug(preCellVectorField, 0);

defineTemplateTypeNameAndDebug(cellScalarField::DimensionedInternalField, 0);
defineTemplateTypeNameAndDebug(cellVectorField::DimensionedInternalField, 0);

defineTemplateTypeNameAndDebug(cellScalarField, 0);
defineTemplateTypeNameAndDebug(cellVectorField, 0);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
// Constructor given a DgGeometricField and dimensionSet
// This allocates storage for the field but not values.
// Note : This constructor should only be used to
//       construct TEMPORARY variables
template<class Type, template<class> class PatchField, class GeoMesh>
Foam::DgGeometricField<Type, PatchField, GeoMesh>::DgGeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const word& patchFieldType
)
: GeometricField<Type, PatchField, GeoMesh>(io, mesh, ds, patchFieldType)
{
    if (debug)
    {
        Info<< "DgGeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "creating temporary"
            << endl << this->info() << endl;
    }

}


template<class Type, template<class> class PatchField, class GeoMesh>
Foam::DgGeometricField<Type, PatchField, GeoMesh>::DgGeometricField
(
    const IOobject& io,
    const Mesh& mesh
)
:
    GeometricField<Type, PatchField, GeoMesh>(io, mesh)
{
    
    if (debug)
    {
        Info<< "Finishing read-construct of "
               "DgGeometricField<Type, PatchField, GeoMesh>"
            << endl << this->info() << endl;
    }
}
*/




} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

