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

\*---------------------------------------------------------------------------*/

// #include "dgmCahnHilliard.H"
// #include "dgCahnHilliardScheme.H"
// #include "bosssCahnHilliardScheme.H"
// #include "IOobjectList.H"
// #include "IOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// namespace Foam
// {

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// namespace dgm
// {

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
//     const word& name
// )
// {
//     IOdictionary dict
//     (
//         IOobject
//         (
//             "transportProperties",
//             vf.time().constant(),
//             vf.mesh()(),
//             IOobject::MUST_READ,
//             IOobject::NO_WRITE
//         )
//     );

//     // const dimensionedScalar  Gamma
//     // (
//     //     "Gamma",
//     //     vf.dimensions(),
//     //     readScalar(dict.lookup("gamma"))
//     // );




//     // return dgm::dgCahnHilliard(Gamma, vf, name);
//     return dgm::dgCahnHilliard(vf, Uf, name);
// }


// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {

// //    surfaceScalarField Gamma
// //    (
// //        IOobject
// //        (
// //            "1",
// //            vf.time().constant(),
// //            vf.mesh(),
// //            IOobject::NO_READ
// //        ),
// //        vf.mesh(),
// //        dimensionedScalar("1", dimless, 1.0)
// //    );
// //
//     return dgm::dgCahnHilliard
//     (
//         vf,
//         Uf,
//         "dgCahnHilliard(" + vf.name() + Uf.name() + ')'
//     );
// }


//template<class Type>
//tmp<dgMatrix<Type> >
//dgLaplacian
//(
//    const zeroField&,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return tmp<dgMatrix<Type> >
//    (
//        new dgMatrix<Type>(vf, dimensionSet(0, 0, -2, 0, 0))
//    );
//}
//
//
//template<class Type>
//tmp<dgMatrix<Type> >
//dgLaplacian
//(
//    const zeroField&,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return tmp<dgMatrix<Type> >
//    (
//        new dgMatrix<Type>(vf, dimensionSet(0, 0, -2, 0, 0))
//    );
//}


// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     const geometricOneField&,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
//     const word& name
// )
// {
//     return dgm::dgCahnHilliard(vf, Uf, name);
// }


// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     const geometricOneField&,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {
//     return dgm::dgCahnHilliard(vf, Uf); //Geo one field
// }


// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     // const dimensioned<GType>& gamma,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {
// //    GeometricField<GType, dgsPatchField, surfaceMesh> Gamma
// //    (
// //        IOobject
// //        (
// //            gamma.name(),
// //            vf.instance(),
// //            vf.mesh(),
// //            IOobject::NO_READ
// //        ),
// //        vf.mesh(),
// //        gamma
// //    );
// //
// //    return dgm::dgLaplacian(Gamma, vf);
// }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// template<class Type>//, class GType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     // const dimensionedScalar& gamma,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<dgVector, dgPatchField, cellMesh>& Uf,
//     const word& name
// )
// {
//     return dg::dgCahnHilliardScheme<Type, dgVector>::New
//     (
//         vf.mesh(),
//         vf.mesh().schemesDict().dgCahnHilliardScheme(name)
//     )().dgmCahnHilliard(vf, Uf);
// }


// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     // const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
//     const word& name
// )
// {
//     // tmp<dgMatrix<Type> > CahnHilliardOp(dgm::dgCahnHilliard(tgamma(), vf, name));
//     tmp<dgMatrix<Type> > CahnHilliardOp(dgm::dgCahnHilliard(vf, Uf, name));
//     // tgamma.clear();
//     return CahnHilliardOp;
// }


// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     // const DgGeometricField<GType, dgPatchField, cellMesh>& gamma,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {
//     return dgm::dgCahnHilliard
//     (
//         // gamma,
//         vf,
//         Uf,
//         // "dgCahnHilliard(" + gamma.name() + ',' + vf.name() + ')'
//         "dgCahnHilliard(" + vf.name() + ',' + Uf.name() + ')'
//     );
// }


// template<class Type, class GType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//     // const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& Uf
// )
// {
//     // tmp<dgMatrix<Type> > CahnHilliardOp(dgm::dgCahnHilliard(tgamma(), vf));
//     tmp<dgMatrix<Type> > CahnHilliardOp(dgm::dgCahnHilliard(vf, Uf));
//     // tgamma.clear();
//     return CahnHilliardOp;
// }


// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//    // const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const GeometricField<VType, dgPatchField, cellMesh>& Uf,
//    const word& name
// )
// {
//    return dg::bosssCahnHilliardScheme<Type, VType>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgCahnHilliardScheme(name)
//    )().dgmCahnHilliard(vf, Uf);
// }
//
//
// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//    // const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const GeometricField<VType, dgPatchField, cellMesh>& Uf,
//    const word& name
// )
// {
//    tmp<dgMatrix<Type> > tCH = dgm::dgLaplacian(vf, Uf, name);
//    // tgamma.clear();
//    return tCH;
// }
//
//
// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgCahnHilliard
// (
//    // const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const GeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {
//    return dgm::dgCahnHilliard
//    (
//        // gamma,
//        vf,
//        Uf,
//        "dgCahnHilliard( vf.name() + Uf.name() + ')'"
//    );
// }
//
//
// template<class Type, class VType>
// tmp<dgMatrix<Type> >
// dgLaplacian
// (
//    // const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tGamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const GeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {
//    tmp<dgMatrix<Type> > tdgm(dgm::dgCahnHilliard(vf, Uf));
//    return tdgm;
// }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// } // End namespace dgm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// } // End namespace Foam

// ************************************************************************* //
