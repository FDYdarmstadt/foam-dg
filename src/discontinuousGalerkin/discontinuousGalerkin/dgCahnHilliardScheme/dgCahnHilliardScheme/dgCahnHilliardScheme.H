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
    Foam::dg::dgCahnHilliardScheme

Description
    Abstract base class for dgCahnHilliard schemes.

SourceFiles
    dgCahnHilliardScheme.C

\*---------------------------------------------------------------------------*/

#ifndef dgCahnHilliardScheme_H
#define dgCahnHilliardScheme_H

#include "tmp.H"
#include "cellFieldsFwd.H"
//#include "surfaceFieldsFwd.H"
//#include "linear.H"
//#include "correctedSnGrad.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "dimensionedTypes.H"

#include "surfaceFieldsFwd.H"

// #include "dg.H"
// #include "HashTable.H"
// //#include "linear.H"
// #include "dgMatrices.H"

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
                           Class dgCahnHilliardScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class VType>
class dgCahnHilliardScheme
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
        dgCahnHilliardScheme(const dgCahnHilliardScheme&){};

        //- Disallow default bitwise assignment
        void operator=(const dgCahnHilliardScheme&){};


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            dgCahnHilliardScheme,
            Istream,
            (const dgMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        dgCahnHilliardScheme(const dgMesh& mesh)
        :
            mesh_(mesh)
//            tinterpGammaScheme_(new linear<GType>(mesh)),
//            tsnGradScheme_(new correctedSnGrad<Type>(mesh))
        {}

        //- Construct from mesh and Istream
        dgCahnHilliardScheme(const dgMesh& mesh, Istream& is)
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
//        dgCahnHilliardScheme
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

        //- Return a pointer to a new dgCahnHilliardScheme created on freestore
        static tmp<dgCahnHilliardScheme<Type, VType> > New
        // static tmp<dgCahnHilliardScheme<Type, dgVector> > New
        (
            const dgMesh& mesh,
            Istream& schemeData
        );
        // {
//     if (schemeData.eof())
//     {
//         FatalIOErrorIn
//         (
//             "dgCahnHilliardScheme<Type, VType>::New(const dgMesh&, Istream&)",
//             schemeData
//         )   << "CahnHilliard scheme not specified" << nl << nl
//             << "Valid dgCahnHilliard schemes are :" << endl
//             << IstreamConstructorTablePtr_->sortedToc()
//             << exit(FatalIOError);
//     }

//     const word schemeName(schemeData);

//     typename IstreamConstructorTable::iterator cstrIter =
//         IstreamConstructorTablePtr_->find(schemeName);

//     if (cstrIter == IstreamConstructorTablePtr_->end())
//     {
//         FatalIOErrorIn
//         (
//             "dgCahnHilliardScheme<Type, VType>::New(const dgMesh&, Istream&)",
//             schemeData
//         )   << "Unknown dgCahnHilliard scheme " << schemeName << nl << nl
//             << "Valid dgCahnHilliard schemes are :" << endl
//             << IstreamConstructorTablePtr_->sortedToc()
//             << exit(FatalIOError);
//     }

//     return cstrIter()(mesh, schemeData);
// }


    // Destructor

        virtual ~dgCahnHilliardScheme(){}


    // Member Functions

        //- Return mesh reference
        const dgMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<dgMatrix<Type> > dgmCahnHilliard
        (
            // const dimensionedScalar&,
            DgGeometricField<Type, dgPatchField, cellMesh>& vf,
            DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
            // const DgGeometricField<VType, dgPatchField, cellMesh>& phif
            DgGeometricField<Type, dgPatchField, cellMesh>& phif,
            surfaceScalarField& Flux
        ) = 0;
// {
//    return dgmCahnHilliard(vf, Uf);//tinterpGammaScheme_().interpolate(gamma)(), vf);
// }

//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcCahnHilliard
//        (
//            const dimensionedScalar&,
//            const GeometricField<Type, dgPatchField, cellMesh>&
//        ) = 0;

        virtual tmp<DgGeometricField<Type, dgPatchField, cellMesh> > dgcCahnHilliard
        (
            // const dimensionedScalar&,
            DgGeometricField<Type, dgPatchField, cellMesh>& vf,
            DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
            DgGeometricField<Type, dgPatchField, cellMesh>& phif,
            surfaceScalarField& Flux
        ) = 0;
// {
//    return dgcCahnHilliard(vf, Uf);//tinterpGammaScheme_().interpolate(gamma)(), vf);
// }

//        virtual tmp<GeometricField<Type, dgPatchField, cellMesh> > dgcCahnHilliard
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

#define makeDgCahnHilliardTypeScheme(SS, Type, VType)                            \
                                                                              \
    typedef SS<Type, VType> SS##Type##VType;                                  \
    defineNamedTemplateTypeNameAndDebug(SS##Type##VType, 0);                  \
                                                                              \
    dgCahnHilliardScheme<Type, VType>::                                            \
        addIstreamConstructorToTable<SS<Type, VType> >                        \
    add##SS##Type##VType##IstreamConstructorToTable_;

#define makeDgCahnHilliardScheme(SS)                                             \
                                                                              \
makeDgCahnHilliardTypeScheme(SS, dgScalar, dgVector)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgCahnHilliardScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
