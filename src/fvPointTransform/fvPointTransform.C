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

Application
    dgaplacianFoam

Description
    Solves a Laplace equation using the Discontinuous Galerkin Method

Author
    Hrvoje Jasak.  All rights reserved.

\*---------------------------------------------------------------------------*/
#include "BoSSScpp.h"

#include "dgCFD.H"
#include "fvCFD.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"

    Smart_Init();
    BoSSS::Application::ExternalBinding::Initializer MyInit;
    MyInit.BoSSSInitialize();

#   include "createTime.H"

    instantList times = runTime.times();

// Hack:
// Added all-times loop (proper way would be - not here)
forAll (times, timeI)
{
    // Break for going out of bounds
    if ((timeI + 1) == times.size())
    {
        return 0;
    }

    // First entry is '0 constant'
    dimensionedScalar curTime("curTime", dimTime, times[timeI + 1].value());

    runTime.setTime(curTime, timeI);

#   include "createPolyMesh.H"
#   include "createDgMesh.H"
#   include "createFvMesh.H"

#   include "createFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    const dgBase& polynomials = dgMesh.polynomials();

// - Find bounding box
// - Insert 5 points per cell (only in one direction for now as it is 1D)
// - Determine in which cell is which point
// - Interpolate in points and write out to file (regular ascii dat)

    label nPoints = 50;

    const vectorField faces = fvMesh.points();
    scalar max = 0;
    scalar min = 0;


    // Determine min,max for mesh span
    forAll (faces, faceI)
    {
        if (faces[faceI][0] > max)
        {
            max = faces[faceI][0];
        }

        if (faces[faceI][0] < min)
        {
            min = faces[faceI][0];
        }
    }

    Info << "Max: " << max << " and Min: " << min << endl;

    // Number of points = 5*number of cells
    const scalar cells = fvMesh.C().size();
    const scalar pts = nPoints*cells + 1;
    // For x points there are x-1 increments
    const scalar increment = (max - min)/(pts - 1);

    scalarField ptsCoords(pts, 0);

    // Linear coords (global) of the points
    forAll (ptsCoords, ptI)
    {
        ptsCoords[ptI] = min + increment*ptI;
    }


    fileName name = (runTime.timeName())/("ptsInt.csv");
    OFstream OS(name);
    if (Pstream::master())
    {
        if(OS.opened())
        {
            OS << "#x coord, y coord, z coord, scalar" << endl;
        }
    }


    const scalarField& scaleCells = dgMesh.cellScaleCoeffs();


    // For every point determine the cell it is in and local coord
    forAll (ptsCoords, ptI)
    {
        vector coord (ptsCoords[ptI], 0, 0);

        label cellID = fvMesh.findCell(coord);
        const labelList& cellPts = fvMesh.cellPoints(cellID);

        // Lower bound
        scalar lbound = max;
        // Upper bound
        scalar ubound = min;

        const pointField& points = fvMesh.points();

        // Find the point most left and most right (to get cell span)
        forAll (cellPts, cptI)
        {
            // X coord of current point
            scalar curPt = points[cellPts[cptI]][0];

            if (curPt < lbound)
            {
                lbound = curPt;
            }

            if (curPt > ubound)
            {
                ubound = curPt;
            }
        }

        scalar cellSpan = ubound - lbound;

        // Local coord is globalCoord - minimum local Coord
        scalar localCoord =
            (ptsCoords[ptI] - lbound - 0.5*cellSpan)/scaleCells[cellID];


        vector localVec (localCoord, 0, 0);

        scalarField polyEval = polynomials.evaluate(localVec);

        scalar value = 0;

        forAll (polyEval, modI)
        {
            value += T[cellID][modI]*polyEval[modI];
        }


        if (Pstream::master())
        {
            if(OS.opened())
            {
                OS << ptsCoords[ptI]// << ", 0, 0, "
                   << " "
                   << value
                   << endl;
            }
        }
    }

}
    return 0;
}


// ************************************************************************* //
