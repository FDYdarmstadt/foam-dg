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
    Foam::dg::dgLaplacianScheme

Description
    Abstract base class for dgLaplacian schemes.

SourceFiles
    dgLaplacianScheme.C

\*---------------------------------------------------------------------------*/

#ifndef dgLaplacianScheme_H
#define dgLaplacianScheme_H

#include "tmp.H"
#include "cellFieldsFwd.H"
//#include "surfaceFieldsFwd.H"
//#include "linear.H"
//#include "correctedSnGrad.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "dimensionedTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class Type>
class dgMatrix;

class dgMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

/*---------------------------------------------------------------------------*\
                           Class dgLaplacianScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GType>
class dgLaplacianScheme
:
    public refCount
{

protected:

    // Protected data

        const dgMesh& mesh_;
//        tmp<surfaceInterpolationScheme<GType> > tinterpGammaScheme_;
//        tmp<snGradScheme<Type> > tsnGradScheme_;


    // Private Member Functions

        //- Disallow copy construct
        dgLaplacianScheme(const dgLaplacianScheme&);

        //- Disallow default bitwise assignment
        void operator=(const dgLaplacianScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            dgLaplacianScheme,
            Istream,
            (const dgMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        dgLaplacianScheme(const dgMesh& mesh)
        :
            mesh_(mesh)
//            tinterpGammaScheme_(new linear<GType>(mesh)),
//            tsnGradScheme_(new correctedSnGrad<Type>(mesh))
        {}

        //- Construct from mesh and Istream
        dgLaplacianScheme(const dgMesh& mesh, Istream& is)
        :
            mesh_(mesh)
        {}
//            tinterpGammaScheme_(NULL),
//            tsnGradScheme_(NULL)
//        {
//            tinterpGammaScheme_ = tmp<surfaceInterpolationScheme<GType> >
//            (
//                surfaceInterpolationScheme<GType>::New(mesh, is)
//            );
//
//            tsnGradScheme_ = tmp<snGradScheme<Type> >
//            (
//                snGradScheme<Type>::New(mesh, is)
//            );
//        }

        //- Construct from mesh, interpolation and snGradScheme schemes
//        dgLaplacianScheme
//        (
//            const dgMesh& mesh
////            const tmp<surfaceInterpolationScheme<GType> >& igs,
////            const tmp<snGradScheme<Type> >& sngs
//        )
//        :
//            mesh_(mesh)
////            tinterpGammaScheme_(igs),
////            tsnGradScheme_(sngs)
//        {}


    // Selectors

        //- Return a pointer to a new dgLaplacianScheme created on freestore
        static tmp<dgLaplacianScheme<Type, GType> > New
        (
            const dgMesh& mesh,
            Istream& schemeData
        );


    // Destructor

        virtual ~dgLaplacianScheme();


    // Member Functions

        //- Return mesh reference
        const dgMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<dgMatrix<Type> > dgmLaplacian
        (
            const dimensionedScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;

//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;

        virtual tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
        (
            const dimensionedScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;

//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const GeometricField<GType, dgPatchField, cellMesh>&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeDgLaplacianTypeScheme(SS, Type, GType)                            \
                                                                              \
    typedef SS<Type, GType> SS##Type##GType;                                  \
    defineNamedTemplateTypeNameAndDebug(SS##Type##GType, 0);                  \
                                                                              \
    dgLaplacianScheme<Type, GType>::                                            \
        addIstreamConstructorToTable<SS<Type, GType> >                        \
    add##SS##Type##GType##IstreamConstructorToTable_;


#define makeDgLaplacianScheme(SS)                                             \
                                                                              \
makeDgLaplacianTypeScheme(SS, dgScalar, scalar)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgLaplacianScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
