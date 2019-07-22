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

#include "dgCFD.H"
#include "fvCFD.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"

#   include "createTime.H"
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

    label nPoints = 5;

    const vectorField faces = fvMesh.points();
    scalar max = 0;
    scalar min = 0;

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
    const scalar pts = 5.0*cells + 1;
    const scalar increment = (max - min)/(pts - 1);

    scalarField ptsCoords(pts, 0);

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
            OS << "x coord, y coord, z coord, scalar" << endl;
        }
    }


//    forAll (fvMesh.cells(), cellI)
//    {
//        boundBox bb(fvMesh.cells()[cellI].points());
//
//        scalar up = bb.max();
//        scalar dn = bb.min();
//
//        scalar increment = (up - dn)/nPoints;
//        scalarField pts (nPoints, 0);
//
//        forAll (pts, ptI)
//        {
//            pts[ptI] = dn + increment*ptI;
//



    forAll (ptsCoords, ptI)
    {
        vector coord (ptsCoords[ptI], 0, 0);

        label cellID = fvMesh.findCell(coord);
        const labelList& cellPts = fvMesh.cellPoints(cellID);


//        boundBox bb(fvMesh.cellPoints(cellID));
//
        scalar dn = 0;//bb.min()[0];

        const pointField& points = fvMesh.points();

        // Find the point most left
        forAll (cellPts, cptI)
        {
            // X coord of current point
            scalar curPt = points[cellPts[cptI]][0];

            if (curPt < dn)
            {
                dn = curPt;
            }
        }


        scalar localCoord = ptsCoords[ptI] - dn;
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
                OS << ptsCoords[ptI] << ", 0, 0, " << value
                   << endl;
            }
        }
    }





//    forAll (fvMesh.cells(), cellI)
//    {
//        boundBox bb(fvMesh.cells()[cellI].points());
//
//        scalar up = bb.max();
//        scalar dn = bb.min();
//
//        scalar increment = (up - dn)/nPoints;
//        scalarField pts (nPoints, 0);
//
//        forAll (pts, ptI)
//        {
//            pts[ptI] = dn + increment*ptI;
//
//            scalar value = 0;
//
//            forAll (polyEval, modI)
//            {
//                vector coord (ptsCoords[ptI], 0, 0);
//
//                scalarField polyEval = polynomials.evaluate(coord);
//
//                value += T[cellI][modI]*polyEval[modI];
//            }
//
//            if (Pstream::master())
//            {
//                if(OS.opened())
//                {
//                    OS << ptsCoords[ptI] << ",0,0, " << tab << value
//                       << endl;
//                }
//            }
//        }
//
//    }



//        forAll (polyEval, modI)
//        {
//            Tvol[cellI] +=
//            T[cellI][modI]*polyEval[modI];//*dgMesh.cellScaleCoeffs()[cellI];
//
////            Info<< "Cell: " << cellI << ", modI: " << modI << ", polyEval: "
////                << polyEval[modI] << ", T:" << T[cellI][modI]
////                << ", calculated: " << Tvol[cellI] << endl;
//        }
//    }
//
//    // SAMO U OVAJ DG-based solver ubacim da mogu raditi volScalarField i
//    // evaluiram u cell centreima
//
//
//    Info<< "Tvol: " << Tvol.internalField() << nl << endl;

    return 0;
}


// ************************************************************************* //
