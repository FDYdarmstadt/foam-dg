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

#include "EulerDdtScheme.H"
//#include "surfaceInterpolate.H"
//#include "dgcDiv.H"
#include "dgMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//EulerDdtScheme<Type>::fvcDdt
//(
//    const dimensioned<Type>& dt
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    IOobject ddtIOobject
//    (
//        "ddt("+dt.name()+')',
//        mesh().time().timeName(),
//        mesh()
//    );
//
//    if (mesh().moving())
//    {
//        tmp<GeometricField<Type, dgPatchField, cellMesh> > tdtdt
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                mesh(),
//                dimensioned<Type>
//                (
//                    "0",
//                    dt.dimensions()/dimTime,
//                    pTraits<Type>::zero
//                )
//            )
//        );
//
//        tdtdt().internalField() =
//            rDeltaT.value()*dt.value()*(1.0 - mesh().V0()/mesh().V());
//
//        return tdtdt;
//    }
//    else
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                mesh(),
//                dimensioned<Type>
//                (
//                    "0",
//                    dt.dimensions()/dimTime,
//                    pTraits<Type>::zero
//                ),
//                calculatedFvPatchField<Type>::typeName
//            )
//        );
//    }
//}
//
//
//template<class Type>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//EulerDdtScheme<Type>::fvcDdt
//(
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    IOobject ddtIOobject
//    (
//        "ddt("+vf.name()+')',
//        mesh().time().timeName(),
//        mesh()
//    );
//
//    if (mesh().moving())
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                mesh(),
//                rDeltaT.dimensions()*vf.dimensions(),
//                rDeltaT.value()*
//                (
//                    vf.internalField()
//                  - vf.oldTime().internalField()*mesh().V0()/mesh().V()
//                ),
//                rDeltaT.value()*
//                (
//                    vf.boundaryField() - vf.oldTime().boundaryField()
//                )
//            )
//        );
//    }
//    else
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                rDeltaT*(vf - vf.oldTime())
//            )
//        );
//    }
//}
//
//
//template<class Type>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//EulerDdtScheme<Type>::fvcDdt
//(
//    const dimensionedScalar& rho,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    IOobject ddtIOobject
//    (
//        "ddt("+rho.name()+','+vf.name()+')',
//        mesh().time().timeName(),
//        mesh()
//    );
//
//    if (mesh().moving())
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                mesh(),
//                rDeltaT.dimensions()*rho.dimensions()*vf.dimensions(),
//                rDeltaT.value()*rho.value()*
//                (
//                    vf.internalField()
//                  - vf.oldTime().internalField()*mesh().V0()/mesh().V()
//                ),
//                rDeltaT.value()*rho.value()*
//                (
//                    vf.boundaryField() - vf.oldTime().boundaryField()
//                )
//            )
//        );
//    }
//    else
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                rDeltaT*rho*(vf - vf.oldTime())
//            )
//        );
//    }
//}
//
//
//template<class Type>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//EulerDdtScheme<Type>::fvcDdt
//(
//    const volScalarField& rho,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    IOobject ddtIOobject
//    (
//        "ddt("+rho.name()+','+vf.name()+')',
//        mesh().time().timeName(),
//        mesh()
//    );
//
//    if (mesh().moving())
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                mesh(),
//                rDeltaT.dimensions()*rho.dimensions()*vf.dimensions(),
//                rDeltaT.value()*
//                (
//                    rho.internalField()*vf.internalField()
//                  - rho.oldTime().internalField()
//                   *vf.oldTime().internalField()*mesh().V0()/mesh().V()
//                ),
//                rDeltaT.value()*
//                (
//                    rho.boundaryField()*vf.boundaryField()
//                  - rho.oldTime().boundaryField()
//                   *vf.oldTime().boundaryField()
//                )
//            )
//        );
//    }
//    else
//    {
//        return tmp<GeometricField<Type, dgPatchField, cellMesh> >
//        (
//            new GeometricField<Type, dgPatchField, cellMesh>
//            (
//                ddtIOobject,
//                rDeltaT*(rho*vf - rho.oldTime()*vf.oldTime())
//            )
//        );
//    }
//}
//
//
template<class Type>
tmp<dgMatrix<Type> >
EulerDdtScheme<Type>::dgmDdt
(
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    tmp<dgMatrix<Type> > tdgm
    (
        new dgMatrix<Type>
        (
            vf,
            vf.dimensions()
        )
    );

    const dgMesh& mesh = dgDdtScheme<Type>::mesh();

    dgMatrix<Type>& dgm = tdgm();

    scalar rDeltaT = 1.0/mesh.time().deltaT().value();

    dgScalar rDel(rDeltaT);


    const dgBase& polynomials = mesh.polynomials();

    const PtrList<scalarField>& polyEval = polynomials.gaussPtsEval();
    const PtrList<scalarField>& polyEvalW = polynomials.wtdGaussEval();
    const scalarField& scaleCells = mesh.cellScaleCoeffs();


    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField&
        ds = dgm.diag().asSquare();

    forAll(vf, cellI)
    {
        forAll (polynomials, modJ)
        {
            forAll (polynomials, modI)
            {
                for (label ptI = 1; ptI < (polyEval.size() - 1); ptI++)
                {
                    ds[cellI](modJ, modI) +=
                        rDeltaT*polyEval[ptI][modI]*polyEvalW[ptI][modJ]/2
                        *mesh.mesh().cellVolumes()[cellI];

                    if (mag(ds[cellI](modJ, modI)) < SMALL)
                    {
                        ds[cellI](modJ, modI) = 0;
                    }
                }

                dgm.source()[cellI][modJ] += ds[cellI](modJ, modI)
                    *vf.oldTime().internalField()[cellI][modJ];
            }
        }
    }


    return tdgm;
}


template<class Type>
tmp<dgMatrix<Type> >
EulerDdtScheme<Type>::dgmDdt
(
    const dimensionedDgScalar& rho,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    tmp<dgMatrix<Type> > tfvm
    (
        new dgMatrix<Type>
        (
            vf,
            vf.dimensions()
        )
    );

    return tfvm;
}


template<class Type>
tmp<dgMatrix<Type> >
EulerDdtScheme<Type>::dgmDdt
(
    const cellScalarField& rho,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    tmp<dgMatrix<Type> > tfvm
    (
        new dgMatrix<Type>
        (
            vf,
            vf.dimensions()
        )
    );

    return tfvm;
}
//
//
//template<class Type>
//tmp<typename EulerDdtScheme<Type>::fluxFieldType>
//EulerDdtScheme<Type>::fvcDdtPhiCorr
//(
//    const volScalarField& rA,
//    const GeometricField<Type, dgPatchField, cellMesh>& U,
//    const fluxFieldType& phiAbs
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    IOobject ddtIOobject
//    (
//        "ddtPhiCorr(" + rA.name() + ',' + U.name() + ',' + phiAbs.name() + ')',
//        mesh().time().timeName(),
//        mesh()
//    );
//
//    tmp<fluxFieldType> phiCorr =
//        phiAbs.oldTime() - (fvc::interpolate(U.oldTime()) & mesh().Sf());
//
//    return tmp<fluxFieldType>
//    (
//        new fluxFieldType
//        (
//            ddtIOobject,
//            this->fvcDdtPhiCoeff(U.oldTime(), phiAbs.oldTime(), phiCorr())
//           *fvc::interpolate(rDeltaT*rA)*phiCorr
//        )
//    );
//}
//
//
//template<class Type>
//tmp<typename EulerDdtScheme<Type>::fluxFieldType>
//EulerDdtScheme<Type>::fvcDdtPhiCorr
//(
//    const volScalarField& rA,
//    const volScalarField& rho,
//    const GeometricField<Type, dgPatchField, cellMesh>& U,
//    const fluxFieldType& phiAbs
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    IOobject ddtIOobject
//    (
//        "ddtPhiCorr("
//      + rA.name() + ','
//      + rho.name() + ','
//      + U.name() + ','
//      + phiAbs.name() + ')',
//        mesh().time().timeName(),
//        mesh()
//    );
//
//    if
//    (
//        U.dimensions() == dimVelocity
//     && phiAbs.dimensions() == dimVelocity*dimArea
//    )
//    {
//        return tmp<fluxFieldType>
//        (
//            new fluxFieldType
//            (
//                ddtIOobject,
//                rDeltaT
//               *this->fvcDdtPhiCoeff(U.oldTime(), phiAbs.oldTime())
//               *(
//                   fvc::interpolate(rA*rho.oldTime())*phiAbs.oldTime()
//                 - (fvc::interpolate(rA*rho.oldTime()*U.oldTime())
//                  & mesh().Sf())
//                )
//            )
//        );
//    }
//    else if
//    (
//        U.dimensions() == dimVelocity
//     && phiAbs.dimensions() == rho.dimensions()*dimVelocity*dimArea
//    )
//    {
//        return tmp<fluxFieldType>
//        (
//            new fluxFieldType
//            (
//                ddtIOobject,
//                rDeltaT
//               *this->fvcDdtPhiCoeff
//                (
//                    U.oldTime(),
//                    phiAbs.oldTime()/fvc::interpolate(rho.oldTime())
//                )
//               *(
//                   fvc::interpolate(rA*rho.oldTime())
//                  *phiAbs.oldTime()/fvc::interpolate(rho.oldTime())
//                 - (
//                       fvc::interpolate
//                       (
//                           rA*rho.oldTime()*U.oldTime()
//                       ) & mesh().Sf()
//                   )
//                )
//            )
//        );
//    }
//    else if
//    (
//        U.dimensions() == rho.dimensions()*dimVelocity
//     && phiAbs.dimensions() == rho.dimensions()*dimVelocity*dimArea
//    )
//    {
//        return tmp<fluxFieldType>
//        (
//            new fluxFieldType
//            (
//                ddtIOobject,
//                rDeltaT
//               *this->fvcDdtPhiCoeff
//                (
//                    rho.oldTime(),
//                    U.oldTime(),
//                    phiAbs.oldTime()
//                )
//               *(
//                   fvc::interpolate(rA)*phiAbs.oldTime()
//                 - (fvc::interpolate(rA*U.oldTime()) & mesh().Sf())
//                )
//            )
//        );
//    }
//    else
//    {
//        FatalErrorIn
//        (
//            "EulerDdtScheme<Type>::fvcDdtPhiCorr"
//        )   << "dimensions of phiAbs are not correct"
//            << abort(FatalError);
//
//        return fluxFieldType::null();
//    }
//}
//
//
//template<class Type>
//tmp<typename EulerDdtScheme<Type>::fluxFieldType>
//EulerDdtScheme<Type>::fvcDdtConsistentPhiCorr
//(
//    const GeometricField<Type, fvsPatchField, surfaceMesh>& faceU,
//    const GeometricField<Type, dgPatchField, cellMesh>& U,
//    const surfaceScalarField& rAUf
//)
//{
//    tmp<fluxFieldType> toldTimeFlux =
//        (mesh().Sf() & faceU.oldTime())*rAUf/mesh().time().deltaT();
//
//    if (mesh().moving())
//    {
//        // Mesh is moving, need to take into account the ratio between old and
//        // current cell volumes
//        volScalarField V0ByV
//        (
//            IOobject
//            (
//                "V0ByV",
//                mesh().time().timeName(),
//                mesh(),
//                IOobject::NO_READ,
//                IOobject::NO_WRITE
//            ),
//            mesh(),
//            dimensionedScalar("one", dimless, 1.0),
//            zeroGradientFvPatchScalarField::typeName
//        );
//        V0ByV.internalField() = mesh().V0()/mesh().V();
//        V0ByV.correctBoundaryConditions();
//
//        // Correct the flux with interpolated volume ratio
//        toldTimeFlux() *= fvc::interpolate(V0ByV);
//    }
//
//    return toldTimeFlux;
//}
//
//
//template<class Type>
//tmp<surfaceScalarField> EulerDdtScheme<Type>::meshPhi
//(
//    const GeometricField<Type, dgPatchField, cellMesh>&
//)
//{
//    return mesh().phi();
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
