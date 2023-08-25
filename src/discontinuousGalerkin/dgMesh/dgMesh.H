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
    dgMesh

Description
    Discontinuous Galerkin mesh

SourceFiles
    dgMesh.C

\*---------------------------------------------------------------------------*/

#ifndef dgMesh_H
#define dgMesh_H

#include "BoSSScpp.h"

#include "GeoMesh.H"
#include "MeshObject.H"
#include "polyMesh.H"
#include "lduMesh.H"
#include "fvMesh.H"
#include "primitiveMesh.H"
#include "dgBoundaryMesh.H"
#include "DimensionedField.H"
#include "primitiveFieldsFwd.H"
#include "dgSchemes.H"
#include "dgSolution.H"
#include "dgOrder.H"
#include "dgBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations
class dgMeshLduAddressing;

/*---------------------------------------------------------------------------*\
                          Class dgMesh Declaration
\*---------------------------------------------------------------------------*/

class dgMesh
:
    public GeoMesh<fvMesh>,
    public MeshObject<fvMesh, dgMesh>,
    public lduMesh
{
    // Private data

        //- Boundary mesh
        dgBoundaryMesh boundary_;

        // Here for now, probably eventually a better place will exist
        dgSchemes schemesDict_;

        dgSolution solutionDict_;

        mutable BoSSS::Foundation::Grid::OpenFOAMGrid* bosssmesh_;

        mutable scalarField* cellScaleCoeffsPtr_;

        autoPtr<dgBase> dgBasePtr_;
        //dgPolynomials polynomials_;

        const IOobject meshIOobj =
                IOobject
                (
                "fvMesh",
                // runTime.timeName(),
                "0",
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
                );

    // Demand-driven data

        //- ldu addressing
        mutable dgMeshLduAddressing* lduPtr_;


    // Static Private Data


    // Private Member Functions

        //- Disallow default bitwise copy construct
        dgMesh(const dgMesh&);

        //- Disallow default bitwise assignment
        void operator=(const dgMesh&);


        // Private member functions to calculate demand driven data

            //- Calculate addressing
            void calcAddressing();

            //- Clear addressing
            void clearAddressing();


public:

    // Public typedefs

        typedef dgMesh Mesh;
        typedef dgBoundaryMesh BoundaryMesh;

        fvMesh* finVolMesh_;


    //- Runtime type information
    TypeName("dgMesh");


    // Constructors

        //- Construct from polyMesh
        explicit dgMesh(const fvMesh& m);


    //- Destructor
    virtual ~dgMesh();


    // Member Functions

        BoSSS::Foundation::Grid::OpenFOAMGrid* GetBoSSSobject() const {
            return bosssmesh_;
        }

        void SetBoSSSobject(BoSSS::Foundation::Grid::OpenFOAMGrid* bo) const {
            bosssmesh_ = bo;
        }

        // Database

            //- Return access to fvMesh
            const fvMesh& mesh() const
            {
                return MeshObject<fvMesh, dgMesh>::mesh();
            }

            //- Return reference to time
            const Time& time() const;

        // const IOobject meshIOobj =
        //         IOobject
        //         (
        //         "fvMesh",
        //         // runTime.timeName(),
        //         "0",
        //         mesh(),
        //         IOobject::NO_READ,
        //         IOobject::NO_WRITE
        //         );

        fvMesh* finVolMesh() const {
            if (finVolMesh_ == NULL){
                Info << "finVolMesh not initialized" << endl;
            }
            return finVolMesh_;
        }

    // Access

            //- Return reference to the mesh database
            virtual const objectRegistry& thisDb() const;

            //- Return constant reference to boundary mesh
            const dgBoundaryMesh& boundary() const;

            //- Return ldu addressing
            virtual const lduAddressing& lduAddr() const;

            //- Return a list of pointers for each patch
            //  with only those pointing to interfaces being set
            virtual lduInterfacePtrsList interfaces() const
            {
                return boundary().interfaces();
            }

            //- Internal face owner
            const unallocLabelList& owner() const
            {
                return lduAddr().lowerAddr();
            }

            //- Internal face neighbour
            const unallocLabelList& neighbour() const
            {
                return lduAddr().upperAddr();
            }

            //- Return access to polynomials
            const dgBase& polynomials() const
            {
                return dgBasePtr_();
            }

            dgBase& polynomials()
            {
                return dgBasePtr_();
            }

            const dgSchemes& schemesDict() const
            {
                return schemesDict_;
            }

            dgSchemes& schemesDict()
            {
                return schemesDict_;
            }

            const dgSolution& solutionDict() const
            {
                return solutionDict_;
            }

            dgSolution& solutionDict()
            {
                return solutionDict_;
            }

            label modalSize() const
            {
                return dgOrder::length;
            }

            scalar size() const
            {
                if (!cellScaleCoeffsPtr_)
                {
                    calcCellScaleCoeffs();
                }

                return cellScaleCoeffsPtr_->size();
            }

            const scalarField& cellScaleCoeffs() const
            {
                if (!cellScaleCoeffsPtr_)
                {
                    calcCellScaleCoeffs();
                }

                return *cellScaleCoeffsPtr_;
            }

            // Communication support

                //- Return communicator used for parallel communication
                virtual int comm() const
                {
                    return mesh().comm();
                }


        // Mesh motion and mophing

            //- Is mesh moving
            bool moving() const
            {
                return mesh().moving();
            }

            //- Update after mesh motion
            virtual bool movePoints() const;

            //- Update after topo change
            virtual bool updateMesh(const mapPolyMesh&) const;

            scalarList gaussWeights();

            scalarList gaussPoints();


        // Mapping

            void calcCellScaleCoeffs() const;


        // Demand-driven data


    // Member Operators

        bool operator!=(const dgMesh& m) const;
        bool operator==(const dgMesh& m) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
