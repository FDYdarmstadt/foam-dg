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
#include "dgBase.H"

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
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{

// TU MORAM RECI AKO JE RED POLINOMA MANJI OD 2, RADI S DVIJE TOCKE KOJE SU...
// (ili u polynomials gauss Weights)
    tmp<cellScalarField> tCellIntegral(new cellScalarField("cellIntegral", vf));
    cellScalarField& cellIntegral = tCellIntegral();

    dgScalarField& cellIntegralInt = cellIntegral.internalField();

    cellIntegralInt = dgScalar::zero;

    // This should be handled in a nicer way
    const dgMesh& dgMesh = vf.mesh();
    scalarField cellIntInt = volumeIntegrate(vf.internalField(), dgMesh);

    dgScalarField dgsf (cellIntegralInt.size());

    forAll (cellIntInt, i)
    {
        cellIntegralInt[i] = dgScalar(cellIntInt[i]);
    }

    const polyMesh& mesh = vf.mesh()();
    const scalarField points = mesh.points().component(vector::X);

    scalarField cellMax(mesh.cellCentres().component(vector::X));
    scalarField cellMin(cellMax);

    forAll (cellIntegralInt, cellI)
    {
        const labelList& cellPts = mesh.cellPoints(cellI);

        // Determine min,max for each cell
        forAll (cellPts, ptI)
        {
            if
            (
                points[cellPts[ptI]] < cellMin[cellI]
            )
            {
                cellMin[cellI] = points[cellPts[ptI]];
            }

            if (points[cellPts[ptI]] > cellMax[cellI])
            {
                cellMax[cellI] = points[cellPts[ptI]];
            }
        }
    }

    // TO-DO:
    // Make two functions valueLocal() and valueGlobal() so that either
    // x=[min(cellBounds),max(cellBounds)] or x=[-1,1] can be provided

    // Normalize and scale based on real cell size (not reference one)
    cellIntegralInt = cellIntegralInt*(cellMax - cellMin)/2;

    return tCellIntegral;
}


template<class Type>
tmp<scalarField>//Field<Type> >
volumeIntegrate
(
    const Field<Type>& vf,
    const dgMesh& mesh
)
{
//    const polyMesh& mesh = vf.mesh()();
    // Random constructor - should be considered!
    const dgBase& polynomials = mesh.polynomials();

    // Local coordinates (reference element)
    scalarField gaussCoords = polynomials.gaussPoints();
    scalarField gaussWeights = polynomials.gaussWeights();

    tmp<scalarField> tCellIntegral(new scalarField(vf.size(), 0.0));
    scalarField& cellIntegral = tCellIntegral();

    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(gaussCoords, ptI)
    {
        // Calculate modal values in given coordinate (Gaussian point
        // coordinate)
        scalarField polyEval =
            polynomials.evaluate(vector(gaussCoords[ptI],0,0));

        forAll (cellIntegral, cellI)
        {
            forAll (polyEval, modI)
            {
                cellIntegral[cellI] +=
                    polyEval[modI]*vf[cellI][modI]*gaussWeights[ptI];
            }
        }
    }

    return tCellIntegral;
}


template<class Type>
tmp<Field<Type> >
volumeIntegrate
(
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf
)
{
    tmp<Field<Type> > tvivf = tvf().mesh().V()*tvf().internalField();
    tvf.clear();
    return tvivf;
}


template<class Type>
tmp<cellScalarField>
volumeIntegrateGrad
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{

// TU MORAM RECI AKO JE RED POLINOMA MANJI OD 2, RADI S DVIJE TOCKE KOJE SU...
// (ili u polynomials gauss Weights)
    tmp<cellScalarField> tCellIntegral(new cellScalarField("cellIntegral", vf));
    cellScalarField& cellIntegral = tCellIntegral();

    dgScalarField& cellIntegralInt = cellIntegral.internalField();

    cellIntegralInt = dgScalar::zero;

    const dgMesh& dgMesh = vf.mesh();
    const polyMesh& mesh = vf.mesh()();
    scalarField cellIntInt = volumeIntegrateGrad(vf.internalField(), dgMesh);

    dgScalarField dgsf (cellIntegralInt.size());

    // NOTE - INSERTED SQR INSTEAD OF IN LAPLACIAN CALCULATION
    forAll (cellIntInt, i)
    {
        cellIntegralInt[i] = dgScalar(cellIntInt[i]);
    }

    const scalarField points = mesh.points().component(vector::X);

    scalarField cellMax(mesh.cellCentres().component(vector::X));
    scalarField cellMin(cellMax);

    forAll (cellIntegralInt, cellI)
    {
        const labelList& cellPts = mesh.cellPoints(cellI);

        // Determine min,max for each cell
        forAll (cellPts, ptI)
        {
            if
            (
                points[cellPts[ptI]] < cellMin[cellI]
            )
            {
                cellMin[cellI] = points[cellPts[ptI]];
            }

            if (points[cellPts[ptI]] > cellMax[cellI])
            {
                cellMax[cellI] = points[cellPts[ptI]];
            }
        }
    }

    // TO-DO:
    // Make two functions valueLocal() and valueGlobal() so that either
    // x=[min(cellBounds),max(cellBounds)] or x=[-1,1] can be provided

    // Normalize and scale based on real cell size (not reference one)
    cellIntegralInt = cellIntegralInt*(cellMax - cellMin)/2;

    return tCellIntegral;
}


template<class Type>
tmp<scalarField>//Field<Type> >
volumeIntegrateGrad
(
    const Field<Type>& vf,
    const dgMesh& mesh
)
{
//    const polyMesh& mesh = vf.mesh()();
    // Constructor - should be considered!
    const dgBase& polynomials = mesh.polynomials();

    // Local coordinates (reference element)
    scalarField gaussCoords = polynomials.gaussPoints();
    scalarField gaussWeights = polynomials.gaussWeights();

    tmp<scalarField> tCellIntegral(new scalarField(vf.size(), 0.0));
    scalarField& cellIntegral = tCellIntegral();

    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(gaussCoords, ptI)
    {
        // Calculate modal values in given coordinate (Gaussian point
        // coordinate)
        scalarField polyEval =
            polynomials.gradEvaluate(vector(gaussCoords[ptI],0,0));

        forAll (cellIntegral, cellI)
        {
            // Test part
            forAll (polyEval, modI)
            {
                cellIntegral[cellI] +=
                    polyEval[modI]*vf[cellI][modI]*gaussWeights[ptI];
            }

            // Trial part
            cellIntegral[cellI] *= gSum(polyEval);
        }
    }


    return tCellIntegral;
}


template<class Type>
dimensioned<Type>
domainIntegrate
(
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
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
    const tmp<DgGeometricField<Type, dgPatchField, cellMesh> >& tvf
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
