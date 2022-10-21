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

#include "dgcCahnHilliard.H"
#include "dgMesh.H"
#include "dgCahnHilliardScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dgc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<Type, dgPatchField, cellMesh>& phif,
    const word& name
)
{

    return dgc::dgCahnHilliard(vf, Uf, phif, name);


//    return dg::dgCahnHilliard<Type, scalar>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgCahnHilliard(name)
//    )().dgcCahnHilliard(vf);
}


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >& tUf,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tPhif,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
    (
        dgc::dgCahnHilliard(tvf(), name)
    );
    tvf.clear();
    return CahnHilliard;
}


//template<class Type>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const dimensionedScalar& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgc::dgCahnHilliard(gamma, vf, "dgCahnHilliard(" + vf.name() + ')');
//}


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<Type, dgPatchField, cellMesh>& phif
)
{
    return dgc::dgCahnHilliard(vf, Uf, phif, "dgCahnHilliard(" + vf.name() + Uf.name() + ')');
}


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >& tUf,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tPhif
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
    (
        dgc::dgCahnHilliard(tvf(), tUf(), tPhif())
    );
    tvf.clear();
    tUf.clear();
    tPhif.clear();
    return CahnHilliard;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const dimensioned<GType>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return gamma*dgc::dgCahnHilliard(vf, name);
//}


// template<class Type, class VType>
// tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
// dgCahnHilliard
// (
//     const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
//     const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >& tUf,
//     const word& name
// )
// {
//     tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//     (
//         dgc::dgCahnHilliard(tvf(), Uvf(), name)
//     );
//     tvf.clear();
//     tUf.clear();
//     return CahnHilliard;
// }


// template<class Type, class VType>
// tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
// dgCahnHilliard
// (
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
//     const DgGeometricField<Type, dgPatchField, cellMesh>& Phif
// )
// {
//     return dgc::dgCahnHilliard
//     (
//         // vf, Uf, Phif, "dgCahnHilliard(" + vf.name() + ',' + Uf.name() + ')'
//         vf, Uf, Phif, "dgCahnHilliard(" + vf.name() + Uf.name() + ')'
//     );
// }


// template<class Type, class VType>
// tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
// dgCahnHilliard
// (
//     const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
//     const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >& tUf
// )
// {
//     tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//     (
//         dgc::dgCahnHilliard(gamma, tvf())
//     );
//     tvf.clear();
//     return CahnHilliard;
// }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>//, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const DgGeometricField<dgVector, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<dgScalar, dgPatchField, cellMesh>& Phif,
    const word& name
)
{
    Info << "DGC LAPLACIAN" << endl;

    return dg::dgCahnHilliardScheme<Type, dgVector>::New
    (
        vf.mesh(),
        vf.mesh().schemesDict().dgCahnHilliard(name)
    )().dgcCahnHilliard(vf, Uf, Phif);
}


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const tmp<DgGeometricField<VType, dgPatchField, cellMesh>>& Uf,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh>>& Phif,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
    (
        dgc::dgCahnHilliard(vf, Uf(), Phif(), name)
    );
    Uf.clear();
    Phif.clear();
    return CahnHilliard;
}


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const DgGeometricField<VType, dgPatchField, cellMesh> & Uf,
    const DgGeometricField<Type, dgPatchField, cellMesh> & Phif,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
    (
        dgc::dgCahnHilliard(tvf(), Uf, Phif, name)
    );
    tvf.clear();
    return CahnHilliard;
}


// template<class Type, class VType>
// tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
// dgCahnHilliard
// (
//     const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
//     const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >& tUf,
//     const word& name
// )
// {
//     tmp<DgGeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//     (
//         dgc::dgCahnHilliard(tgamma(), tvf(), name)
//     );
//     tgamma.clear();
//     tvf.clear();
//     return CahnHilliard;
// }


// template<class Type, class VType>
// tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
// dgCahnHilliard
// (
//     const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
//     const DgGeometricField<VType, dgPatchField, cellMesh>& Uf
// )
// {
//     return dgc::dgCahnHilliard
//     (
//         vf,
//         Uf,
//         "dgCahnHilliard(" + vf.name() + ',' + Uf.name() + ')'
//     );
// }


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    // const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const tmp<DgGeometricField<VType, dgPatchField, cellMesh>>& Uf,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh>>& Phif
)
{
    return dgc::dgCahnHilliard
    (
        vf,
        Uf,
        Phif,
        // "dgCahnHilliard(" + vf.name() + ',' + Uf().name() + ')'
        "dgCahnHilliard(" + vf.name() + Uf().name() + ')'
    );
}


template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgCahnHilliard
(
    // const DgGeometricField<GType, dgPatchField, cellMesh>& gamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    const DgGeometricField<Type, dgPatchField, cellMesh>& Phif
)
{
    return dgc::dgCahnHilliard
    (
        tvf,
        Uf,
        Phif,
        // "dgCahnHilliard(" + tvf().name() + ',' + Uf.name() + ')'
        "dgCahnHilliard(" + tvf().name() + Uf.name() + ')'
    );
}


// template<class Type, class VType>
// tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
// dgCahnHilliard
// (
//     // const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
//     const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
//     const tmp<DgGeometricField<VType, dgPatchField, cellMesh> >& tUf
// )
// {
//     return dgc::dgCahnHilliard
//     (
//         tgamma,
//         tvf,
//         "dgCahnHilliard(" + tgamma().name() + ',' + tvf().name() + ')'
//     );
// }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return dg::dgCahnHilliard<Type, GType>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgCahnHilliard(name)
//    )().dgcCahnHilliard(gamma, vf);
//}


//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//    (
//        dgc::dgCahnHilliard(tgamma(), vf, name)
//    );
//    tgamma.clear();
//    return CahnHilliard;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf,
//    const word& name
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//    (
//        dgc::dgCahnHilliard(gamma, tvf(), name)
//    );
//    tvf.clear();
//    return CahnHilliard;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf,
//    const word& name
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//    (
//        dgc::dgCahnHilliard(tgamma(), tvf(), name)
//    );
//    tgamma.clear();
//    tvf.clear();
//    return CahnHilliard;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgc::dgCahnHilliard
//    (
//        gamma,
//        vf,
//        "dgCahnHilliard(" + gamma.name() + ',' + vf.name() + ')'
//    );
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//    (
//        dgc::dgCahnHilliard(tgamma(), vf)
//    );
//    tgamma.clear();
//    return CahnHilliard;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgCahnHilliard
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//    (
//        dgc::dgCahnHilliard(gamma, tvf())
//    );
//    tvf.clear();
//    return CahnHilliard;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> > dgCahnHilliard
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > CahnHilliard
//    (
//        dgc::dgCahnHilliard(tgamma(), tvf())
//    );
//    tgamma.clear();
//    tvf.clear();
//    return CahnHilliard;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dgc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
