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
#include "volMesh.H"
#include "dgPatchFields.H"
#include "OpenFoamPatchField.h"
#include "cellFieldsFwd.H"
#include "dgOrder.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
using BoSSS::Application::ExternalBinding::OpenFoamDGField;
using BoSSS::Application::ExternalBinding::OpenFoamPatchField;

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
    OpenFoamPatchField* bosssObjectPatchField_ = NULL;
        volScalarField* vsF_ = NULL;
        volVectorField* vvF_ = NULL;

    public: const dgMesh& dgmesh_;

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
        // inline static const DgGeometricField<Type, PatchField, GeoMesh>& null(){}


    // Constructors

        //- empty Constructor
        //  This allocates storage for the field but not values.
        //  Used only within this class to create TEMPORARY variables
        // DgGeometricField
        // (
        //     const Mesh& b,
        //     const dimensionSet& c,
        //     const word& patchFieldType=PatchField<Type>::calculatedType()
        // ) :
        //     GeometricField<Type, PatchField, GeoMesh>(nullptr, b, c, patchFieldType),
        //     dgmesh_(b)
        // {
        //     GetBoSSSobject();
        //     GetBoSSSPatchobject();
        // }

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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
        }

        //- Construct and read given IOobject
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            volScalarField& vsf
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b),
            dgmesh_(b)
        {
            GetBoSSSobject();
            GetBoSSSPatchobject();
            vsF_ = &vsf;
            this->importFromVolScaField();
        }

        //- Construct and read given IOobject
        DgGeometricField
        (
            const IOobject& a,
            const Mesh& b,
            volVectorField& vvf
        ) : GeometricField<Type, PatchField, GeoMesh>(a, b),
            dgmesh_(b)
        {
            GetBoSSSobject();
            GetBoSSSPatchobject();
            vvF_ = &vvf;
            this->importFromVolVecField();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
        }

        //- Construct as copy
        DgGeometricField
        (
            const DgGeometricField<Type, PatchField, GeoMesh>& a
        ) : GeometricField<Type, PatchField, GeoMesh>(a)  ,
            dgmesh_(a.dgmesh_)
        { 
            GetBoSSSobject();
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
            GetBoSSSPatchobject();
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
        if(bosssObjectPatchField_ != NULL) {
            delete bosssObjectPatchField_;
        }
    }


    // Member Functions
// template<>
// template<template<class> class PatchField, class GeoMesh>
    void GenericSyncMeanFromBoSSSDGScalarField(
        GeometricField<scalar, fvPatchField, volMesh> &dgf) {

        BoSSS::Application::ExternalBinding::OpenFoamDGField *bo = GetBoSSSobject();
      label J = dgf.mesh().nCells();
      for (label j = 0; j < J; j++) {
          dgf[j] = bo->GetMean(0, j);
          // Info << "bo(1): " << bo->GetMean(0,j) << endl;
          // Info << "dgf: " << dgf[j] << endl;
      }
    }

// template<>
// template<template<class> class PatchField, class GeoMesh>
    void GenericSyncMeanFromBoSSSDGVectorField(
        GeometricField<vector, fvPatchField, volMesh> &dgf) {

        BoSSS::Application::ExternalBinding::OpenFoamDGField *bo = GetBoSSSobject();
      label J = dgf.mesh().nCells();
      for (label j = 0; j < J; j++) {
        int N = dgf[j].size();
        for (int i = 0; i < N; i++) {
          dgf[j][i] = bo->GetMean(i, j);
        }
      }
    }

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

    OpenFoamPatchField* GetBoSSSPatchobject() {
        if(bosssObjectPatchField_ == NULL) {
            bosssObjectPatchField_ = GenericGetBoSSSPatchobject(*this);
        }
        return bosssObjectPatchField_;
    }

    void SyncFromBoSSSDGField(OpenFoamDGField* bo) {
        GenericSyncFromBoSSSDGField(*this, bo);
    }

    void SyncToBoSSS() {
        GenericSyncToBoSSS(*this);
    }

    void SyncFromBoSSS() {
        OpenFoamDGField* bo = GetBoSSSobject();
        SyncFromBoSSSDGField(bo);
        if (std::is_same<Type, dgScalar>()) {
            GenericSyncMeanFromBoSSSDGScalarField(*vsF_);
        }
        else if (std::is_same<Type, dgVector>()) {
            GenericSyncMeanFromBoSSSDGVectorField(*vvF_);
        } else {
            throw std::runtime_error("type error!");
        }
    }

    void UpdateFVField(OpenFoamDGField* bo) {
        GenericUpdateFVField(*this, bo);
    }

    void UpdateFV0Field() {
        GenericUpdateFVField(*this);
    }

    const dgMesh& dgmesh() {
        return dgmesh_;
    }

        GeometricField<scalar, fvPatchField, volMesh> volScaField(){

            if (std::is_same<Type, dgVector>()){
                throw std::runtime_error("Cannot return volScalarField of dgVectorField");
            }
            return *vsF_;
        };

        GeometricField<vector, fvPatchField, volMesh> volVecField(){
            if (std::is_same<Type, dgScalar>()){
                throw std::runtime_error("Cannot return volVectorField of dgScalarField");
            }
            return *vvF_;
        };

        GeometricField<scalar, fvPatchField, volMesh> importFromVolScaField(){

            if (std::is_same<Type, dgVector>()){
                throw std::runtime_error("Cannot return volScalarField of dgVectorField");
            }
            importFromVolScalarField(*this, *vsF_);
            return *vsF_;
        };

        GeometricField<vector, fvPatchField, volMesh> importFromVolVecField(){
            if (std::is_same<Type, dgScalar>()){
                throw std::runtime_error("Cannot return volVectorField of dgScalarField");
            }
            importFromVolVectorField(*this, *vvF_);
            return *vvF_;
        };
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

template<class Type, template<class> class PatchField, class GeoMesh>
OpenFoamPatchField* GenericGetBoSSSPatchobject(DgGeometricField<Type, PatchField, GeoMesh> &dgf);

template<template<class> class PatchField, class GeoMesh>
OpenFoamPatchField* GenericGetBoSSSPatchobject(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf) {
    int NoOfComps = 1;
    List<int> edgeTags;
    List<int> edgeTypes;
    List<double> edgeValues;
    // List<List<double>> edgeValues;
    int i = 0;
    int noOfNonEmptyFaces = 0;
    for (auto bf : dgf.boundaryField()) {
      string bcType = dgf.boundaryField().types()[i];

      int bcTypeInt;

      if (dgf.dgmesh_.boundary()[i].type() == "empty") {
        bcTypeInt = -1;
      } else {
          // List<double> bcVal;
          double bcVal = bf[0][0];
          // edgeValues.append(bcVal);
        if (bcType == "dirichlet") {
          bcTypeInt = 0;
        } else if (bcType == "homogenous") {
          bcTypeInt = 1;
        }
        noOfNonEmptyFaces++;
        edgeTags.append(noOfNonEmptyFaces);
        edgeTypes.append(bcTypeInt);
        edgeValues.append(bcVal);
      }
      i++;
    }
    int edgeTagsArr[edgeTags.size()];
    std::copy(edgeTags.begin(), edgeTags.end(), edgeTagsArr);

    int edgeTypesArr[edgeTypes.size()];
    std::copy(edgeTypes.begin(), edgeTypes.end(), edgeTypesArr);

    double edgeValuesArr[edgeValues.size()];
    std::copy(edgeValues.begin(), edgeValues.end(), edgeValuesArr);
    // double* edgeValuesArr = (double*) malloc(sizeof(double)*edgeValues.size());
    // for (int i = 0; i < edgeValues.size(); i++){
    //     // edgeValuesArrContent[0] = edgeValues[i][0];
    //     // edgeValuesArr[i] = edgeValuesArrContent;
    //     edgeValuesArr[i] = edgeValues[i][0];
    // }

  OpenFoamPatchField* bosssObjectPatchField_ =
        new OpenFoamPatchField(dgf.dgmesh_.GetBoSSSobject(), 1, noOfNonEmptyFaces,
                               edgeTagsArr, edgeTypesArr, edgeValuesArr);
  return bosssObjectPatchField_;
}

template<template<class> class PatchField, class GeoMesh>
OpenFoamPatchField* GenericGetBoSSSPatchobject(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf) {
    int NoOfComps = dgOrder::dim;
    List<int> edgeTags;
    List<int> edgeTypes;
    List<List<double>> edgeValues;
    int i = 0;
    int noOfNonEmptyFaces = 0;
    for (auto bf : dgf.boundaryField()) {
      string bcType = dgf.boundaryField().types()[i];

      int bcTypeInt;

      if (dgf.dgmesh_.boundary()[i].type() == "empty") {
        bcTypeInt = -1;
      } else {
        List<double> bcVal;
        for (int d = 0; d < NoOfComps; d++)
            bcVal.append(bf[0][0][d]);
        if (bcType == "dirichlet") {
          bcTypeInt = 0;
        } else if (bcType == "homogenous") {
          bcTypeInt = 1;
        }
        noOfNonEmptyFaces++;
        edgeTags.append(noOfNonEmptyFaces);
        edgeTypes.append(bcTypeInt);
        edgeValues.append(bcVal);
      }
      i++;
    }
    int edgeTagsArr[edgeTags.size()];
    std::copy(edgeTags.begin(), edgeTags.end(), edgeTagsArr);

    int edgeTypesArr[edgeTypes.size()];
    std::copy(edgeTypes.begin(), edgeTypes.end(), edgeTypesArr);

    // double edgeValuesArr[edgeValues.size()][dgOrder::dim];
    // double** edgeValuesArr = (double**) malloc(sizeof(double*)*edgeValues.size());
    // double* edgeValuesArrContent = (double*) malloc(sizeof(double)*dgOrder::dim);
    double* edgeValuesArr = (double*) malloc(sizeof(double)*dgOrder::dim * edgeValues.size());
    for (int i = 0; i < edgeValues.size(); i++){
        for (int d = 0; d < dgOrder::dim; d++){
            edgeValuesArr[i * dgOrder::dim + d] = edgeValues[i][d];
        }
        // edgeValuesArr[i] = edgeValuesArrContent;
    }

  OpenFoamPatchField* bosssObjectPatchField_ =
        new OpenFoamPatchField(dgf.dgmesh_.GetBoSSSobject(), dgOrder::dim, noOfNonEmptyFaces,
                               edgeTagsArr, edgeTypesArr, edgeValuesArr);
  return bosssObjectPatchField_;
}

// template<template<class> class PatchField, class GeoMesh>
// void syncToVolScalarField(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf, GeometricField<scalar, fvPatchField, volMesh> volField){
//     OpenFoamDGField* bo = dgf.GetBoSSSobject();
//     label J = dgf.dgmesh_.mesh().nCells();
//     for (label j = 0; j < J; j++) {
//         dgf[j][0] = volField[j];
//     }
// }

// template<template<class> class PatchField, class GeoMesh>
// void syncToVolVectorField(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf, GeometricField<vector, fvPatchField, volMesh> volField){
//     OpenFoamDGField* bo = dgf.GetBoSSSobject();
//     label J = dgf.dgmesh_.mesh().nCells();
//     for (label j = 0; j < J; j++) {
//         for (label d = 0; d < 3; d++) {
//             dgf[j][d][0] = volField[j][d];
//         }
//     }
// }

template<template<class> class PatchField, class GeoMesh>
void importFromVolScalarField(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf, GeometricField<scalar, fvPatchField, volMesh> volField){
    OpenFoamDGField* bo = dgf.GetBoSSSobject();
    label J = dgf.dgmesh_.mesh().nCells();
    for (label j = 0; j < J; j++) {
        bo->SetMean(0, j, volField[j]);
        dgf[j][0] = bo->GetDGcoordinate(0, j, 0);
    }
}

template<template<class> class PatchField, class GeoMesh>
void importFromVolVectorField(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf, GeometricField<vector, fvPatchField, volMesh> volField){
    OpenFoamDGField* bo = dgf.GetBoSSSobject();
    label J = dgf.dgmesh_.mesh().nCells();
    for (label j = 0; j < J; j++) {
        for (label d = 0; d < 3; d++) {
            bo->SetMean(d, j, volField[j][d]);
            dgf[j][d][0] = bo->GetDGcoordinate(d, j, 0);
        }
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void GenericSyncToBoSSS(DgGeometricField<Type, PatchField, GeoMesh> &dgf);

template<template<class> class PatchField, class GeoMesh>
void GenericSyncToBoSSS(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf) {

        label J = dgf.dgmesh_.mesh().nCells();
        OpenFoamDGField* bo = dgf.GetBoSSSobject();

        for(label j = 0; j < J; j++) {
            dgScalar cellValue = dgf[j];
            int N = cellValue.size();
            for (int n = 0; n < N; n++) {
                bo->SetDGcoordinate(0, j, n, dgf[j][n]);
            }
        }
}

template<template<class> class PatchField, class GeoMesh>
void GenericSyncToBoSSS(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf) {
  label J = dgf.dgmesh_.mesh().nCells();
  OpenFoamDGField *bo = dgf.GetBoSSSobject();

  for (label j = 0; j < J; j++) {
    dgVector cellValue = dgf[j];
    int N = cellValue.size();
    for (int d = 0; d < 3; d++) {
      for (int n = 0; n < N; n++) {
        bo->SetDGcoordinate(d, j, n, cellValue[d][n]);
      }
    }
  }
}

template<class Type, template<class> class PatchField, class GeoMesh>
OpenFoamPatchField* GenericSyncFromBoSSSDGField(DgGeometricField<Type, PatchField, GeoMesh> &dgf, OpenFoamDGField* bo);

template<template<class> class PatchField, class GeoMesh>
void GenericSyncFromBoSSSDGField(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf, OpenFoamDGField* bo) {

        label J = dgf.dgmesh_.mesh().nCells();

            for(label j = 0; j < J; j++) {
                int N = dgf[j].size();
                for (int n = 0; n < N; n++) {
                    dgf[j][n] = bo->GetDGcoordinate(0, j, n);
                }
            }
    }

template<template<class> class PatchField, class GeoMesh>
void GenericSyncFromBoSSSDGField(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf, OpenFoamDGField* bo) {
  label J = dgf.dgmesh_.mesh().nCells();

  for (label j = 0; j < J; j++) {
    int N = dgf[j].size();
    int D = 3;
    for (int d = 0; d < D; d++) {
      for (int n = 0; n < N; n++) {
          dgf[j][d][n] = bo->GetDGcoordinate(d, j, n);
      }
    }
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
OpenFoamPatchField* GenericFVScalarField(DgGeometricField<Type, PatchField, GeoMesh> &dgf, OpenFoamDGField* bo);

template<template<class> class PatchField, class GeoMesh>
void GenericUpdateFVField(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf, OpenFoamDGField* bo) {

  label J = dgf.dgmesh_.mesh().nCells();

  for (label j = 0; j < J; j++) {
    int N = dgf[j].size();
    // dgf.vsF_->operator[](j) = dgf[j][0];
    dgf.vsF_->operator[](j) = bo->GetMean(0, j);
  }
}

template<template<class> class PatchField, class GeoMesh>
void GenericUpdateFVField(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf, OpenFoamDGField* bo) {
  label J = dgf.dgmesh_.mesh().nCells();

  for (label j = 0; j < J; j++) {
    int N = dgf[j].size();
    int D = 3;
    for (int d = 0; d < D; d++) {
      for (int n = 0; n < N; n++) {
          dgf.vvF_.operator[](j)[d] = bo->GetMean(d, j);
      }
    }
  }
}

// for use with a pseudo-DG field of degree 0
template<class Type, template<class> class PatchField, class GeoMesh>
OpenFoamPatchField* GenericFVScalarField(DgGeometricField<Type, PatchField, GeoMesh> &dgf);

template<template<class> class PatchField, class GeoMesh>
void GenericUpdateFVField(DgGeometricField<dgScalar, PatchField, GeoMesh> &dgf) {

  label J = dgf.dgmesh_.mesh().nCells();

  for (label j = 0; j < J; j++) {
    int N = dgf[j].size();
    dgf.vsF_->operator[](j) = dgf[j][0];
  }
}

template<template<class> class PatchField, class GeoMesh>
void GenericUpdateFVField(DgGeometricField<dgVector, PatchField, GeoMesh> &dgf) {
  label J = dgf.dgmesh_.mesh().nCells();

  for (label j = 0; j < J; j++) {
    int N = dgf[j].size();
    int D = 3;
    for (int d = 0; d < D; d++) {
      for (int n = 0; n < N; n++) {
          dgf.vvF_.operator[](j)[d] = dgf[j][d][0];
      }
    }
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
