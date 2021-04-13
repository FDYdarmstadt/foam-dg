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
    Foam::dg::dgDiv2Scheme

Description
    Abstract base class for dgDiv2 schemes.

SourceFiles
    dgDiv2Scheme.C

\*---------------------------------------------------------------------------*/

#ifndef dgDiv2Scheme_H
#define dgDiv2Scheme_H

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
                           Class dgDiv2Scheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GType>
class dgDiv2Scheme
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
        dgDiv2Scheme(const dgDiv2Scheme&);

        //- Disallow default bitwise assignment
        void operator=(const dgDiv2Scheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            dgDiv2Scheme,
            Istream,
            (const dgMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        dgDiv2Scheme(const dgMesh& mesh)
        :
            mesh_(mesh)
//            tinterpGammaScheme_(new linear<GType>(mesh)),
//            tsnGradScheme_(new correctedSnGrad<Type>(mesh))
        {}

        //- Construct from mesh and Istream
        dgDiv2Scheme(const dgMesh& mesh, Istream& is)
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
//        dgDiv2Scheme
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

        //- Return a pointer to a new dgDiv2Scheme created on freestore
        static tmp<dgDiv2Scheme<Type, GType> > New
        (
            const dgMesh& mesh,
            Istream& schemeData
        );


    // Destructor

        virtual ~dgDiv2Scheme();


    // Member Functions

        //- Return mesh reference
        const dgMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<dgMatrix<Type> > dgmDiv2
        (
//            const dimensionedScalar&,
            const DgGeometricField<dgVector, dgPatchField, cellMesh>&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;

//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;

        virtual tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgcDiv2
        (
            const dimensionedScalar&,
            const DgGeometricField<Type, dgPatchField, cellMesh>&
        ) = 0;

//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcLaplacian
//        (
//            const GeometricField<GType, dgPatchField, cellMesh>&,
//            const DgGeometricField<Type, dgPatchField, cellMesh>&
//        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeDiv2TypeScheme(SS, Type, GType)                            \
                                                                              \
    typedef SS<Type, GType> SS##Type##GType;                                  \
    defineNamedTemplateTypeNameAndDebug(SS##Type##GType, 0);                  \
                                                                              \
    dgDiv2Scheme<Type, GType>::                                            \
        addIstreamConstructorToTable<SS<Type, GType> >                        \
    add##SS##Type##GType##IstreamConstructorToTable_;


#define makeDiv2Scheme(SS)                                             \
                                                                              \
makeDiv2TypeScheme(SS, dgScalar, scalar)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgDiv2Scheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
