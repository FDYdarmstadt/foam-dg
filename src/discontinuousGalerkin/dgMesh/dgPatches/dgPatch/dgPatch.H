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
    Foam::dgPatch

Description
    Discontinuous Galerkin patch

SourceFiles
    dgPatch.C
    newDgPatch.C

\*---------------------------------------------------------------------------*/

#ifndef dgPatch_H
#define dgPatch_H

#include "polyPatch.H"
#include "labelList.H"
#include "typeInfo.H"
#include "tmp.H"
#include "primitiveFields.H"
#include "SubField.H"
//#include "dgPatchFieldsFwd.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class dgBoundaryMesh;

/*---------------------------------------------------------------------------*\
                           Class dgPatch Declaration
\*---------------------------------------------------------------------------*/

class dgPatch
{
    // Private data

        //- Reference to the underlying polyPatch
        const polyPatch& polyPatch_;

        //- Reference to boundary mesh
        const dgBoundaryMesh& boundaryMesh_;


    // Private Member Functions

        //- Disallow construct as copy
        dgPatch(const dgPatch&);

        //- Disallow assignment
        void operator=(const dgPatch&);


protected:

    // Protected Member Functions

        //- Initialise the patches for moving points
        virtual void initMovePoints();

        //- Correct patches after moving points
        virtual void movePoints();

        //- Initialise the update of the patch topology
        virtual void initUpdateMesh();

        //- Update of the patch topology
        virtual void updateMesh();


public:

    typedef dgBoundaryMesh BoundaryMesh;

    friend class dgBoundaryMesh;

    //- Runtime type information
    TypeName(polyPatch::typeName_());


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            dgPatch,
            polyPatch,
            (const polyPatch& patch, const dgBoundaryMesh& bm),
            (patch, bm)
        );


    // Constructors

        //- Construct from polyPatch
        dgPatch(const polyPatch&, const dgBoundaryMesh&);


    // Selectors

        //- Return a pointer to a new patch created on freestore from polyPatch
        static autoPtr<dgPatch> New(const polyPatch&, const dgBoundaryMesh&);


    //- Destructor
    virtual ~dgPatch();


    // Member Functions

    // Access

            //- Return the polyPatch
            const polyPatch& patch() const
            {
                return polyPatch_;
            }

            //- Return name
            const word& name() const
            {
                return polyPatch_.name();
            }

            //- Return size
            virtual label size() const
            {
                return polyPatch_.size();
            }

            //- Return true if this patch is coupled
            bool coupled() const
            {
                return polyPatch_.coupled();
            }

            //- Return the index of this patch in the dgBoundaryMesh
            label index() const
            {
                return polyPatch_.index();
            }

            //- Return boundaryMesh reference
            const dgBoundaryMesh& boundaryMesh() const
            {
                return boundaryMesh_;
            }

            //- Return faceCells
            virtual const unallocLabelList& faceCells() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
