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

#include "localDivScheme.H"
//#include "surfaceInterpolate.H"
//#include "dgcDiv.H"
#include "dgcVolumeIntegrate.H"
#include "dgMatrices.H"
#include "ExpandTensorNField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

//template<class Type, class GType>
//void localDivScheme<Type, GType>::calcEta()
//{
//    if (!etaPtr_)
//    {
//        const polyMesh& mesh = this->mesh().mesh();;
//        const dgBase& polynomials = this->mesh().polynomials();
////        dgBase polynomials(mesh);
//
//        scalar sqrP = sqr(scalar(polynomials.order()));
//
//        const pointField& points = mesh.points();
//
//        // Eta should be determined based on min or max cell dimension
//        scalarField cellSize(mesh.cellCentres().size(), 0);
//
//        // Cell size is determined by creating per-cell bounding box and taking
//        // the maximum dimension (span) of the bounding box
//        forAll (cellSize, cellI)
//        {
//            labelList pointLabels = mesh.cellPoints(cellI);
//            pointField cellPts(pointLabels.size());
//
//            forAll (pointLabels, ptI)
//            {
//                cellPts[ptI] = points[pointLabels[ptI]];
//            }
//
////            pointField& cellPt = cellPoints(cellI);
//
//            // Create cell bounding box
//            boundBox cellPtBB(cellPts);
//
//            // Determine cellSize based on max span of the cell bounding box
//            cellSize[cellI] = cellPtBB.minDim();
//        }
//
////        Info<< "Cell sizes: " << cellSize << endl;
////        Info<< "P: " << sqrP << endl;
////        Info<< "eta: " << sqrP/cellSize << endl;
//
//        etaPtr_ = new scalarField(sqrP/cellSize);
////        etaPtr_ = new scalarField(cellSize);
//    }
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<GeometricField<Type, dgPatchField, cellMesh> >
localDivScheme<Type, GType>::dgcDiv
(
    const dimensionedScalar& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    // Laplace consists of four terms:
    // Volume term, consistency term, symmetry term and penalty term

    // Volume term is (gradU * gradV):
    tmp<GeometricField<Type, dgPatchField, cellMesh> > tLaplacianVol
    (
        // The term is squared in volumeIntegrateGrad - KEEP IN MIND
        dgc::volumeIntegrateGrad(vf)
    );

    tLaplacianVol().rename("dgDiv(" + vf.name() + ')');

    return tLaplacianVol;
}
//
//
template<class Type, class GType>
tmp<dgMatrix<Type> >
localDivScheme<Type, GType>::dgmDiv
(
    const GeometricField<dgVector, dgPatchField, cellMesh>& vvf,
    const GeometricField<Type, dgPatchField, cellMesh>& vsf
)
{
    const dgMesh& mesh = this->mesh();
    const dgBase& polynomials = mesh.polynomials();

    tmp<dgMatrix<Type> > tdgm
    (
        new dgMatrix<Type>
        (
            vsf,
            vsf.dimensions()
        )
    );
    dgMatrix<Type>& dgm = tdgm();

    // Volume part
    label dim = polynomials.size();

    scalarList A(dim, 0.0);

    // Check if this is weighted or unweighted
    const PtrList<scalarField>& polyEval = polynomials.gaussPtsEval();
    const PtrList<scalarField>& polyGEval = polynomials.wtdGaussGradEval();
    const scalarField& scaleCells = mesh.cellScaleCoeffs();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
        = dgm.diag().asSquare();

    Field<scalarField> gradCoeffs(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsNeg(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsPos(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsCrssPos(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsCrssNeg(ds[0].size(), scalarField(dim, 0.0));
    scalarField coeffs(Type::coeffLength, 0.0);

    label lastPoint = polyEval.size() - 1;

    forAll (coeffs, modJ)
    {
        forAll (coeffs, modI)
        {

            label coeff = modJ*coeffs.size() + modI;

            scalarList vectorSize (3, 0.0);

            forAll (vectorSize, inner)
            {

                // Go over all Gaussian points for the whole mesh and add
                // value*weight
                forAll(coeffs, pti)
                {
                    // Zeroth entry is for coordinate -1
                    label ptI = pti + 1;


                    // Sum over all gauss points
                    gradCoeffs[coeff][inner] += polyEval[ptI][modJ]
                                              *polyEval[ptI][inner]
                                              *polyGEval[ptI][modI];
                }

                // Neg is evaluated at -1
                surfaceCoeffsNeg[coeff][inner] = polyEval[0][modJ]
                                          *polyEval[0][inner]
                                          *polyEval[0][modI];

                // Pos is evaluated at +1
                surfaceCoeffsPos[coeff][inner] = polyEval[lastPoint][modJ]
                                              *polyEval[lastPoint][inner]
                                              *polyEval[lastPoint][modI];


                // Maybe I need cross terms here for off-diag
//                such as: polyEval[0] polyEval[0] polyEval[lastPoint]
                surfaceCoeffsCrssPos[coeff][inner] = polyEval[lastPoint][modJ]
                                              *polyEval[lastPoint][inner]
                                              *polyEval[0][modI];
                surfaceCoeffsCrssNeg[coeff][inner] = polyEval[0][modJ]
                                              *polyEval[0][inner]
                                              *polyEval[lastPoint][modI];
            }
        }
    }

    forAll(vsf, cellI)
    {
        forAll(A, modJ)
        {
            forAll(A, modI)
            {
                label coeff = modJ*coeffs.size() + modI;
                scalarList vectorSize (3, 0.0);

                forAll(vectorSize, inner)
                {
                    ds[cellI](modJ, modI) += -
                        vvf[cellI][0][inner]*gradCoeffs[coeff][inner]
                        /scaleCells[cellI];
                }
            }
        }
//Info << nl<< "DS CELL " << cellI << ", " << ds[cellI] << nl<< endl;
    }

//    Info<< "MATRIX: " << ds << nl << endl;




    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ls
        = dgm.lower().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& us
        = dgm.upper().asSquare();

{
    // Go over all faces, if nei exists it is an internal face - do:
    // In d(cellID, cellID) insert owner contrib, in M(cellID, neiID) insert nei
    // contrib.
    // Do this both for owner and neighbour (each has both diag and off diag
    // contrib)

    // Evaluate grad in ref cell boundaries (x=-1,1)
    const PtrList<scalarField>& gaussEdgeEval = polynomials.gaussPtsEval();
    const PtrList<scalarField>& gaussGradEdgeEval =
        polynomials.gaussPtsGradEval();
    // I evaluate polyEval in gauss points and (-1) and (1) so that Ptr
    // size is length+2

    const vectorField& faceSf = mesh().faceAreas();

    const labelList& neiList = mesh().faceNeighbour();

    // Go over all internal faces (only those that have nei)
    forAll (neiList, faceI)
    {

        // Nei is -1 for boundary faces
        const label& own = mesh().faceOwner()[faceI];
        const label& nei = mesh().faceNeighbour()[faceI];


        // Matrix size
        label mtxSize = Type::coeffLength;
        mtxSize *= mtxSize;

        scalarField diagCoeffOwn(mtxSize, 0.0);
        scalarField diagCoeffNei(mtxSize, 0.0);
        scalarField diagCoeffOwnNei(mtxSize, 0.0);
        scalarField diagCoeffNeiOwn(mtxSize, 0.0);

        scalar Sf = mag(faceSf[faceI]);

        label gaussPt = 0;
        label  gaussPtNei = gaussEdgeEval.size() - 1;


        Field<scalarField>& surfaceCoeffsOwn = surfaceCoeffsPos;
        Field<scalarField>& surfaceCoeffsNei = surfaceCoeffsNeg;
        Field<scalarField>& surfaceCoeffsCrssOwn = surfaceCoeffsCrssPos;
        Field<scalarField>& surfaceCoeffsCrssNei = surfaceCoeffsCrssNeg;

        // Owner local coord = 1 on face1
        if
        (
            mesh().cellCentres()[own].component(vector::X)
          < mesh().cellCentres()[nei].component(vector::X)
        )
        {
            gaussPt = gaussEdgeEval.size() - 1;
            gaussPtNei = 0;
        }
        // Owner local coord = -1 on face
        else
        {
            gaussPt = 0;
            gaussPtNei = gaussEdgeEval.size() - 1;
            surfaceCoeffsOwn = surfaceCoeffsNeg;
            surfaceCoeffsNei = surfaceCoeffsPos;
            surfaceCoeffsCrssOwn = surfaceCoeffsCrssNeg;
            surfaceCoeffsCrssNei = surfaceCoeffsCrssPos;
        }

        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];


        scalarList Aown(mtxSize, 0.0);
        scalarList Anei(mtxSize, 0.0);
        scalarList AownNei(mtxSize, 0.0);
        scalarList AneiOwn(mtxSize, 0.0);

        forAll(A, modJ)
        {
            forAll(A, modI)
            {
                label coeff = modJ*coeffs.size() + modI;
                scalarList vectorSize (3, 0.0);

                forAll(vectorSize, inner)
                {
                    Aown[coeff] +=
                        vvf[own][0][inner]*surfaceCoeffsOwn[coeff][inner];

                    Anei[coeff] +=
                        vvf[nei][0][inner]*surfaceCoeffsCrssNei[coeff][inner];

//                    AownNei[coeff] +=
//                        vvf[own][0][inner]*surfaceCoeffsCrssNeg[coeff][inner];
//
//                    AneiOwn[coeff] +=
//                        vvf[nei][0][inner]*surfaceCoeffsCrssPos[coeff][inner];
                }
            }
        }

//Info << " CELL: " << own << ", vec: " << vvf[own]
//     << " and cell: " <<  nei << ", vec: " << vvf[nei]
//     << ", surface Coeff own: " << surfaceCoeffsOwn
//     << " and surfaceCoeff nei: :" << surfaceCoeffsNei << endl;

        // For owner, calculate diag and off-diag contrib
        forAll (polyEval, coeffJ)
        {
            forAll (polyEval, coeffI)
            {

                label mtxCoeff = coeffJ*polyEval.size() + coeffI;

                diagCoeffOwn[mtxCoeff] =
                    Aown[mtxCoeff]//*polyEval[coeffI]
                    *Sf/2.0;


                diagCoeffNei[mtxCoeff] =
                    Anei[mtxCoeff]//*polyEvalNei[coeffI]
                    *Sf/2.0;

                diagCoeffOwnNei[mtxCoeff] =
                    AownNei[mtxCoeff]//*polyEval[coeffI]
                    *Sf/2.0;


                diagCoeffNeiOwn[mtxCoeff] =
                    AneiOwn[mtxCoeff]//*polyEvalNei[coeffI]
                    *Sf/2.0;


                // For cell owner
                ds[own](coeffJ, coeffI) += diagCoeffOwn[mtxCoeff];
                ds[nei](coeffJ, coeffI) += diagCoeffNei[mtxCoeff];

//              us[faceI](coeffJ, coeffI) += diagCoeffNei[mtxCoeff];
//              us[faceI](coeffJ, coeffI) += diagCoeffNeiOwn[mtxCoeff];
            }
        }
//Info << " DIAG COEFF OWN: " << diagCoeffOwn << nl << " DIAG COEFF NEI: "
//    << diagCoeffNei << nl << endl;
    }
}

    forAll(mesh.mesh().boundaryMesh(), patchI)
    {
        if (!isA<emptyPolyPatch>(mesh.mesh().boundaryMesh()[patchI]))
        {
        const dgPatchField<Type>& psf = vsf.boundaryField()[patchI];
        const dgPatchField<dgVector>& pvf = vvf.boundaryField()[patchI];

        FieldField<Field, scalar> vic = psf.valIntCoeffsDiv(pvf);
        tmp<dgScalarField> vbc = psf.valBouCoeffsDiv(pvf);

//        Info << " SOURCE PRIOR TO IMPLEMENTATION: " << vbc() <<  ", for patch: "
//             << mesh.mesh().boundaryMesh()[patchI].name() << endl;


        forAll(ds, cellI)
        {
            dgm.source()[cellI] -= vbc()[cellI];

            forAll (vbc()[cellI], addrI)
            {
                forAll (vbc()[cellI], addrJ)
                {
                    label addr = addrI*vbc()[cellI].size() + addrJ;

                    ds[cellI](addrI, addrJ) -= vic[cellI][addr];
                }
            }
        }
        }
    }

    Info << "DIV OUT diag " << ds << nl << endl;
    Info << "DIV OUT source " << dgm.source() << endl;


    return tdgm;
}
//
//
//
//
//// FOR BACKUP
//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//localDivScheme<Type, GType>::dgmDiv
//(
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    Info << "MATRIX CONSTRUCTOR: " << endl;
//
//    const dgMesh& mesh = this->mesh();
//
//    tmp<dgMatrix<Type> > tdgm
//    (
//        new dgMatrix<Type>
//        (
//            vf,
//            vf.dimensions()
//        )
//    );
//    dgMatrix<Type>& dgm = tdgm();
//
////    const polyMesh& pMesh = mesh.mesh();
//
//    const dgBase& polynomials = mesh.polynomials();//(pMesh);
//
//    // Calculate penalty
//    calcEta();
//    scalarField& etaCells = *etaPtr_;
//
////    scalarField etaCells = etaCellsa;
////    etaCells = 1.0;
////    scalar eta = 1.0;
//
//    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
//        = dgm.diag().asSquare();
//
//{
//    scalarField gradCoeffs(ds[0].size(), 0.0);
//    scalarField coeffs(Type::coeffLength, 0.0);
//
//    const PtrList<scalarField> polyEval = polynomials.wtdGaussGradEval();
//    const PtrList<scalarField>& unwPolyEval = polynomials.gaussPtsGradEval();
//
//    // Go over all Gaussian points for the whole mesh and add value*weight
//    forAll(coeffs, pti)
//    {
//        // Zeroth entry is for coordinate -1
//        label ptI = pti + 1;
//
//        forAll (coeffs, modI)
//        {
//            label coeff = modI*coeffs.size() + modI;
//            gradCoeffs[coeff] += unwPolyEval[ptI][modI]*polyEval[ptI][modI];
//        }
//    }
//
//    const scalarField& scaleCells = mesh.cellScaleCoeffs();
//
//    forAll (vf, cellI)
//    {
//        forAll (coeffs, modI)
//        {
//            forAll (coeffs, modJ)
//            {
//                label coeff = modI*coeffs.size() + modJ;
//
//                ds[cellI](modI, modJ) =
//                    gradCoeffs[coeff]*scaleCells[cellI];
//            }
//        }
//    }
//}
//
//Info << "DIAGONAL: " << ds[0] << endl;
//
//
//    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ls
//        = dgm.lower().asSquare();
//
//    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& us
//        = dgm.upper().asSquare();
//
//// CONSISTENCY TERM
//{
//    // Go over all faces, if nei exists it is an internal face - do:
//    // In d(cellID, cellID) insert owner contrib, in M(cellID, neiID) insert nei
//    // contrib.
//    // Do this both for owner and neighbour (each has both diag and off diag
//    // contrib)
//
//    // Evaluate grad in ref cell boundaries (x=-1,1)
//    const PtrList<scalarField>& gaussEdgeEval = polynomials.gaussPtsEval();
//    const PtrList<scalarField>& gaussGradEdgeEval =
//        polynomials.gaussPtsGradEval();
//    // I evaluate polyEval in gauss points and (-1) and (1) so that Ptr
//    // size is length+2
//
//    const vectorField& faceSf = mesh().faceAreas();
//
//    const labelList& neiList = mesh().faceNeighbour();
//
//    // Go over all internal faces (only those that have nei)
//    forAll (neiList, faceI)
//    {
//
//        // Nei is -1 for boundary faces
//        const label& own = mesh().faceOwner()[faceI];
//        const label& nei = mesh().faceNeighbour()[faceI];
//
//
//        scalar eta = etaCells[own];
//
//
//        // Greater eta is chosen between owner and neighbour
//        if (etaCells[own] < etaCells[nei])
//        {
//            eta = etaCells[nei];
//        }
//
//
//        // Matrix size
//        scalar mtxSize = Type::coeffLength;
//        mtxSize *= mtxSize;
//
//        scalarField diagCoeffOwn(mtxSize, 0.0);
//        scalarField offCoeffOwn(mtxSize, 0.0);
//
//        scalarField diagCoeffNei(mtxSize, 0.0);
//        scalarField offCoeffNei(mtxSize, 0.0);
//
//        scalar Sf = mag(faceSf[faceI]);
//
//        label gaussPt = 0;
//        label  gaussPtNei = gaussEdgeEval.size() - 1;
//
//        // Owner local coord = 1 on face
//        if
//        (
//            mesh().cellCentres()[own].component(vector::X)
//          < mesh().cellCentres()[nei].component(vector::X)
//        )
//        {
//            gaussPt = gaussEdgeEval.size() - 1;
//            gaussPtNei = 0;
//        }
//        // Owner local coord = -1 on face
//        else
//        {
//            gaussPt = 0;
//            gaussPtNei = gaussEdgeEval.size() - 1;
//        }
//
//        // This should be outside of loops
//        const scalarField& polyEval = gaussEdgeEval[gaussPt];
//        const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];
//        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];
//        const scalarField& polyGEvalNei = gaussGradEdgeEval[gaussPtNei];
//
//
//        // For owner, calculate diag and off-diag contrib
//        forAll (polyEval, coeffJ)
//        {
//            forAll (polyEval, coeffI)
//            {
//
//                label mtxCoeff = coeffJ*polyEval.size() + coeffI;
//
//                // Consistency term - diag and offdiag contrib
//                diagCoeffOwn[mtxCoeff] = -
//                    polyGEval[coeffI]*polyEval[coeffJ]*Sf/2.0;
//
//                offCoeffOwn[mtxCoeff] = -(
//                  - polyGEval[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);
//
//                offCoeffNei[mtxCoeff] = -(
//                    polyGEvalNei[coeffI]*polyEval[coeffJ]*Sf/2.0);
//
//                diagCoeffNei[mtxCoeff] = -(
//                  - polyGEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);
//
//                // Symmetry term - diag and offdiag contrib
//                diagCoeffOwn[mtxCoeff] += -(
//                    polyEval[coeffI]*polyGEval[coeffJ]*Sf/2.0);
//
//                offCoeffOwn[mtxCoeff] += -(
//                    polyEval[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);
//
//                offCoeffNei[mtxCoeff] += -(
//                    - polyEvalNei[coeffI]*polyGEval[coeffJ]*Sf/2.0);
//
//                diagCoeffNei[mtxCoeff] += -(
//                    - polyEvalNei[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);
//
//
//                // Penalty term T1
//                diagCoeffOwn[mtxCoeff] +=
//                    polyEval[coeffI]*polyEval[coeffJ]*Sf*eta;
//
//                // Penalty term T3
//                offCoeffOwn[mtxCoeff] +=
//                  - polyEval[coeffI]*polyEvalNei[coeffJ]*Sf*eta;
//
//                // Penalty term T2
//                offCoeffNei[mtxCoeff] +=
//                  - polyEvalNei[coeffI]*polyEval[coeffJ]*Sf*eta;
//
//                // Penalty term T4
//                diagCoeffNei[mtxCoeff] +=
//                    polyEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf*eta;
//
//
//                // For cell owner
//                ds[own](coeffJ, coeffI) += diagCoeffOwn[mtxCoeff];
//                us[faceI](coeffJ, coeffI) += offCoeffNei[mtxCoeff];
//
//                ds[nei](coeffJ, coeffI) += diagCoeffNei[mtxCoeff];
//                ls[faceI](coeffJ, coeffI) += offCoeffOwn[mtxCoeff];
//            }
//        }
//    }
//}
//
//    forAll(mesh.mesh().boundaryMesh(), patchI)
//    {
//        const dgPatchField<Type>& pf = vf.boundaryField()[patchI];
//
//        FieldField<Field, scalar> vic = pf.valueInternalCoeffs();
//        tmp<dgScalarField> vbc = pf.valueBoundaryCoeffs();
//
//
//        forAll(vbc(), cellI)
//        {
//            dgm.source()[cellI] -= vbc()[cellI];
////            dgm.source()[cellI] += vbc()[cellI];
//
//            forAll (vbc()[cellI], addrI)
//            {
//                forAll (vbc()[cellI], addrJ)
//                {
//                    label addr = addrI*vbc()[cellI].size() + addrJ;
//
////                    ds[cellI](addrI, addrJ) += vic[cellI][addr];
//                    ds[cellI](addrI, addrJ) -= vic[cellI][addr];
//                }
//            }
//        }
//    }
//
//
//    Info << "SIP OUT diag " << ds << nl << endl;
//    Info << "SIP OUT upper " << us << nl << endl;
//    Info << "SIP OUT lower " << ls << nl << endl;
//    Info << "SIP OUT source " << dgm.source() << endl;
//
//    return tdgm;
//}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
