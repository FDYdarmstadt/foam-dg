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

InNamespace
    Foam::dgm

Description
    Calculate the matrix for the dgCahnHilliard of the field.

SourceFiles
    dgmCahnHilliard.C

\*---------------------------------------------------------------------------*/

#ifndef dgmCahnHilliard_H
#define dgmCahnHilliard_H

#include "cellFieldsFwd.H"
#include "IOobjectList.H"
#include "IOdictionary.H"
#include "zeroField.H"
//#include "surfaceFieldsFwd.H"
#include "dgMatrices.H"
#include "geometricOneField.H"
#include "dgmCahnHilliard.H"
#include "dgCahnHilliardScheme.H"
#include "bosssCahnHilliardScheme.H"

// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// /*---------------------------------------------------------------------------*\
//                      Namespace dgm functions Declaration
// \*---------------------------------------------------------------------------*/

namespace dgm
{

template<class Type>//, class GType>
tmp<dgMatrix<Type> >
dgCahnHilliard
(
    // const dimensionedScalar& gamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<dgVector, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<dgVector, dgPatchField, cellMesh>& phif,
    const word& name
)
{
    return dg::dgCahnHilliardScheme<Type, dgVector>::New
    (
        vf.mesh(),
        vf.mesh().schemesDict().dgCahnHilliardScheme(name)
    )().dgmCahnHilliard(vf, Uf, phif);
}

template<class Type, class VType>
tmp<dgMatrix<Type> >
dgCahnHilliard
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& phif
)
{

//    surfaceScalarField Gamma
//    (
//        IOobject
//        (
//            "1",
//            vf.time().constant(),
//            vf.mesh(),
//            IOobject::NO_READ
//        ),
//        vf.mesh(),
//        dimensionedScalar("1", dimless, 1.0)
//    );
//
    return dgm::dgCahnHilliard
    (
        vf,
        Uf,
        phif,
        "dgCahnHilliard(" + vf.name() + Uf.name() + ')'
    );
}

template<class Type, class VType>
tmp<dgMatrix<Type> >
dgCahnHilliard
(
    const geometricOneField&,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& phif,
    const word& name
)
{
    return dgm::dgCahnHilliard(vf, Uf, phif, name);
}


template<class Type, class VType>
tmp<dgMatrix<Type> >
dgCahnHilliard
(
    const geometricOneField&,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& phif
)
{
    return dgm::dgCahnHilliard(vf, Uf, phif); //Geo one field
}

// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     // const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& phif
// )
// {
//     // tmp<dgMatrix<Type> > CahnHilliardOp(dgm::dgCahnHilliard(tgamma(), vf));
//     tmp<dgMatrix<Type> > CahnHilliardOp(dgm::dgCahnHilliard(vf, Uf, phif));
//     // tgamma.clear();
//     return CahnHilliardOp;
// }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class VType>
tmp<dgMatrix<Type> >
dgCahnHilliard
(
   // const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
   const GeometricField<Type, dgPatchField, cellMesh>& vf,
   const GeometricField<VType, dgPatchField, cellMesh>& Uf,
   const GeometricField<VType, dgPatchField, cellMesh>& phif,
   const word& name
)
{
   return dg::dgCahnHilliardScheme<Type, VType>::New
   (
       vf.mesh(),
       vf.mesh().schemesDict().dgCahnHilliardScheme(name)
   )().dgmCahnHilliard(vf, Uf, phif);
}

template<class Type, class VType>
tmp<dgMatrix<Type> >
dgCahnHilliard
(
   // const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
   const GeometricField<Type, dgPatchField, cellMesh>& vf,
   const GeometricField<VType, dgPatchField, cellMesh>& Uf,
   const GeometricField<VType, dgPatchField, cellMesh>& phif
)
{
   return dgm::dgCahnHilliard
   (
       // gamma,
       vf,
       Uf,
       phif,
       "dgCahnHilliard( vf.name() + Uf.name() + ')'"
   );
}
//     template<class Type>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const word&
//     );

//     template<class Type>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&
//     );


//    template<class Type>
//    tmp<dgMatrix<Type> > dgCahnHilliard
//    (
//        const zeroField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word&
//    );

//    template<class Type>
//    tmp<dgMatrix<Type> > dgCahnHilliard
//    (
//        const zeroField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );


//     template<class Type>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const geometricOneField&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const word&
//     );

//     template<class Type>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const geometricOneField&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&
//     );


// //    template<class Type, class GType>
// //    tmp<dgMatrix<Type> > dgCahnHilliard
// //    (
// //        const dimensioned<GType>&,
// //        const GeometricField<Type, dgPatchField, cellMesh>&,
// //        const word&
// //    );

//     template<class Type, class GType>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const dimensioned<GType>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&
//     );

// // WE USE THIS:
//     template<class Type>//, class GType>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         // const dimensionedScalar&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const word&
//     );

//     template<class Type, class GType>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const DgGeometricField<GType, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&
//     );


//     template<class Type, class GType>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const word&
//     );

//     template<class Type, class GType>
//     tmp<dgMatrix<Type> > dgCahnHilliard
//     (
//         const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&,
//         const DgGeometricField<Type, dgPatchField, cellMesh>&
//     );


// //    template<class Type, class GType>
// //    tmp<dgMatrix<Type> > dgCahnHilliard
// //    (
// //        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
// //        const GeometricField<Type, dgPatchField, cellMesh>&,
// //        const word&
// //    );
// //
// //    template<class Type, class GType>
// //    tmp<dgMatrix<Type> > dgCahnHilliard
// //    (
// //        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
// //        const GeometricField<Type, dgPatchField, cellMesh>&,
// //        const word&
// //    );
// //
// //    template<class Type, class GType>
// //    tmp<dgMatrix<Type> > dgCahnHilliard
// //    (
// //        const GeometricField<GType, dgsPatchField, surfaceMesh>&,
// //        const GeometricField<Type, dgPatchField, cellMesh>&
// //    );
// //
// //    template<class Type, class GType>
// //    tmp<dgMatrix<Type> > dgCahnHilliard
// //    (
// //        const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >&,
// //        const GeometricField<Type, dgPatchField, cellMesh>&
// //    );
}


// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgmCahnHilliard.C"
#endif

// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// // ************************************************************************* //
