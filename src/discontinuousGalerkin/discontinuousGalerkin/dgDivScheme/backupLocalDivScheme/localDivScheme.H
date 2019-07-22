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
    Foam::dg::localDivScheme

Description
    Basic second-order dgDiv using face-gradients and Gauss' theorem.

SourceFiles
    localDivScheme.C

\*---------------------------------------------------------------------------*/

#ifndef localDivScheme_H
#define localDivScheme_H

#include "dgDivScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                       Class localDivScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GType>
class localDivScheme
:
    public dg::dgDivScheme<Type, GType>
{
    // Private data

        //- Penalty factor Eta pointer
        mutable scalarField* etaPtr_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        localDivScheme(const localDivScheme&);

        //- Disallow default bitwise assignment
        void operator=(const localDivScheme&);

        //- Update Eta
        void calcEta();


public:

    //- Runtime type information
    TypeName("local");


    // Constructors

        //- Construct null
        localDivScheme(const dgMesh& mesh)
        :
            dgDivScheme<Type, GType>(mesh),
            etaPtr_(NULL)
        {}

        //- Construct from Istream
        localDivScheme(const dgMesh& mesh, Istream& is)
        :
            dgDivScheme<Type, GType>(mesh, is),
            etaPtr_(NULL)
        {}



    // Destructor

        virtual ~localDivScheme()
        {}


    // Member Functions


        // NOT IMPLEMENTED DGC FOR DIV (copied from laplacian)
        tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcDiv
        (
            const dimensionedScalar&,
            const GeometricField<Type, dgPatchField, cellMesh>&
        );


        tmp<dgMatrix<Type> > dgmDiv
        (
//            const dimensionedScalar&,
            const GeometricField<dgVector, dgPatchField, cellMesh>&,
            const GeometricField<Type, dgPatchField, cellMesh>&
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "localDivScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
