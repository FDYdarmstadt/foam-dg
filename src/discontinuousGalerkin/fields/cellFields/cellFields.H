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

SourceFiles
    cellFields.C

\*---------------------------------------------------------------------------*/

#ifndef cellFields_H
#define cellFields_H

#include "BoSSScpp.h"

#include "objectRegistry.H"
#include "GeometricFields.H"
#include "cellMesh.H"
#include "dgMesh.H"
#include "dgPatchFields.H"
#include "cellFieldsFwd.H"
#include "dgOrder.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
using BoSSS::Application::ExternalBinding::OpenFoamDGField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*---------------------------------------------------------------------------*\
                           Class DgGeometricField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, template<class> class PatchField, class GeoMesh>
class DgGeometricField
:
    public GeometricField<Type, PatchField, GeoMesh>
{
public:
    OpenFoamDGField* bosssObject_ = NULL;

private:
    const dgMesh& dgmesh_;

public:

    // Public typedefs

        //- Field mesh type (eg. volMesh, surfaceMesh, areaMesh)
        typedef GeoMesh GeoMeshType;

        //- Type of mesh on which this GeometricField is instantiated
        typedef typename GeoMesh::Mesh Mesh;

        //- Type of boundary mesh on which this
        //  GeometricField::Boundary is instantiated
        typedef typename GeoMesh::BoundaryMesh BoundaryMesh;

        //- Type of the internal field from which this GeometricField is derived
        typedef DimensionedField<Type, GeoMesh> DimensionedInternalField;
        typedef Field<Type> InternalField;

        //- Type of the patch field of which the
        //  GeometricField::Boundary is composed
        typedef PatchField<Type> PatchFieldType;

        //- Field element type (eg. scalar, vector)
        typedef Type PrimitiveType;

        //- Field element component type
        typedef typename Field<Type>::cmptType cmptType;


public:


    //- Runtime type information
    TypeName("DgGeometricField");


    // Static Member Functions

        //- Return a null geometric field
        inline static const DgGeometricField<Type, PatchField, GeoMesh>& null();


    // Constructors

        //- Constructor given IOobject, mesh, dimensions and patch type.
        //  This allocates storage for the field but not values.
        //  Used only within this class to create TEMPORARY variables
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            const dimensionSet& c,
            const word& patchFieldType=PatchField<Type>::calculatedType()
        ) : 
            GeometricField<Type, PatchField, GeoMesh>(a, b, c, patchFieldType),
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }
 
        //- Constructor given IOobject, mesh, dimensions and patch types.
        //  This allocates storage for the field but not values.
        //  Used only within this class to create TEMPORARY variables
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            const dimensionSet& c,
            const wordList& patchFieldTypes
        ) : 
            GeometricField<Type, PatchField, GeoMesh>(a, b, c, patchFieldTypes),
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }

        //- Constructor given IOobject, mesh, dimensioned<Type> and patch type.
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            const dimensioned<Type>& c,
            const word& patchFieldType=PatchField<Type>::calculatedType()
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b, c, patchFieldType),
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }
        //- Construct given IOobject, mesh, dimensioned<Type> and patch types
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            const dimensioned<Type>& c,
            const wordList& patchFieldTypes
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b, c, patchFieldTypes) ,
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }
        //- Construct from components
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            const dimensionSet& c,
            const Field<Type>& d,
            const PtrList<PatchField<Type> >& e
        )  : GeometricField<Type, PatchField, GeoMesh>(a, b, c, d, e) ,
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }
        //- Construct and read given IOobject
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b),
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }

        //- Construct and read from given stream
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            Istream& c
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b, c) ,
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }

        //- Construct from dictionary
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            const dictionary& c
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b, c)  ,
            dgmesh_(b)
        { 
            GetBoSSSobject();
        }

        //- Construct as copy
        DgGeometricField
        (
            const DgGeometricField<Type, PatchField, GeoMesh>& a
        ) : GeometricField<Type, PatchField, GeoMesh>(a)  ,
            dgmesh_(a.dgmesh_)
        { 
            GetBoSSSobject();
        }

        //- Construct as copy of tmp<GeometricField> deleting argument
#       ifdef ConstructFromTmp
        DgGeometricField
        (
            const tmp<GeometricField<Type, PatchField, GeoMesh> >& a
        ) : GeometricField<Type, PatchField, GeoMesh>(a) ,
            dgmesh_(a.ptr()->dgmesh_)
        { 
            GetBoSSSobject();
        }
#       endif

        //- Construct as copy resetting IO parameters
        DgGeometricField
        (
            const IOobject&,
            const DgGeometricField<Type, PatchField, GeoMesh>& a
        ) : GeometricField<Type, PatchField, GeoMesh>(a),
            dgmesh_(a.dgmesh_)
        { 
            GetBoSSSobject();
        }
        //- Construct as copy resetting name
        DgGeometricField
        (
            const word& newName,
            const DgGeometricField<Type, PatchField, GeoMesh>& b
        ) : GeometricField<Type, PatchField, GeoMesh>(newName, b),
            dgmesh_(b.dgmesh_)
        { 
            GetBoSSSobject();
        }

#       ifdef ConstructFromTmp
        DgGeometricField
        (
            const word& newName,
            const tmp<DgGeometricField<Type, PatchField, GeoMesh> >& b
        ) : GeometricField<Type, PatchField, GeoMesh>(newName, b),
            dgmesh_(b.dgmesh_)
        { 
            GetBoSSSobject();
        }
#       endif

        //- Construct as copy resetting IO parameters and patch type
        DgGeometricField
        (
            const IOobject& a,
            const DgGeometricField<Type, PatchField, GeoMesh>& b,
            const word& patchFieldType
        ) : 
            GeometricField<Type, PatchField, GeoMesh>(a, b, patchFieldType),
            dgmesh_(b.dgmesh_) 
        { 
            GetBoSSSobject();
        }

        //- Construct as copy resetting IO parameters and boundary types
        DgGeometricField
        (
            const IOobject& a,
            const DgGeometricField<Type, PatchField, GeoMesh>& b,
            const wordList& patchFieldTypes
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b, patchFieldTypes), 
            dgmesh_(b.dgmesh_)
        { 
            GetBoSSSobject();
        }

        //- Clone
        tmp<DgGeometricField<Type, PatchField, GeoMesh> > clone() const
        {
            return tmp<DgGeometricField<Type, PatchField, GeoMesh> >
            (
                new DgGeometricField<Type, PatchField, GeoMesh>(*this)
            );
        }


    //- Destructor
    virtual ~DgGeometricField() { 
        if(bosssObject_ != NULL) {
            delete bosssObject_;
        }
    }


    // Member Functions

    OpenFoamDGField* GetBoSSSobject() {
        if(bosssObject_ == NULL) {
            int NoOfComps;
            if(typeid(Type) == typeid(dgScalar)) {
                NoOfComps = 1;
            } else if(typeid(Type) == typeid(dgVector)) {
                NoOfComps = dgOrder::dim;
            } else {
                NoOfComps = -1;
            }

            //const GeoMesh& msh = mesh();

            bosssObject_ = new OpenFoamDGField(dgmesh_.GetBoSSSobject(), dgOrder::order, NoOfComps);
        }
        return bosssObject_;
    }

    dgMesh dgmesh() const {
        return dgmesh_;
    }
    
    void SyncToBoSSS() {
   
        label J = dgmesh_.mesh().nCells();
        OpenFoamDGField* bo = GetBoSSSobject();

        if(typeid(Type) == typeid(dgScalar)) {
            for(label j = 0; j < J; j++) {
                dgScalar cellValue = (*this)[j];
                int N = cellValue.size();
                for(int n = 0; n < N; n++)
                    bo->SetDGcoordinate(0, j, n, cellValue[n]);
            }
        }

    }

    void SyncFromBoSSS() {
        label J = dgmesh_.mesh().nCells();
        OpenFoamDGField* bo = GetBoSSSobject();

        if(typeid(Type) == typeid(dgScalar)) {
            for(label j = 0; j < J; j++) {
                //int N = dgOrder::length;
                int N = (*this)[j].size();
                dgScalar cellValue;
                for(int n = 0; n < N; n++)
                    cellValue[n] = bo->GetDGcoordinate(0, j, n);
                (*this)[j] = cellValue;
            }
        }

    }


        
/*
        //- Return a component of the field
        tmp<DgGeometricField<cmptType, PatchField, GeoMesh> > component
        (
            const direction
        ) const;

        //- WriteData member function required by regIOobject
        bool writeData(Ostream&) const;

        //- Return transpose (only if it is a tensor field)
        tmp<DgGeometricField<Type, PatchField, GeoMesh> > T() const;
*/
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
