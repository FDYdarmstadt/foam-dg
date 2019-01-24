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

#include "dirichletDgPatchField.H"
#include "dgPolynomials.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void dirichletDgPatchField<Type>::calcEta()
{
    if (!etaPtr_)
    {
        const polyMesh& mesh = this->patch().boundaryMesh().mesh().mesh();
        dgPolynomials polynomials;

        scalar sqrP = sqr(scalar(polynomials.order()));

        const pointField& points = mesh.points();
        const labelList& faceCells = this->patch().faceCells();

        // Eta should be determined based on min or max cell dimension
        scalarField cellSize(faceCells.size(), 0);

        // Cell size is determined by creating per-cell bounding box and taking
        // the maximum dimension (span) of the bounding box
        forAll (cellSize, cellI)
        {
            labelList pointLabels = mesh.cellPoints(faceCells[cellI]);
            pointField cellPts(pointLabels.size());

            forAll (pointLabels, ptI)
            {
                cellPts[ptI] = points[pointLabels[ptI]];
            }

            // Create cell bounding box
            boundBox cellPtBB(cellPts);

            // Determine cellSize based on max span of the cell bounding box
            cellSize[cellI] = cellPtBB.minDim();
        }

        Info<< "Cell sizes: " << cellSize << endl;
        Info<< "P: " << sqrP << endl;
        Info<< "eta: " << sqrP/cellSize << endl;

        etaPtr_ = new scalarField(sqrP/cellSize);
//        etaPtr_ = new scalarField(cellSize);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(p, iF),
    etaPtr_(NULL)
{
    calcEta();
}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary& dict
)
:
    dgPatchField<Type>(p, iF),
    etaPtr_(NULL)

{
    if (dict.found("value"))
    {
        dgPatchField<Type>::operator==
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        FatalIOErrorIn
        (
            "dgPatchField<Type>::dgPatchField"
            "("
            "const dgPatch& p,"
            "const DimensionedField<Type, cellMesh>& iF"
            "const dictionary& dict"
            ")",
            dict
        ) << "Essential entry 'value' missing"
          << exit(FatalIOError);
    }

    calcEta();
}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dirichletDgPatchField& zgpf
)
:
    dgPatchField<Type>(zgpf),
    etaPtr_(NULL)

{
    calcEta();
}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dirichletDgPatchField& zgpf,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(zgpf, iF),
    etaPtr_(NULL)

{
    calcEta();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
//tmp<dgScalarField> dirichletDgPatchField<Type>::valueInternalCoeffs
FieldField<Field, scalar> dirichletDgPatchField<Type>::valueInternalCoeffs
(
//    const tmp<dgScalarField>&
) const
{
// Diagonal

    const dgPolynomials& polynomials = this->polynomials();

    typedef FieldField<Field, scalar> scalarFieldField;

    const dgMesh& mesh = this->patch().boundaryMesh().mesh();
    scalarFieldField diag(mesh.size());

    forAll (diag, cellI)
    {
        diag.set(cellI, new scalarField(sqr(scalar(polynomials.size())), 0.0));
    }

    scalarField& etaCells = *etaPtr_;

    const PtrList<scalarField>& gaussEdgeEval = polynomials.gaussPtsEval();
    const PtrList<scalarField>& gaussGradEdgeEval =
        polynomials.gaussPtsGradEval();
    // I evaluate polyEval in gauss points and (-1) and (1) so that Ptr
    // size is length+2

    const vectorField& faceSf = this->patch().faceAreas();
    const vectorField& faceCf = this->patch().faceCentres();
    const labelList& faceCells = this->patch().faceCells();

    tmp<dgScalarField> tdiagCoeffs
        (
            new dgScalarField(mesh.size(), dgScalar(0.0))
        );

    forAll(faceCells, faceI)
    {
        const label& faceCell = faceCells[faceI];
        label globalFace = this->patch().patch().start() + faceI;


        scalar eta = etaCells[faceI];
        scalar Sf = mag(faceSf[globalFace]);

        // Better name here would be edgePt
        label gaussPt = 0;

        // Owner local coord = 1 on face
        if
        (
            mesh().cellCentres()[faceCells[faceI]].component(vector::X)
          < faceCf[globalFace].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
        }

        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];

        // Penalty term
        forAll (polyEval, modJ)
        {
            forAll (polyEval, modI)
            {
                label addr = modJ*polyEval.size() + modI;

                diag[faceCell][addr] = eta*Sf*polyEval[modI]*polyEval[modJ]
                                     - Sf*polyGEval[modI]*polyEval[modJ]
                                     - Sf*polyEval[modI]*polyGEval[modJ];
            }
        }
    }

//    return tdiagCoeffs;
    return diag;
}

template<class Type>
tmp<dgScalarField> dirichletDgPatchField<Type>::valueBoundaryCoeffs
(
//    const tmp<dgScalarField>& dsf
) const
{

    const vectorField& faceSf = this->patch().faceAreas();
    const vectorField& faceCf = this->patch().faceCentres();
    const labelList& faceCells = this->patch().faceCells();

    const dgMesh& mesh = this->patch().boundaryMesh().mesh();

    const dgPolynomials& polynomials = this->polynomials();

    const PtrList<scalarField>& gaussEdgeEval = polynomials.gaussPtsEval();
    const PtrList<scalarField>& gaussGradEdgeEval =
        polynomials.gaussPtsGradEval();
    // I evaluate polyEval in gauss points and (-1) and (1) so that Ptr
    // size is length+2


    Field<Type> dsf = *this;

    tmp<dgScalarField> tsourceCoeffs
        (
            new dgScalarField(mesh.size(), dgScalar(0.0))
        );
    dgScalarField& sourceCoeffs = tsourceCoeffs();

    scalarField& etaCells = *etaPtr_;

Info<< nl << "ETA CELLS: " << etaCells << nl << endl;

    forAll(faceCells, faceI)
    {
        const label& faceCell = faceCells[faceI];
        label globalFace = this->patch().patch().start() + faceI;
Info << " FACE CELL INDEX : " << faceCell << endl;
        scalar eta = etaCells[faceI];
//        scalar eta = etaCells[faceCell];
        scalar Sf = mag(faceSf[globalFace]);

        // Better name here would be edgePt
        label gaussPt = 0;

        // Owner local coord = 1 on face
        if
        (
            mesh().cellCentres()[faceCells[faceI]].component(vector::X)
          < faceCf[globalFace].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
        }

        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];


        Info<< " EVALUATION cell: " << faceCell << ", eval: " << polyEval
            << ", and grad eval: " << polyGEval << ", ETA: " << eta << endl;
// Should be Type templated for vectors and other

        // Penalty term
        forAll (polyEval, modJ)
        {
            forAll (polyEval, modI)
            {
                sourceCoeffs[faceCell][modJ] +=
                   eta*Sf*polyEval[modI]*polyEval[modJ]*mag(dsf[faceI][modI])
                 - Sf*polyEval[modI]*polyGEval[modJ]*mag(dsf[faceI][modI]);
//                   eta*Sf*polyEval[modI]*polyEval[modJ]*mag(dsf[faceI][modI])
//                 - Sf*polyEval[modI]*polyGEval[modJ]*mag(dsf[faceI][modI]);

                Info<< "ENTERING MODS" << modI << ", " << modJ << endl;
            }
        Info<< "SOURCE cell: " << faceCell << ", is: " <<
        sourceCoeffs[faceCell][modJ] << endl;
        }
    }

    return tsourceCoeffs;
}


template<class Type>
void dirichletDgPatchField<Type>::write(Ostream& os) const
{
    dgPatchField<Type>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
