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

#include "bosSIPLaplacianScheme.H"
//#include "surfaceInterpolate.H"
//#include "dgcDiv.H"
#include "dgcVolumeIntegrate.H"
#include "dgMatrices.H"
#include "ExpandTensorNField.H"

#include "BoSSScpp.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type, class GType>
void bosSIPLaplacianScheme<Type, GType>::calcEta()
{
    Info << "Helo from BoSSS - SIP" << endl;
    
    if (!etaPtr_)
    {
        const polyMesh& mesh = this->mesh().mesh();;
        const dgBase& polynomials = this->mesh().polynomials();

        scalar sqrP = sqr(scalar(polynomials.order()));

        const pointField& points = mesh.points();

        // Eta should be determined based on min or max cell dimension
        scalarField cellSize(mesh.cellCentres().size(), 0);

        // Cell size is determined by creating per-cell bounding box and taking
        // the maximum dimension (span) of the bounding box
        forAll (cellSize, cellI)
        {
            labelList pointLabels = mesh.cellPoints(cellI);
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


        etaPtr_ = new scalarField(sqrP/cellSize);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//SIPLaplacianScheme<Type, GType>::dgmLaplacianUncorrected
//(
//    const surfaceScalarField& gammaMagSf,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    tmp<surfaceScalarField> tdeltaCoeffs =
//        this->tsnGradScheme_().deltaCoeffs(vf);
//    const surfaceScalarField& deltaCoeffs = tdeltaCoeffs();
//
//    tmp<dgMatrix<Type> > tdgm
//    (
//        new dgMatrix<Type>
//        (
//            vf,
//            deltaCoeffs.dimensions()*gammaMagSf.dimensions()*vf.dimensions()
//        )
//    );
//    dgMatrix<Type>& dgm = tdgm();
//
//    dgm.upper() = deltaCoeffs.internalField()*gammaMagSf.internalField();
//    dgm.negSumDiag();
//
//    forAll(dgm.psi().boundaryField(), patchI)
//    {
//        const dgPatchField<Type>& psf = dgm.psi().boundaryField()[patchI];
//        const dgsPatchScalarField& patchGamma =
//            gammaMagSf.boundaryField()[patchI];
//
//        dgm.internalCoeffs()[patchI] = patchGamma*psf.gradientInternalCoeffs();
//        dgm.boundaryCoeffs()[patchI] = -patchGamma*psf.gradientBoundaryCoeffs();
//    }
//
//    // Manipulate internal and boundary coeffs for diffusion. Needed for very
//    // special treatment and is currently used only for ensuring implicit
//    // conservation across GGI interface that has partially covered faces. Does
//    // nothing for other dgPatchFields. VV, 8/Mar/2018.
//    forAll(dgm.psi().boundaryField(), patchI)
//    {
//        dgm.psi().boundaryField()[patchI].manipulateGradientCoeffs(dgm);
//    }
//
//    return tdgm;
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgsPatchField, surfaceMesh> >
//SIPLaplacianScheme<Type, GType>::gammaSnGradCorr
//(
//    const surfaceVectorField& SfGammaCorr,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    const dgMesh& mesh = this->mesh();
//
//    tmp<GeometricField<Type, dgsPatchField, surfaceMesh> > tgammaSnGradCorr
//    (
//        new GeometricField<Type, dgsPatchField, surfaceMesh>
//        (
//            IOobject
//            (
//                "gammaSnGradCorr("+vf.name()+')',
//                vf.instance(),
//                mesh,
//                IOobject::NO_READ,
//                IOobject::NO_WRITE
//            ),
//            mesh,
//            SfGammaCorr.dimensions()*
//            vf.dimensions()*mesh.deltaCoeffs().dimensions()
//        )
//    );
//
//    for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
//    {
//        tgammaSnGradCorr().replace
//        (
//            cmpt,
//            SfGammaCorr & dgc::interpolate(dgc::grad(vf.component(cmpt)))
//        );
//    }
//
//    return tgammaSnGradCorr;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
bosSIPLaplacianScheme<Type, GType>::dgcLaplacian
(
    const dimensionedScalar& gamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    
    // Laplace consists of four terms:
    // Volume term, consistency term, symmetry term and penalty term

    // Volume term is (gradU * gradV):
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > tLaplacianVol
    (
        // The term is squared in volumeIntegrateGrad - KEEP IN MIND
        dgc::volumeIntegrateGrad(vf)
    );

    tLaplacianVol().rename("dgLaplacian(" + vf.name() + ')');

    return tLaplacianVol;
}


//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//SIPLaplacianScheme<Type, GType>::dgcLaplacian
//(
//    const dimensionedScalar& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//
////    const dgMesh& mesh = this->mesh();
//
//    // Laplace consists of four terms:
//    // Volume term, consistency term, symmetry term and penalty term
//
//    // Volume term is (gradU * gradV):
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > tLaplacianVol
//    (
//        // The term is squared in volumeIntegrateGrad - KEEP IN MIND
//        dgc::volumeIntegrateGrad(vf)
//    );
//
//    tLaplacianVol().rename("dgLaplacian(" + vf.name() + ')');
//
//    return tLaplacianVol;
//}


//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//SIPLaplacianScheme<Type, GType>::dgcLaplacian
//(
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    const dgMesh& mesh = this->mesh();
//
//    // gradU * gradV
//
//
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > tLaplacian
//    (
//        vf
////        dgc::div(this->tsnGradScheme_().snGrad(vf)*mesh.magSf())
//    );
//
//    tLaplacian().rename("dgLaplacian(" + vf.name() + ')');
//
//    return tLaplacian;
//}


template<class Type, class GType>
tmp<dgMatrix<Type> >
bosSIPLaplacianScheme<Type, GType>::dgmLaplacian
(
    const dimensionedScalar& gamma,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    
    const dgMesh& mesh = this->mesh();

    tmp<dgMatrix<Type> > tdgm
    (
        new dgMatrix<Type>
        (
            vf,
            vf.dimensions()
        )
    );
    dgMatrix<Type>& dgm = tdgm();

    BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = dgm.GetBoSSSobject();

    BoSSS::Application::ExternalBinding::FixedOperators* BoSSSOp = new BoSSS::Application::ExternalBinding::FixedOperators();
    BoSSSOp->Laplacian(bosssMtx);
    delete BoSSSOp;

/*
    const dgBase& polynomials = mesh.polynomials();

    // Calculate penalty
    calcEta();
    scalarField& etaCells = *etaPtr_;

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
        = dgm.diag().asSquare();

{
    scalarField gradCoeffs(ds[0].size(), 0.0);
    scalarField coeffs(polynomials.quadratureOrder(), 0.0);

    const PtrList<scalarField> polyEval = polynomials.wtdGaussGradEval();
    const PtrList<scalarField>& unwPolyEval = polynomials.gaussPtsGradEval();

    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(coeffs, pti)
    {
        // Zeroth entry is for coordinate -1
        label ptI = pti + 1;

        forAll (polynomials, modI)
        {
            label coeff = modI*polynomials.size() + modI;
            gradCoeffs[coeff] += unwPolyEval[ptI][modI]*polyEval[ptI][modI];
        }
    }

    const scalarField& scaleCells = mesh.cellScaleCoeffs();

    forAll (vf, cellI)
    {
        forAll (polynomials, modI)
        {
            forAll (polynomials, modJ)
            {
                label coeff = modI*polynomials.size() + modJ;

                ds[cellI](modI, modJ) =
                    gradCoeffs[coeff]/scaleCells[cellI];
            }
        }
    }
}

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ls
        = dgm.lower().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& us
        = dgm.upper().asSquare();

// CONSISTENCY TERM
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


        scalar eta = etaCells[own];


        // Greater eta is chosen between owner and neighbour
        if (etaCells[own] < etaCells[nei])
        {
            eta = etaCells[nei];
        }


        // Matrix size
        scalar mtxSize = Type::coeffLength;
        mtxSize *= mtxSize;

        scalarField diagCoeffOwn(mtxSize, 0.0);
        scalarField offCoeffOwn(mtxSize, 0.0);

        scalarField diagCoeffNei(mtxSize, 0.0);
        scalarField offCoeffNei(mtxSize, 0.0);

        scalar Sf = mag(faceSf[faceI]);

        label gaussPt = 0;
        label  gaussPtNei = gaussEdgeEval.size() - 1;

        // Owner local coord = 1 on face
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
        }

        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];
        const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];
        const scalarField& polyGEvalNei = gaussGradEdgeEval[gaussPtNei];


        // For owner, calculate diag and off-diag contrib
        forAll (polyEval, coeffJ)
        {
            forAll (polyEval, coeffI)
            {

                label mtxCoeff = coeffJ*polyEval.size() + coeffI;

                // Consistency term - diag and offdiag contrib
                diagCoeffOwn[mtxCoeff] = -
                    polyGEval[coeffI]*polyEval[coeffJ]*Sf/2.0;

                offCoeffOwn[mtxCoeff] = -(
                  - polyGEval[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);

                offCoeffNei[mtxCoeff] = -(
                    polyGEvalNei[coeffI]*polyEval[coeffJ]*Sf/2.0);

                diagCoeffNei[mtxCoeff] = -(
                  - polyGEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);

                // Symmetry term - diag and offdiag contrib
                diagCoeffOwn[mtxCoeff] += -(
                    polyEval[coeffI]*polyGEval[coeffJ]*Sf/2.0);

                offCoeffOwn[mtxCoeff] += -(
                    polyEval[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);

                offCoeffNei[mtxCoeff] += -(
                    - polyEvalNei[coeffI]*polyGEval[coeffJ]*Sf/2.0);

                diagCoeffNei[mtxCoeff] += -(
                    - polyEvalNei[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);


                // Penalty term T1
                diagCoeffOwn[mtxCoeff] +=
                    polyEval[coeffI]*polyEval[coeffJ]*Sf*eta;

                // Penalty term T3
                offCoeffOwn[mtxCoeff] +=
                  - polyEval[coeffI]*polyEvalNei[coeffJ]*Sf*eta;

                // Penalty term T2
                offCoeffNei[mtxCoeff] +=
                  - polyEvalNei[coeffI]*polyEval[coeffJ]*Sf*eta;

                // Penalty term T4
                diagCoeffNei[mtxCoeff] +=
                    polyEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf*eta;


                // For cell owner
                ds[own](coeffJ, coeffI) += diagCoeffOwn[mtxCoeff];
                us[faceI](coeffJ, coeffI) += offCoeffNei[mtxCoeff];

                ds[nei](coeffJ, coeffI) += diagCoeffNei[mtxCoeff];
                ls[faceI](coeffJ, coeffI) += offCoeffOwn[mtxCoeff];
            }
        }
    }
}

    // Setup BC's
    forAll(mesh.mesh().boundaryMesh(), patchI)
    {
        const dgPatchField<Type>& pf = vf.boundaryField()[patchI];

        FieldField<Field, scalar> vic = pf.valIntCoeffsLaplace();
        tmp<dgScalarField> vbc = pf.valBouCoeffsLaplace();


        forAll(vbc(), cellI)
        {
            dgm.source()[cellI] -= vbc()[cellI]*gamma.value();

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

//    ds = ds*gamma.value();
//    ls = ls*gamma.value();
//    us = us*gamma.value();
*/
    return tdgm;
}


//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//SIPLaplacianScheme<Type, GType>::dgcLaplacian
//(
//    const GeometricField<GType, dgsPatchField, surfaceMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    const dgMesh& mesh = this->mesh();
//
//    const surfaceVectorField Sn(mesh.Sf()/mesh.magSf());
//    const surfaceVectorField SfGamma(mesh.Sf() & gamma);
//    const GeometricField<scalar, dgsPatchField, surfaceMesh> SfGammaSn
//    (
//        SfGamma & Sn
//    );
//    const surfaceVectorField SfGammaCorr(SfGamma - SfGammaSn*Sn);
//
//    tmp<GeometricField<Type, dgPatchField, cellMesh> > tLaplacian
//    (
//        dgc::div
//        (
//            SfGammaSn*this->tsnGradScheme_().snGrad(vf)
//          + gammaSnGradCorr(SfGammaCorr, vf)
//        )
//    );
//
//    tLaplacian().rename("dgLaplacian(" + gamma.name() + ',' + vf.name() + ')');
//
//    return tLaplacian;
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
