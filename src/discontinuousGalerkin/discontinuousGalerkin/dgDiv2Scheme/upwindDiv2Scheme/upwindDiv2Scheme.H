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
    Foam::dg::upwindDiv2Scheme

Description
    Basic second-order dgDiv2 using face-gradients and Gauss' theorem.

SourceFiles
    upwindDiv2Scheme.C

\*---------------------------------------------------------------------------*/

#ifndef upwindDiv2Scheme_H
#define upwindDiv2Scheme_H

#include "dgDiv2Scheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                       Class upwindDiv2Scheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GType>
class upwindDiv2Scheme
:
    public dg::dgDiv2Scheme<Type, GType>
{
    // Private data

        //- Penalty factor Eta pointer
        mutable scalarField* etaPtr_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        upwindDiv2Scheme(const upwindDiv2Scheme&);

        //- Disallow default bitwise assignment
        void operator=(const upwindDiv2Scheme&);

        //- Update Eta
        void calcEta();


public:

    //- Runtime type information
    TypeName("upwind");


    // Constructors

        //- Construct null
        upwindDiv2Scheme(const dgMesh& mesh)
        :
            dgDiv2Scheme<Type, GType>(mesh),
            etaPtr_(NULL)
        {}

        //- Construct from Istream
        upwindDiv2Scheme(const dgMesh& mesh, Istream& is)
        :
            dgDiv2Scheme<Type, GType>(mesh, is),
            etaPtr_(NULL)
        {}



    // Destructor

        virtual ~upwindDiv2Scheme()
        {}


    // Member Functions


        // NOT IMPLEMENTED DGC FOR DIV (copied from laplacian)
        tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgcDiv2
        (
            const dimensionedScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        );


        tmp<dgMatrix<Type> > dgmDiv2
        (
//            const dimensionedScalar&,
            const DgGeometricField<dgVector, dgPatchField, cellMesh>&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "upwindDiv2Scheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
