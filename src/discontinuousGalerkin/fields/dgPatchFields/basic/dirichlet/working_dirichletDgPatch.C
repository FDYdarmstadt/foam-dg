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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(p, iF)
{
}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dgPatch& p,
    const DimensionedField<Type, cellMesh>& iF,
    const dictionary& dict
)
:
    dgPatchField<Type>(p, iF)
{
    if (dict.found("value"))
    {
        dgPatchField<Type>::operator==
        (
            Field<Type>("value", dict, p.size())
        );
    }
//    else if (!valueRequired)
//    {
//        dgPatchField<Type>::operator=(pTraits<Type>::zero);
//    }
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

}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dirichletDgPatchField& zgpf
)
:
    dgPatchField<Type>(zgpf)
{
}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dirichletDgPatchField& zgpf,
    const DimensionedField<Type, cellMesh>& iF
)
:
    dgPatchField<Type>(zgpf, iF)
{
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

//    Info << "VALUE INTERNALE COEFFS: " << *this << endl;
//    tmp<dgScalarField > diagCoeffs
//    (
//        new Field<Type>(this->size(), pTraits<Type>::zero)
//    );


    const dgPolynomials& polynomials = this->polynomials();

    typedef FieldField<Field, scalar> scalarFieldField;

    const dgMesh& mesh = this->patch().boundaryMesh().mesh();
    scalarFieldField diag(mesh.size());

    forAll (diag, cellI)
    {
        diag.set(cellI, new scalarField(sqr(scalar(polynomials.size())), 0.0));
    }


    scalar eta = 1.0;//6.0;

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
//    dgScalarField& diagCoeffs = tdiagCoeffs();

    forAll(faceCells, faceI)
    {
        const label& faceCell = faceCells[faceI];
    label globalFace = this->patch().patch().start() + faceI;

        scalar Sf = mag(faceSf[globalFace]);

        // vf[faceCells[faceI]] is global addressing

        // Better name here would be edgePt
        label gaussPt = 0;
        scalar norm = 1;

        // Owner local coord = 1 on face
        if
        (
            mesh().cellCentres()[faceCells[faceI]].component(vector::X)
          < faceCf[globalFace].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
            norm = -1;
        }

Info<< "CELL : "<< mesh().cellCentres()[faceCells[faceI]].component(vector::X)
    << ", FACE: " << faceCf[globalFace]
    << ", faceCells: " << globalFace
    << ", achieved gaussPt: " << gaussPt << ", and norm: " << norm << endl;


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

//            Info<< "PENALY TERM: " << diag[faceCell][addr]
//                << ", Sf: " << Sf <<  endl;
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
//    Info << "VALUE BOU COEFFS: " << *this << endl;

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

    scalar eta = 1.0;//6.0;

    forAll(faceCells, faceI)
    {
        const label& faceCell = faceCells[faceI];
    label globalFace = this->patch().patch().start() + faceI;

        scalar Sf = mag(faceSf[globalFace]);

        // Better name here would be edgePt
        label gaussPt = 0;
        scalar norm = -1;

        // Owner local coord = 1 on face
        if
        (
            mesh().cellCentres()[faceCells[faceI]].component(vector::X)
          < faceCf[globalFace].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
            norm = -1;
        }

Info<< "CELL : "<< mesh().cellCentres()[faceCells[faceI]].component(vector::X)
    << ", FACE: " << faceCf[globalFace]
    << ", faceCells: " << globalFace
    << ", achieved gaussPt: " << gaussPt << ", and norm: " << norm << endl;



        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];


// This is hacked - should be Type templated for vectors and other

        // Penalty term
        forAll (polyEval, modJ)
        {
            forAll (polyEval, modI)
            {
                sourceCoeffs[faceCell][modJ] +=
                   eta*Sf*polyEval[modI]*polyEval[modJ]*mag(dsf[faceI][modI])
                 - Sf*polyEval[modI]*polyGEval[modJ]*mag(dsf[faceI][modI]);
            }
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
