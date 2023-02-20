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

Description

\*---------------------------------------------------------------------------*/

#include "dgMesh.H"
#include "foamTime.H"
#include "polyMesh.H"
#include "primitiveMesh.H"
#include "demandDrivenData.H"
#include "dgMeshLduAddressing.H"
//#include "dgPolynomials.H"

#include "BoSSScpp.h"

using namespace BoSSS::Application::ExternalBinding;
using namespace BoSSS::Foundation::Grid;
using namespace BoSSS::Foundation::Grid::Classic;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(dgMesh, 1);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

const Foam::lduAddressing& Foam::dgMesh::lduAddr() const
{
    if (!lduPtr_)
    {
        lduPtr_ = new dgMeshLduAddressing(*this);
    }

    return *lduPtr_;
}


void Foam::dgMesh::clearAddressing()
{
    deleteDemandDrivenData(lduPtr_);
    deleteDemandDrivenData(cellScaleCoeffsPtr_);

    // Geometry dependent object updated through call-back
    // and handled by polyMesh
    // HJ, 29/Aug/2010
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dgMesh::dgMesh(const polyMesh& pMesh)
:
    GeoMesh<polyMesh>(pMesh),
    MeshObject<polyMesh, dgMesh>(pMesh),
    boundary_(*this, pMesh.boundaryMesh()),
    schemesDict_(mesh().thisDb()),
    solutionDict_(mesh().thisDb()),
    cellScaleCoeffsPtr_(NULL),
    dgBasePtr_
    (
        dgBase::New(pMesh)
    ),
    lduPtr_(NULL)
{
    if (debug)
    {
        InfoIn("dgMesh::dgMesh(const polyMesh& pMesh)")
            << "Creating dgMesh from polyMesh" << endl;
    }

    int nPoints = pMesh.nPoints();

    const Foam::polyBoundaryMesh& bm = pMesh.boundaryMesh();
    // const UList<wordRe> patchNames = 

    Foam::pointField points = pMesh.points();
    double* pointsArray = (double*) malloc(sizeof(double)*nPoints*3);
    for(label i = 0; i < nPoints; i++) {
        pointsArray[i*3 + 0] = points[i].x();
        pointsArray[i*3 + 1] = points[i].y();
        pointsArray[i*3 + 2] = points[i].z();
    }
    
    label nCells = pMesh.nCells();
    label nFaces = pMesh.nFaces();
    label nInternalFaces = pMesh.nInternalFaces();

    faceList faces = pMesh.faces();
    int emptyTag = -1;

    int i = 0;
    int found = 0;
    for (auto patch : bm.types()){
        if (patch == "empty"){
            if (found == 1){
                throw "more than one empty patch found; please subsume all empty patches into one in blockMeshDict.";
            }
            emptyTag = i;
            found = 1;
        }
        i++;
    }

    int* vertices_per_face_Array = (int*) malloc(sizeof(int)*nFaces);
    int* faceOwner_Array = (int*) malloc(sizeof(int)*nFaces);
    int* faceNeighbor_Array = (int*) malloc(sizeof(int)*nFaces);

    int TotalListLength = 0;
    for(int iF = 0; iF < nFaces; iF++) {
        //int vertices_per_face = 
        face f = faces[iF];
        int vertices_per_face = f.size();
        vertices_per_face_Array[iF] = vertices_per_face;
        TotalListLength += vertices_per_face;
        //Info << "Face " << iF << " Vtx per face: " << vertices_per_face << " ";

        faceOwner_Array[iF] = pMesh.faceOwner()[iF];
        if(iF < nInternalFaces) {
            //Info << "(" << pMesh.faceOwner()[iF] << "--" << pMesh.faceNeighbour()[iF] << ") ";
            faceNeighbor_Array[iF] = pMesh.faceNeighbour()[iF];
        } else {
            //Info << "(" << pMesh.faceOwner()[iF] << ") ";
        }

    }

    int** facesArray = (int**) malloc(sizeof(int*)*nFaces);
    int* facesContent = (int*) malloc(sizeof(int)*TotalListLength);
    int* pfacesContent = facesContent;
    for(int iF = 0; iF < nFaces; iF++) {
        //int vertices_per_face = 
        face f = faces[iF];
        int vertices_per_face = f.size();

        facesArray[iF] = pfacesContent;
        for(int iVtx = 0; iVtx < vertices_per_face; iVtx++) {
            *pfacesContent = f[iVtx];
            pfacesContent++;
        }
    }
    int noOfBmNames = bm.names().size();
    int* nameLengths = (int*) malloc(sizeof(int) * noOfBmNames);
    int** names = (int**) malloc(sizeof(int*) * noOfBmNames);
    for (int i = 0; i < noOfBmNames; i++){
        int len = bm.names()[i].size();
        nameLengths[i] = len;
        names[i] = (int*) malloc(sizeof(int) * len);
        for (int j = 0; j < len; j++){
            names[i][j] = (int)bm.names()[i][j];
        }
    }
    int* patches = (int*) malloc(sizeof(int) * (nFaces - nInternalFaces));
    for (int i = 0; i < (nFaces - nInternalFaces); i++){
        patches[i] = bm.patchID()[i];
    }

    this->bosssmesh_ = new BoSSS::Foundation::Grid::OpenFOAMGrid(nPoints, nCells, nFaces, nInternalFaces, noOfBmNames, nameLengths, emptyTag, facesArray, vertices_per_face_Array, faceNeighbor_Array, faceOwner_Array, pointsArray, names, patches);
    free(pointsArray);
    free(vertices_per_face_Array);
    free(faceNeighbor_Array);
    free(faceOwner_Array);
    free(facesArray);
    free(facesContent);

    fvMesh *finVolMesh__ = new Foam::fvMesh(
        IOobject
        (
        "fvMesh",
        // runTime.timeName(),
        "0",
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
        ),
        // meshIOobj,
        Xfer<pointField>(pMesh.points()),
        // mesh().allPoints(),
        // mesh().pointPoints(),
        Xfer<faceList>(pMesh.faces()),
        // mesh().allFaces(),
        Xfer<cellList>(pMesh.cells()), true);
    this->finVolMesh_ = finVolMesh__;
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::dgMesh::~dgMesh()
{
    if(bosssmesh_ != NULL)
        delete bosssmesh_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::Time& Foam::dgMesh::time() const
{
    return mesh().time();
}


const Foam::objectRegistry& Foam::dgMesh::thisDb() const
{
    return mesh().thisDb();
}


const Foam::dgBoundaryMesh& Foam::dgMesh::boundary() const
{
    return boundary_;
}


bool Foam::dgMesh::movePoints() const
{
    return false;
}


bool Foam::dgMesh::updateMesh(const mapPolyMesh&) const
{
    return false;
}


// Not sure whether I need this part of code

//evaluatePolynomial(dgScalarField& dgsf)
//{
//
//    forAll(integrationPoints, ptI)
//    {
//        // Obtain polynomial values for given coordinate
//        scalarList polyEval = polynomial_.evaluate(localCoords);
//
//        forAll(mesh_, cellI)
//        {
//            forAll(polyEval, modI)
//            {
//                // Scale polynomial values for each cell based on DG coeffs
//                value += dgsf[cellI][modI]*polyEval[modI];
//            }
//        }
//    }
//}


// Only access function (for now)
Foam::scalarList Foam::dgMesh::gaussWeights()
{
//    const scalarList& weights = polynomials_.gaussWeights();
//
//    scalarListList weightsMesh(this->size(), weights);
//    dgPolynomials polynomials();

//    return polynomials.gaussWeights();

    scalarList left(1,1.0);
    return left;
}

// Only access function (for now)
Foam::scalarList Foam::dgMesh::gaussPoints()
{
//    dgPolynomials polynomials();

    scalarList one(2,2.0);
    return one;
//    return polynomials.gaussPoints();
}


void Foam::dgMesh::calcCellScaleCoeffs() const
{
    const polyMesh& mesh = this->mesh();
    const scalarField points = mesh.points().component(vector::X);

    scalarField cellMax(mesh.cellCentres().component(vector::X));
    scalarField cellMin(cellMax);

    forAll (cellMax, cellI)
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

    // Normalize and scale based on real cell size (not reference one)
    cellScaleCoeffsPtr_ = new scalarField((cellMax - cellMin)/2);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

bool Foam::dgMesh::operator!=(const dgMesh& m) const
{
    return &m != this;
}


bool Foam::dgMesh::operator==(const dgMesh& m) const
{
    return &m == this;
}


// ************************************************************************* //
