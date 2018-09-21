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

#include "dgcVolumeIntegrate.H"
#include "dgMesh.H"
#include "Field.H"
#include "dgPolynomials.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dgc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class Type>
tmp<cellScalarField>
volumeIntegrate
(
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    // Local coordinates (reference element)
    scalarList gaussCoords = vf.mesh().gaussPoints();
    scalarList gaussWeights = vf.mesh().gaussWeights();

    tmp<cellScalarField> tCellIntegral(vf, 0);

    dgScalarField& cellIntegral = tCellIntegral().internalField();
    scalarField cellMax(vf.mesh().cellCentres().component(vector::X));
    scalarField cellMin(cellMax);

    dgPolynomials polynomials();

    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(gaussCoords, ptI)
    {
        // Calculate modal values in given coordinate (Gaussian point
        // coordinate)
        scalarField polyEval = polynomials.evaluate(gaussCoords[ptI]);

        forAll (cellIntegral, cellI)
        {
            forAll (polyEval, modI)
            {
                cellIntegral[cellI] +=
                    polyEval[modI]*vf.internalField()[cellI][modI];
            }


            // Determine min,max for each cell
            forAll (vf.mesh()[cellI], ptI)
            {
                if
                (
                    vf.mesh().cellPoints(cellI)[ptI] < cellMin[cellI]
                )
                {
                    cellMin[cellI] = vf.mesh().cellPoints(cellI)[ptI];
                }

                if (vf.mesh().cellPoints(cellI)[ptI] > cellMax[cellI])
                {
                    cellMax[cellI] = vf.mesh().cellPoints(cellI)[ptI];
                }
            }
        }

        // TO-DO:
        // Make two functions valueLocal() and valueGlobal() so that either
        // x=[min(cellBounds),max(cellBounds)] or x=[-1,1] can be provided

        // Multiply obtained value with Gaussian weight to assemble the integral
//        cellIntegral += gaussWeights[ptI]*value;
    }

    // Normalize and scale based on real cell size (not reference one)
    cellIntegral *= (cellMax - cellMin)/2;

    return tCellIntegral;
//    return vf.mesh().V()*vf.internalField();
}



template<class Type>
tmp<scalarField>//Field<Type> >
volumeIntegrate
(
    const Field<Type>& vf
)
{
    dgPolynomials polynomials(3);

    polynomials.test();
//
//    // Local coordinates (reference element)
    scalarField gaussCoords = polynomials.gaussPoints();
    scalarField gaussWeights = polynomials.gaussWeights();


// TU MORAM RECI AKO JE RED POLINOMA MANJI OD 2, RADI S DVIJE TOCKE KOJE SU...
// (ili u polynomials gauss Weights)
    tmp<scalarField> tCellIntegral(new scalarField(vf.size(), 0.0));
    scalarField& cellIntegral = tCellIntegral();

//    scalarField cellMax(vf.mesh().cellCentres().component(vector::X));
//    scalarField cellMin(cellMax);

//    Info << "coeffs: " << vf[0]  << ", and weights: " << gaussWeights<< endl;

    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(gaussCoords, ptI)
    {
        // Calculate modal values in given coordinate (Gaussian point
        // coordinate)
        scalarField polyEval =
            polynomials.evaluate(vector(gaussCoords[ptI],0,0));

    Info << "POLY EVAL: " << polyEval << " in " << gaussCoords[ptI]
         << endl;
//      << ", summed: " << gSum(polyEval)
//         << ", and cellIntegral: " << cellIntegral << endl;

        forAll (cellIntegral, cellI)
        {
            forAll (polyEval, modI)
            {
                cellIntegral[cellI] +=
                    polyEval[modI]*vf[cellI][modI]*gaussWeights[ptI];

//                Info << nl << polyEval[modI] << ", " << vf[cellI][modI] << ", "
//                     << gaussWeights[modI]
//                     << ", CELL INTEGRAL: " << cellIntegral[cellI] << endl;
            }
        }

//            // Determine min,max for each cell
//            forAll (vf.mesh()[cellI], ptI)
//            {
//                if
//                (
//                    vf.mesh().cellPoints(cellI)[ptI] < cellMin[cellI]
//                )
//                {
//                    cellMin[cellI] = vf.mesh().cellPoints(cellI)[ptI];
//                }
//
//                if (vf.mesh().cellPoints(cellI)[ptI] > cellMax[cellI])
//                {
//                    cellMax[cellI] = vf.mesh().cellPoints(cellI)[ptI];
//                }
//            }
//        }

        // TO-DO:
        // Make two functions valueLocal() and valueGlobal() so that either
        // x=[min(cellBounds),max(cellBounds)] or x=[-1,1] can be provided

        // Multiply obtained value with Gaussian weight to assemble the integral
//        cellIntegral += gaussWeights[ptI]*value;
    }

    // Normalize and scale based on real cell size (not reference one)
//    cellIntegral *= (cellMax - cellMin)/2;

    return tCellIntegral;
//    return vf.mesh().V()*vf.internalField();
}

template<class Type>
tmp<Field<Type> >
volumeIntegrate
(
    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    tmp<Field<Type> > tvivf = tvf().mesh().V()*tvf().internalField();
    tvf.clear();
    return tvivf;
}


template<class Type>
dimensioned<Type>
domainIntegrate
(
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    return dimensioned<Type>
    (
        "domainIntegrate(" + vf.name() + ')',
        dimVol*vf.dimensions(),
        gSum(dgc::volumeIntegrate(vf))
    );
}

template<class Type>
dimensioned<Type>
domainIntegrate
(
    const tmp<GeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    dimensioned<Type> integral = domainIntegrate(tvf());
    tvf.clear();
    return integral;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dgc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
