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
    dgBoundaryMesh

Description
    Discontinuous Galerkin boundary mesh

SourceFiles
    dgBoundaryMesh.C

\*---------------------------------------------------------------------------*/

#ifndef dgBoundaryMesh_H
#define dgBoundaryMesh_H

#include "dgPatchList.H"
#include "lduInterfacePtrsList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class dgMesh;

/*---------------------------------------------------------------------------*\
                        Class dgBoundaryMesh Declaration
\*---------------------------------------------------------------------------*/

class dgBoundaryMesh
:
    public dgPatchList
{
    // Private data

        //- Reference to mesh
        const dgMesh& mesh_;



    // Private Member Functions

        //- Disallow construct as copy
        dgBoundaryMesh(const dgBoundaryMesh&);

        //- Disallow assignment
        void operator=(const dgBoundaryMesh&);


        // Constructor helper

        //- Add fvPatches corresponding to the given polyBoundaryMesh
        void addPatches(const polyBoundaryMesh&);


public:

    //- Runtime type information
    TypeName("dgBoundaryMesh");


    // Constructors

        //- Construct from polyBoundaryMesh
        dgBoundaryMesh
        (
            const dgMesh&,
            const polyBoundaryMesh&
        );


    //- Destructor
    virtual ~dgBoundaryMesh()
    {}


    // Member functions

        // Access

            //- Return the mesh reference
            const dgMesh& mesh() const;

            //- Return a list of pointers for each patch
            //  with only those pointing to interfaces being set
            lduInterfacePtrsList interfaces() const;


        // Edit

            //- Correct dgBoundaryMesh after moving points
            void movePoints();

            //- Correct dgBoundaryMesh after topology update
            void updateMesh(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
