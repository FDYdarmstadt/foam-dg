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
    Foam::dg::SIPLaplacianScheme

Description
    Basic second-order dgLaplacian using face-gradients and Gauss' theorem.

SourceFiles
    SIPLaplacianScheme.C

\*---------------------------------------------------------------------------*/

#ifndef bosssCahnHilliardScheme_H
#define bosssCahnHilliardScheme_H

#include "dgCahnHilliardScheme.H"
// #include "tmp.H"
// #include "cellFieldsFwd.H"
// //#include "surfaceFieldsFwd.H"
// //#include "linear.H"
// //#include "correctedSnGrad.H"
// #include "typeInfo.H"
// #include "runTimeSelectionTables.H"
// #include "dimensionedTypes.H"
// #include "dgMesh.H"
// #include "dgMatrix.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                       Class SIPLaplacianScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class VType>
class bosssCahnHilliardScheme
:
    public dg::dgCahnHilliardScheme<Type, VType>,
            public refCount
{
    // Private data

        //- Penalty factor Eta pointer
        // mutable scalarField* etaPtr_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        bosssCahnHilliardScheme<Type, VType>Scheme(const bosssCahnHilliardScheme<Type, VType>&);

        //- Disallow default bitwise assignment
        void operator=(const bosssCahnHilliardScheme<Type, VType> Scheme);


public:

    //- Runtime type information
    TypeName("bosssCahnHilliard");


    // Constructors

        //- Construct null
        bosssCahnHilliardScheme<Type, VType>(const dgMesh& mesh)
        :
            dgCahnHilliardScheme<Type, VType>(mesh)
        {}

        //- Construct from Istream
        bosssCahnHilliardScheme<Type, VType>(const dgMesh& mesh, Istream& is)
        :
            dgCahnHilliardScheme<Type, VType>(mesh, is)
        {}

        //- Construct from mesh, interpolation and snGradScheme schemes
//        SIPLaplacianScheme
//        (
//            const dgMesh& mesh
//            const tmp<surfaceInterpolationScheme<GType> >& igs,
//            const tmp<snGradScheme<Type> >& sngs
//        )
//        :
//            dgLaplacianScheme<Type, GType>(mesh, igs, sngs)
//        {}


    // Destructor

        virtual ~bosssCahnHilliardScheme<Type, VType>()
        {}


    // Member Functions

        tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgcCahnHilliard
        (
            // const dimensionedScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&,
            const DgGeometricField<VType, dgPatchField, cellMesh>&,
            const DgGeometricField<VType, dgPatchField, cellMesh>&
        );

//        tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        );

        tmp<dgMatrix<Type> > dgmCahnHilliard
        (
            // const dimensionedScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&,
            const DgGeometricField<VType, dgPatchField, cellMesh>&,
            const DgGeometricField<VType, dgPatchField, cellMesh>&
        );

//        tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const GeometricField<GType, dgsPatchField, surfaceMesh>&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "bosssCahnHilliardScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
