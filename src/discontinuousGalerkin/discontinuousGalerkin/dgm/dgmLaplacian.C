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

#include "dgmLaplacian.H"
#include "dgLaplacianScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dgm
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const GeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    const dimensionedScalar  Gamma ("Gamma", vf.dimensions(), 0.0);

//    cellScalarField Gamma
//    (
//        IOobject
//        (
//            "1",
//            vf.time().constant(),
//            vf.mesh(),
//            IOobject::MUST_READ
////            IOobject::NO_WRITE
//        ),
////        vf
//        vf.mesh()
////        dimensionedDgScalar("1", dimless, 1.0)
//    );

    return dgm::dgLaplacian(Gamma, vf, name);
}


template<class Type>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const GeometricField<Type, dgPatchField, cellMesh>& vf
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
    return dgm::dgLaplacian
    (
//        Gamma,
        vf,
        "dgLaplacian(" + vf.name() + ')'
    );
//    return dg::dgLaplacianScheme<Type, GType>::New
//    return dg::dgLaplacianScheme<Type, dimensionedScalar>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgLaplacianScheme(name)
//    )().dgmLaplacian(vf);

//   return dg::dgmLaplacian(vf); //Matrix
}


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


template<class Type>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const geometricOneField&,
    const GeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    return dgm::dgLaplacian(vf, name);
}


template<class Type>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const geometricOneField&,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    Info << "TEST 2 " << endl;
    return dgm::dgLaplacian(vf); //Geo one field
}


//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgLaplacian
//(
//    const dimensioned<GType>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    GeometricField<GType, dgsPatchField, surfaceMesh> Gamma
//    (
//        IOobject
//        (
//            gamma.name(),
//            vf.instance(),
//            vf.mesh(),
//            IOobject::NO_READ
//        ),
//        vf.mesh(),
//        gamma
//    );
//
//    return dgm::dgLaplacian(Gamma, vf, name);
//}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const dimensioned<GType>& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
//    GeometricField<GType, dgsPatchField, surfaceMesh> Gamma
//    (
//        IOobject
//        (
//            gamma.name(),
//            vf.instance(),
//            vf.mesh(),
//            IOobject::NO_READ
//        ),
//        vf.mesh(),
//        gamma
//    );
//
//    return dgm::dgLaplacian(Gamma, vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>//, class GType>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const dimensionedScalar& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    return dg::dgLaplacianScheme<Type, scalar>::New
    (
        vf.mesh(),
        vf.mesh().schemesDict().dgLaplacianScheme(name)
    )().dgmLaplacian(gamma, vf);
}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const tmp<GeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    tmp<dgMatrix<Type> > Laplacian(dgm::dgLaplacian(tgamma(), vf, name));
    tgamma.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const GeometricField<GType, dgPatchField, cellMesh>& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgm::dgLaplacian
    (
        gamma,
        vf,
        "dgLaplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgLaplacian
(
    const tmp<GeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    tmp<dgMatrix<Type> > Laplacian(dgm::dgLaplacian(tgamma(), vf));
    tgamma.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<dgMatrix<Type> >
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
//    )().dgmLaplacian(gamma, vf);
//}
//
//
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgLaplacian
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    tmp<dgMatrix<Type> > tLaplacian = dgm::dgLaplacian(tgamma(), vf, name);
//    tgamma.clear();
//    return tLaplacian;
//}
//
//
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgLaplacian
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgm::dgLaplacian
//    (
//        gamma,
//        vf,
//        "dgLaplacian(" + gamma.name() + ',' + vf.name() + ')'
//    );
//}
//
//
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgLaplacian
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tGamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    tmp<dgMatrix<Type> > tdgm(dgm::dgLaplacian(tGamma(), vf));
//    tGamma.clear();
//    return tdgm;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dgm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
