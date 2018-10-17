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

#ifndef SIPLaplacianScheme_H
#define SIPLaplacianScheme_H

#include "dgLaplacianScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                       Class SIPLaplacianScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GType>
class SIPLaplacianScheme
:
    public dg::dgLaplacianScheme<Type, GType>
{
    // Private Member Functions

//        tmp<dgMatrix<Type> > dgmLaplacianUncorrected
//        (
//            const surfaceScalarField& gammaMagSf,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        );
//
//        tmp<GeometricField<Type, dgsPatchField, surfaceMesh> > gammaSnGradCorr
//        (
//            const surfaceVectorField& SfGammaCorr,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        );

        //- Disallow default bitwise copy construct
        SIPLaplacianScheme(const SIPLaplacianScheme&);

        //- Disallow default bitwise assignment
        void operator=(const SIPLaplacianScheme&);


public:

    //- Runtime type information
    TypeName("SIP");


    // Constructors

        //- Construct null
        SIPLaplacianScheme(const dgMesh& mesh)
        :
            dgLaplacianScheme<Type, GType>(mesh)
        {}

        //- Construct from Istream
        SIPLaplacianScheme(const dgMesh& mesh, Istream& is)
        :
            dgLaplacianScheme<Type, GType>(mesh, is)
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

        virtual ~SIPLaplacianScheme()
        {}


    // Member Functions

        tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
        (
            const dimensionedScalar&,
            const GeometricField<Type, dgPatchField, cellMesh>&
        );

//        tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        );

        tmp<dgMatrix<Type> > dgmLaplacian
        (
            const dimensionedScalar&,
            const GeometricField<Type, dgPatchField, cellMesh>&
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
#   include "SIPLaplacianScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
