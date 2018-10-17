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
    Info << "DIRICHLET 1" << endl;
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

    Info << "DIRICHLET 2 " << this[0][0] << endl;
}


template<class Type>
dirichletDgPatchField<Type>::dirichletDgPatchField
(
    const dirichletDgPatchField& zgpf
)
:
    dgPatchField<Type>(zgpf)
{
    Info << "DIRICHLET 3" << endl;
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
    Info << "DIRICHLET 4" << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<dgScalarField> dirichletDgPatchField<Type>::valueInternalCoeffs
(
//    const tmp<dgScalarField>&
) const
{
// Diagonal

    Info << "VALUE INTERNALE COEFFS: " << *this << endl;
//    tmp<dgScalarField > diagCoeffs
//    (
//        new Field<Type>(this->size(), pTraits<Type>::zero)
//    );

    // How to access prescribed value?
//    dgScalarField refValue(*this);

//    const dgMesh& mesh = this->mesh();

//    dgPolynomials polynomials;
    const dgPolynomials& polynomials = this->polynomials();


//    Info << polynomials1.gaussPtsEval() << endl;
//    Info << this->dimensionedInternalField().mesh().polynomials().gaussPtsEval() << endl;


    const dgMesh& mesh = this->patch().boundaryMesh().mesh();

//    scalar eta = 1.0;//6.0;

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
    dgScalarField& diagCoeffs = tdiagCoeffs();

    forAll(faceCells, faceI)
    {
        const label& faceCell = faceCells[faceI];

        // vf[faceCells[faceI]] is global addressing

        label gaussPt = 0;
        label  gaussPtNei = gaussEdgeEval.size() - 1;

        // Owner local coord = 1 on face
        if
        (
            mesh().cellCentres()[faceCells[faceI]].component(vector::X)
          < faceCf[faceI].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
            gaussPtNei = 0;
        }

        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];
        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];
//        const scalarField& polyGEvalNei = gaussGradEdgeEval[gaussPtNei];

        scalar Sf = mag(faceSf[faceI]);

        forAll (diagCoeffs[0], modI)
        {
//            diagCoeffs[faceCell][modI] = polyGEval[modI]*gSum(polyEvalNei)*Sf*0.5
//                        - polyGEval[modI]*gSum(polyEval)*Sf*0.5;
        }
    }

    return tdiagCoeffs;
}

template<class Type>
tmp<dgScalarField> dirichletDgPatchField<Type>::valueBoundaryCoeffs
(
//    const tmp<dgScalarField>& dsf
) const
{
    Info << "VALUE BOU COEFFS: " << *this << endl;

// Source
Info << "*" ;
//    dgPolynomials polynomials;
    const dgPolynomials& polynomials = this->polynomials();
Info << "*" ;

    const dgMesh& mesh = this->patch().boundaryMesh().mesh();
Info << "*" ;

//    scalar eta = 1.0;//6.0;
//
Info << "*" ;
    const PtrList<scalarField>& gaussEdgeEval = polynomials.gaussPtsEval();
Info << "*" ;
    const PtrList<scalarField>& gaussGradEdgeEval =
        polynomials.gaussPtsGradEval();
//    // I evaluate polyEval in gauss points and (-1) and (1) so that Ptr
//    // size is length+2

Info << "*" ;

    const vectorField& faceSf = this->patch().faceAreas();
    const vectorField& faceCf = this->patch().faceCentres();
    const labelList& faceCells = this->patch().faceCells();
Info << "*" ;

    tmp<dgScalarField> tsourceCoeffs
        (
            new dgScalarField(mesh.size(), dgScalar(0.0))
        );
    dgScalarField& sourceCoeffs = tsourceCoeffs();
Info << "*" ;

    forAll(faceCells, faceI)
    {
        const label& faceCell = faceCells[faceI];

        // vf[faceCells[faceI]] is global addressing

        label gaussPt = 0;
        label  gaussPtNei = gaussEdgeEval.size() - 1;

Info << "+" ;
        // Owner local coord = 1 on face
        if
        (
            mesh().cellCentres()[faceCells[faceI]].component(vector::X)
          < faceCf[faceI].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
            gaussPtNei = 0;
        }
Info << "-" ;

        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];
        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];
        const scalarField& polyGEvalNei = gaussGradEdgeEval[gaussPtNei];

        scalar Sf = mag(faceSf[faceI]);
Info << "0" ;

        forAll (sourceCoeffs[0], modI)
        {
            sourceCoeffs[faceCell][modI] =
                mag(this[faceI][modI])*polyGEvalNei[modI]*gSum(polyEvalNei)*Sf*0.5
              - mag(this[faceI][modI])*polyGEvalNei[modI]*gSum(polyEval)*Sf*0.5;
        }
    }
Info << "*" << endl;

    return tsourceCoeffs;
//    return *this;
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
