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

#include "dgmDiv.H"
#include "dgDivScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dgm
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type>
//tmp<dgMatrix<Type> >
//dgDiv
//(
////    const GeometricField<dgVector, dgPatchField, cellMesh>& vvf,
//    const GeometricField<Type, dgPatchField, cellMesh>& vsf,
//    const word& name
//)
//{
//    const dimensionedScalar  Gamma ("Gamma", vf.dimensions(), 0.0);

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

//    return dgm::dgDiv(vvf, vsf, name);
//}


template<class Type>
tmp<dgMatrix<Type> >
dgDiv
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
    return dgm::dgDiv
    (
        vf,
        "dgDiv(" + vf.name() + ')'
    );
}


//template<class Type>
//tmp<dgMatrix<Type> >
//dgDiv
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
//dgDiv
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
dgDiv
(
    const geometricOneField&,
    const GeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    return dgm::dgDiv(vf, name);
}


template<class Type>
tmp<dgMatrix<Type> >
dgDiv
(
    const geometricOneField&,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    Info << "TEST 2 " << endl;
    return dgm::dgDiv(vf); //Geo one field
}


//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgDiv
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
//    return dgm::dgDiv(Gamma, vf, name);
//}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgDiv
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
//    return dgm::dgDiv(Gamma, vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>//, class GType>
tmp<dgMatrix<Type> >
dgDiv
(
//    const dimensionedScalar& gamma,
    const GeometricField<dgVector, dgPatchField, cellMesh>& vvf,
    const GeometricField<Type, dgPatchField, cellMesh>& vsf,
    const word& name
)
{
    return dg::dgDivScheme<Type, scalar>::New
    (
        vsf.mesh(),
        vsf.mesh().schemesDict().dgDivScheme(name)
    )().dgmDiv(vvf, vsf);
}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgDiv
(
    const tmp<GeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    tmp<dgMatrix<Type> > Laplacian(dgm::dgDiv(tgamma(), vf, name));
    tgamma.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgDiv
(
    const GeometricField<GType, dgPatchField, cellMesh>& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgm::dgDiv
    (
        gamma,
        vf,
        "dgDiv(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<dgMatrix<Type> >
dgDiv
(
    const tmp<GeometricField<GType, dgPatchField, cellMesh> >& tgamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    tmp<dgMatrix<Type> > Laplacian(dgm::dgDiv(tgamma(), vf));
    tgamma.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgDiv
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return dg::dgDivScheme<Type, GType>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgDivScheme(name)
//    )().dgmDiv(gamma, vf);
//}
//
//
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgDiv
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tgamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    tmp<dgMatrix<Type> > tLaplacian = dgm::dgDiv(tgamma(), vf, name);
//    tgamma.clear();
//    return tLaplacian;
//}
//
//
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgDiv
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgm::dgDiv
//    (
//        gamma,
//        vf,
//        "dgDiv(" + gamma.name() + ',' + vf.name() + ')'
//    );
//}
//
//
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgDiv
//(
//    const tmp<GeometricField<GType, dgsPatchField, surfaceMesh> >& tGamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    tmp<dgMatrix<Type> > tdgm(dgm::dgDiv(tGamma(), vf));
//    tGamma.clear();
//    return tdgm;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dgm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
