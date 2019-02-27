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

#include "dg.H"
#include "HashTable.H"
//#include "surfaceInterpolate.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<dgDdtScheme<Type> > dgDdtScheme<Type>::New
(
    const dgMesh& mesh,
    Istream& schemeData
)
{
    if (dg::debug)
    {
        Info<< "dgDdtScheme<Type>::New(const dgMesh&, Istream&) : "
               "constructing dgDdtScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorIn
        (
            "dgDdtScheme<Type>::New(const dgMesh&, Istream&)",
            schemeData
        )   << "Ddt scheme not specified" << nl << nl
            << "Valid dgDdt schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorIn
        (
            "dgDdtScheme<Type>::New(const dgMesh&, Istream&)",
            schemeData
        )   << "Unknown dgDdt scheme " << schemeName << nl << nl
            << "Valid dgDdt schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
dgDdtScheme<Type>::~dgDdtScheme()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type>
//tmp<surfaceScalarField> dgDdtScheme<Type>::dgcDdtPhiCoeff
//(
//    const GeometricField<Type, dgPatchField, cellMesh>& U,
//    const fluxFieldType& phi,
//    const fluxFieldType& phiCorr
//)
//{
//    tmp<surfaceScalarField> tdgDdtCouplingCoeff = scalar(1)
//      - min
//        (
//            mag(phiCorr)
//           /(mag(phi) + dimensionedScalar("small", phi.dimensions(), SMALL)),
//            scalar(1)
//        );
//
//    surfaceScalarField& dgDdtCouplingCoeff = tdgDdtCouplingCoeff();
//
//    forAll (U.boundaryField(), patchi)
//    {
//        if (U.boundaryField()[patchi].fixesValue())
//        {
//            dgDdtCouplingCoeff.boundaryField()[patchi] = 0.0;
//        }
//    }
//
//    if (debug > 1)
//    {
//        Info<< "dgDdtCouplingCoeff mean max min = "
//            << gAverage(dgDdtCouplingCoeff.internalField())
//            << " " << gMax(dgDdtCouplingCoeff.internalField())
//            << " " << gMin(dgDdtCouplingCoeff.internalField())
//            << endl;
//    }
//
//    return tdgDdtCouplingCoeff;
//}
//
//
//template<class Type>
//tmp<surfaceScalarField> dgDdtScheme<Type>::dgcDdtPhiCoeff
//(
//    const GeometricField<Type, dgPatchField, cellMesh>& U,
//    const fluxFieldType& phi
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    tmp<surfaceScalarField> tdgDdtCouplingCoeff = scalar(1)
//      - min
//        (
//            mag(phi - (mesh().Sf() & dgc::interpolate(U)))
//           /(mag(phi) + dimensionedScalar("small", phi.dimensions(), VSMALL)),
//           //(rDeltaT*mesh().magSf()/mesh().deltaCoeffs()),
//            scalar(1)
//        );
//
//    surfaceScalarField& dgDdtCouplingCoeff = tdgDdtCouplingCoeff();
//
//    forAll (U.boundaryField(), patchi)
//    {
//        if (U.boundaryField()[patchi].fixesValue())
//        {
//            dgDdtCouplingCoeff.boundaryField()[patchi] = 0.0;
//        }
//    }
//
//    if (debug > 1)
//    {
//        Info<< "dgDdtCouplingCoeff mean max min = "
//            << gAverage(dgDdtCouplingCoeff.internalField())
//            << " " << gMax(dgDdtCouplingCoeff.internalField())
//            << " " << gMin(dgDdtCouplingCoeff.internalField())
//            << endl;
//    }
//
//    return tdgDdtCouplingCoeff;
//}
//
//
//template<class Type>
//tmp<surfaceScalarField> dgDdtScheme<Type>::dgcDdtPhiCoeff
//(
//    const volScalarField& rho,
//    const GeometricField<Type, dgPatchField, cellMesh>& rhoU,
//    const fluxFieldType& phi
//)
//{
//    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();
//
//    tmp<surfaceScalarField> tdgDdtCouplingCoeff = scalar(1)
//      - min
//        (
//            mag(phi - (mesh().Sf() & dgc::interpolate(rhoU)))
//           /(
//                mag(phi) + dimensionedScalar("small", phi.dimensions(), VSMALL)
//                //dgc::interpolate(rho)*rDeltaT
//                //*mesh().magSf()/mesh().deltaCoeffs()
//            ),
//            scalar(1)
//        );
//
//    surfaceScalarField& dgDdtCouplingCoeff = tdgDdtCouplingCoeff();
//
//    forAll (rhoU.boundaryField(), patchi)
//    {
//        if (rhoU.boundaryField()[patchi].fixesValue())
//        {
//            dgDdtCouplingCoeff.boundaryField()[patchi] = 0.0;
//        }
//    }
//
//    if (debug > 1)
//    {
//        Info<< "dgDdtCouplingCoeff mean max min = "
//            << gAverage(dgDdtCouplingCoeff.internalField())
//            << " " << gMax(dgDdtCouplingCoeff.internalField())
//            << " " << gMin(dgDdtCouplingCoeff.internalField())
//            << endl;
//    }
//
//    return tdgDdtCouplingCoeff;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
