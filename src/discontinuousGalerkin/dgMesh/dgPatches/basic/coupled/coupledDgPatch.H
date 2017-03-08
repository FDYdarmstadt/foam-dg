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
    Foam::coupledDgPatch

Description
    An abstract base class for patches that couple regions of the
    computational domain e.g. cyclic and processor-processor links.

SourceFiles
    coupledDgPatch.C

\*---------------------------------------------------------------------------*/

#ifndef coupledDgPatch_H
#define coupledDgPatch_H

#include "dgPatch.H"
#include "lduInterface.H"
#include "coupledPolyPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class coupledDgPatch Declaration
\*---------------------------------------------------------------------------*/

class coupledDgPatch
:
    public lduInterface,
    public dgPatch
{
    // Private data

        const coupledPolyPatch& coupledPolyPatch_;


protected:

    // Protected Member Functions


public:

    friend class surfaceInterpolation;


    //- Runtime type information
    TypeName(coupledPolyPatch::typeName_());


    // Constructors

        //- Construct from polyPatch
        coupledDgPatch(const polyPatch& patch, const dgBoundaryMesh& bm)
        :
            dgPatch(patch, bm),
            coupledPolyPatch_(refCast<const coupledPolyPatch>(patch))
        {}


    //- Destructor
    virtual ~coupledDgPatch();


    // Member Functions

        // Access

            //- Return true if patch is coupled
            virtual bool coupled() const
            {
                return coupledPolyPatch_.coupled();
            }

            //- Return face transformation tensor
            const tensorField& forwardT() const
            {
                return coupledPolyPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            const tensorField& reverseT() const
            {
                return coupledPolyPatch_.reverseT();
            }

            //- Are the cyclic planes parallel
            bool parallel() const
            {
                return coupledPolyPatch_.parallel();
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
