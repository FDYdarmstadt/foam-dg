/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
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

#include "dgmDdt.H"
//#include "volFields.H"
//#include "surfaceFields.H"
#include "dgDdtScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dgm
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// This should be used
template<class Type>
tmp<dgMatrix<Type> >
dgDdt
(
    DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    return dg::dgDdtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().schemesDict().dgDdtScheme(name)
    )().dgmDdt(vf);
}


template<class Type>
tmp<dgMatrix<Type> >
dgDdt
(
    const geometricOneField&,
    DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    const word& name
)
{
    return dgm::dgDdt(vf, name);
}


//template<class Type>
//tmp<dgMatrix<Type> >
//dgDdt
//(
//    const dimensionedScalar& rho,
//    GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return dg::dgDdtScheme<Type>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgDdtScheme(name)
//    )().dgmDdt(rho, vf);
//}


//template<class Type>
//tmp<dgMatrix<Type> >
//dgDdt
//(
//    const volScalarField& rho,
//    GeometricField<Type, dgPatchField, cellMesh>& vf,
//    const word& name
//)
//{
//    return dg::dgDdtScheme<Type>::New
//    (
//        vf.mesh(),
//        vf.mesh().schemesDict().dgDdtScheme(name)
//    )().dgmDdt(rho, vf);
//}


template<class Type>
tmp<dgMatrix<Type> >
dgDdt
(
    DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgm::dgDdt(vf, "dgDdt(" + vf.name() + ')');
}


template<class Type>
tmp<dgMatrix<Type> >
dgDdt
(
    const geometricOneField&,
    DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dgm::dgDdt(vf);
}


//template<class Type>
//tmp<dgMatrix<Type> >
//dgDdt
//(
//    const dimensionedScalar& rho,
//    GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgm::dgDdt(rho, vf, "dgDdt(" + rho.name() + ',' + vf.name() + ')');
//}
//
//
//template<class Type>
//tmp<dgMatrix<Type> >
//dgDdt
//(
//    const volScalarField& rho,
//    GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgm::dgDdt(rho, vf, "dgDdt(" + rho.name() + ',' + vf.name() + ')');
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dgm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
