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

#include "dgcLaplacian.H"
#include "dgMesh.H"
#include "dgLaplacianScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dgc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    const dimensionedScalar Gamma ("Gamma", vf.dimensions(), 0.0);

    return dgc::dgLaplacian(Gamma, vf, name);

//    return dg::dgLaplacianScheme<Type, scalar>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgLaplacianScheme(name)
//    )().dgcLaplacian(vf);
}


template<class Type>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


//template<class Type>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const dimensionedScalar& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgc::dgLaplacian(gamma, vf, "dgLaplacian(" + vf.name() + ')');
//}


template<class Type>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgc::dgLaplacian(vf, "dgLaplacian(" + vf.name() + ')');
}


template<class Type>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(tvf())
    );
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const dimensioned<GType>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return gamma*dgc::dgLaplacian(vf, name);
//}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const dimensioned<GType>& gamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const dimensioned<GType>& gamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return gamma*dgc::dgLaplacian
    (
        vf, "dgLaplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const dimensioned<GType>& gamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(gamma, tvf())
    );
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>//, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const dimensionedScalar& gamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    Info << "DGC LAPLACIAN" << endl;

    return dg::dgLaplacianScheme<Type, scalar>::New
    (
        vf.mesh(),
        vf.mesh().schemesDict().dgLaplacianScheme(name)
    )().dgcLaplacian(gamma, vf);
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(tgamma(), vf, name)
    );
    tgamma.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const DgGeometricField<GType, dgPatchField, cellMesh>& gamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf,
    const word& name
)
{
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > Laplacian
    (
        dgc::dgLaplacian(tgamma(), tvf(), name)
    );
    tgamma.clear();
    tvf.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const DgGeometricField<GType, dgPatchField, cellMesh>& gamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgc::dgLaplacian
    (
        gamma,
        vf,
        "dgLaplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgc::dgLaplacian
    (
        tgamma,
        vf,
        "dgLaplacian(" + tgamma().name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const DgGeometricField<GType, dgPatchField, cellMesh>& gamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    return dgc::dgLaplacian
    (
        gamma,
        tvf,
        "dgLaplacian(" + gamma.name() + ',' + tvf().name() + ')'
    );
}


template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
dgLaplacian
(
    const tmp<DgGeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    return dgc::dgLaplacian
    (
        tgamma,
        tvf,
        "dgLaplacian(" + tgamma().name() + ',' + tvf().name() + ')'
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return dg::dgLaplacianScheme<Type, GType>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgLaplacianScheme(name)
//    )().dgcLaplacian(gamma, vf);
//}


//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > Laplacian
//    (
//        dgc::dgLaplacian(tgamma(), vf, name)
//    );
//    tgamma.clear();
//    return Laplacian;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf,
//    const word& name
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > Laplacian
//    (
//        dgc::dgLaplacian(gamma, tvf(), name)
//    );
//    tvf.clear();
//    return Laplacian;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> > dgLaplacian
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf,
//    const word& name
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > Laplacian
//    (
//        dgc::dgLaplacian(tgamma(), tvf(), name)
//    );
//    tgamma.clear();
//    tvf.clear();
//    return Laplacian;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgc::dgLaplacian
//    (
//        gamma,
//        vf,
//        "dgLaplacian(" + gamma.name() + ',' + vf.name() + ')'
//    );
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > Laplacian
//    (
//        dgc::dgLaplacian(tgamma(), vf)
//    );
//    tgamma.clear();
//    return Laplacian;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacian
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > Laplacian
//    (
//        dgc::dgLaplacian(gamma, tvf())
//    );
//    tvf.clear();
//    return Laplacian;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> > dgLaplacian
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf
//)
//{
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > Laplacian
//    (
//        dgc::dgLaplacian(tgamma(), tvf())
//    );
//    tgamma.clear();
//    tvf.clear();
//    return Laplacian;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dgc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
