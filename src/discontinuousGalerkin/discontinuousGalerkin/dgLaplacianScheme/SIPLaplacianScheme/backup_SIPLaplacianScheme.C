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

#include "SIPLaplacianScheme.H"
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
tmp<GeometricField<Type, dgPatchField, cellMesh> >
SIPLaplacianScheme<Type, GType>::dgcLaplacian
(
    const dimensionedScalar& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
//    const dgMesh& mesh = this->mesh();

    // Laplace consists of four terms:
    // Volume term, consistency term, symmetry term and penalty term

    // Volume term is (gradU * gradV):
    tmp<GeometricField<Type, dgPatchField, cellMesh> > tLaplacianVol
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
SIPLaplacianScheme<Type, GType>::dgmLaplacian
(
    const dimensionedScalar& gamma,
    const GeometricField<Type, dgPatchField, cellMesh>& vf
)
{
    Info << "MATRIX CONSTRUCTOR: " << endl;

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

//    Info << "CONSTRUCT MATRIX OUT " << endl;
    dgPolynomials polynomials;


    scalar eta = 1.0;//6.0;

// VOLUME TERM
//    Info << "VOLUME TERM IN " << endl;

//    typename CoeffField<VectorN<scalar, Type::coeffLength> >::linearTypeField& d
//        = dgm.diag().asLinear();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
        = dgm.diag().asSquare();

{
    scalarField gradCoeffs(ds[0].size(), 0.0);
    scalarField coeffs(Type::coeffLength, 0.0);

    const PtrList<scalarField> polyEval = polynomials.wtdGaussGradEval();
    const PtrList<scalarField>& unwPolyEval = polynomials.gaussPtsGradEval();

    // Go over all Gaussian points for the whole mesh and add value*weight
    forAll(coeffs, pti)
    {
        // Zeroth entry is for coordinate -1
        label ptI = pti + 1;

        forAll (coeffs, modI)
        {
            forAll (coeffs, modJ)
            {
                label coeff = modI*coeffs.size() + modJ;
                gradCoeffs[coeff] += unwPolyEval[ptI][modJ]*polyEval[ptI][modI];

//            Info << "modI: " << modI << ", modJ: " << modJ << ", gradCoeff:"
//                 << gradCoeffs[coeff] << endl;
            }
        }
    }

//Info << "GRAD COEFFS: " << gradCoeffs << endl;

    const scalarField& scaleCells = mesh.cellScaleCoeffs();

    forAll (vf, cellI)
    {
        forAll (coeffs, modI)
        {
            forAll (coeffs, modJ)
            {
                label coeff = modI*coeffs.size() + modJ;

                ds[cellI](modI, modJ) =
                    gradCoeffs[coeff]*scaleCells[cellI];// + SMALL;
            }
        }
    }
}


    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ls
        = dgm.lower().asSquare();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& us
        = dgm.upper().asSquare();
//
//
//    typename CoeffField<VectorN<scalar, Type::coeffLength> >::linearTypeField& u
//        = dgm.upper().asLinear();
//
//    typename CoeffField<VectorN<scalar, Type::coeffLength> >::linearTypeField& l
//        = dgm.lower().asLinear();


//    Info << "VOLUME TERM OUT " << endl;
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

    const vectorField& faceSf = mesh().faceAreas();//Centres();

//    const labelList& ownList = mesh().faceOwner();
    const labelList& neiList = mesh().faceNeighbour();

    // Go over all internal faces (only those that have nei)
    forAll (neiList, faceI)
    {

        // Nei is -1 for boundary faces
        const label& own = mesh().faceOwner()[faceI];
        const label& nei = mesh().faceNeighbour()[faceI];

        // If neighbour exists it is internal face
//        {
            // If own(C().x()) < nei(C().x()), face coord=1, otherwise -1

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

            // Set the scope - calculate owner cell first
            {
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
                    diagCoeffOwn[mtxCoeff] = -(
                        polyGEval[coeffI]*polyEval[coeffJ]*Sf/2.0
                      - polyGEval[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);


                    offCoeffOwn[mtxCoeff] = -(
                        polyGEvalNei[coeffI]*polyEval[coeffJ]*Sf/2.0
                      - polyGEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);

//Info << "PolyGEval[" << coeffI << "]: " << polyGEval[coeffI] << ", gsum:"
//     << gSum(polyEval) << ", Sf: " << Sf << endl;
//Info << "DIAG OWN: " << diagCoeffOwn[coeffI]
//     << ", OFF OWN : " << offCoeffOwn[coeffI] << endl;


                    // Symmetry term - diag and offdiag contrib
                    diagCoeffOwn[mtxCoeff] += -(
                        polyEval[coeffI]*polyGEval[coeffJ]*Sf/2.0
                      + polyEval[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);

                    offCoeffOwn[mtxCoeff] -= -(
                        polyEvalNei[coeffI]*polyGEval[coeffJ]*Sf/2.0
                      + polyEvalNei[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);


                    // Penalty term - diag and offdiag contrib
                    diagCoeffOwn[mtxCoeff] +=
                        polyEval[coeffI]*polyEval[coeffJ]*Sf*eta;
//                      - polyEval[coeffI]*polyEvalNei[coeffJ]*Sf*eta;


                    offCoeffOwn[mtxCoeff] +=
                        - polyEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf*eta;
//                      - polyEvalNei[coeffI]*polyEval[coeffJ]*Sf*eta;




                    // Consistency term - diag and offdiag contrib
//                    diagCoeffOwn[mtxCoeff] = -(
//                        polyGEval[coeffI]*polyEval[coeffJ]*Sf/2.0
//                      - polyGEval[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);
//
//
//                    offCoeffOwn[mtxCoeff] = -(
//                        polyGEvalNei[coeffI]*polyEval[coeffJ]*Sf/2.0
//                      - polyGEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);
//
////Info << "PolyGEval[" << coeffI << "]: " << polyGEval[coeffI] << ", gsum:"
////     << gSum(polyEval) << ", Sf: " << Sf << endl;
////Info << "DIAG OWN: " << diagCoeffOwn[coeffI]
////     << ", OFF OWN : " << offCoeffOwn[coeffI] << endl;
//
//
//                    // Symmetry term - diag and offdiag contrib
//                    diagCoeffOwn[mtxCoeff] += -(
//                        polyEval[coeffI]*polyGEval[coeffJ]*Sf/2.0
//                      + polyEval[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);
//
//                    offCoeffOwn[mtxCoeff] -= -(
//                        polyEvalNei[coeffI]*polyGEval[coeffJ]*Sf/2.0
//                      + polyEvalNei[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);
//
//
//                    // Penalty term - diag and offdiag contrib
//                    diagCoeffOwn[mtxCoeff] +=
//                        polyEval[coeffI]*polyEval[coeffJ]*Sf*eta;
////                      - polyEval[coeffI]*polyEvalNei[coeffJ]*Sf*eta;
//
//
//                    offCoeffOwn[mtxCoeff] +=
//                        - polyEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf*eta;
////                      - polyEvalNei[coeffI]*polyEval[coeffJ]*Sf*eta;

                    // For cell owner
                    ds[own](coeffJ, coeffI) += diagCoeffOwn[mtxCoeff];
//                    us[faceI](coeffJ, coeffI) += offCoeffOwn[mtxCoeff];

                    ls[faceI](coeffJ, coeffI) += offCoeffOwn[mtxCoeff];

//                Info << "DIAGONAL OWN: " << d[own](coeffI) << ", coeff: "
//                     << coeffI << ", cell: " << own << endl;
                    }
//                }
            }
//Info << "GGGG OUT" << endl;
//
//    Info << "SIP OUT diag " << ds << nl << endl;
            // Switch local coords for nei cell
            label saveLabel = gaussPt;
            gaussPt = gaussPtNei;
            gaussPtNei = saveLabel;

            // Set the scope - calculate neighbour cell
            {
                // This should be outside of loops
                const scalarField& polyEval = gaussEdgeEval[gaussPt];
                const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];
                const scalarField& polyGEval = gaussGradEdgeEval[gaussPt];
                const scalarField& polyGEvalNei = gaussGradEdgeEval[gaussPtNei];

                // For neighbour, calculate diag and off-diag contrib
                forAll (polyEval, coeffJ)
                {
                    forAll (polyEval, coeffI)
                    {

                    label mtxCoeff = coeffJ*polyEval.size() + coeffI;

//                    // Consistency term
//                    diagCoeffNei[mtxCoeff] = -(
//                        polyGEval[coeffI]*polyEval[coeffJ]*Sf/2.0
//                      - polyGEval[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);
//
//                    offCoeffNei[mtxCoeff] = -(
//                        polyGEvalNei[coeffI]*polyEval[coeffJ]*Sf/2.0
//                      - polyGEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf/2.0);
//
//                    // Symmetry term - diag and offdiag contrib
//                    diagCoeffNei[mtxCoeff] += -(
//                        polyEval[coeffI]*polyGEval[coeffJ]*Sf/2.0
//                      + polyEval[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);
//
//                    offCoeffNei[mtxCoeff] -= -(
//                        polyEvalNei[coeffI]*polyGEval[coeffJ]*Sf/2.0
//                      + polyEvalNei[coeffI]*polyGEvalNei[coeffJ]*Sf/2.0);
//
//
//                    // Penalty term - diag and offdiag contrib
//                    diagCoeffNei[mtxCoeff] +=
//                        polyEval[coeffI]*polyEval[coeffJ]*Sf*eta;
////                      - polyEval[coeffI]*polyEvalNei[coeffJ]*Sf*eta;

                    offCoeffNei[mtxCoeff] +=
                        - polyEvalNei[coeffI]*polyEvalNei[coeffJ]*Sf*eta;
//                      - polyEvalNei[coeffI]*polyEval[coeffJ]*Sf*eta;

                    // For cell nei
                    ds[nei](coeffJ, coeffI) += diagCoeffNei[mtxCoeff];
                    ls[faceI](coeffJ, coeffI) += offCoeffNei[mtxCoeff];

//                Info << "DIAGONAL NEI: " << d[nei](coeffI) << ", coeff: "
//                     << coeffI << ", cell: " << own << endl;
                    }
                }
            }
        }
    }
}

//    Info << "SIP OUT diag " << ds << nl << endl;
//    Info << "VALUEAING BOUDNARYI FIELDS" << endl;


    forAll(mesh.mesh().boundaryMesh(), patchI)
    {
        const dgPatchField<Type>& pf = vf.boundaryField()[patchI];

        FieldField<Field, scalar> vic = pf.valueInternalCoeffs();
        tmp<dgScalarField> vbc = pf.valueBoundaryCoeffs();

        forAll(vbc(), cellI)
        {
            dgm.source()[cellI] += vbc()[cellI];

            forAll (vbc()[cellI], addrI)
            {
                forAll (vbc()[cellI], addrJ)
                {
                    label addr = addrI*vbc()[cellI].size() + addrJ;

                    ds[cellI](addrI, addrJ) += vic[cellI][addr];
                }
            }
        }
    }
//    Info << "And 1" << endl;
//        const dgPatchField<Type>& pf = vf.boundaryField()[patchI];
//
//    Info << "And 2" << endl;
////        Info << pf.valueBoundaryCoeffs() << endl;
//
//    }
//
    Info << "SIP OUT diag " << ds << nl << endl;
//    Info << "SIP OUT diag " << d << endl;
    Info << "SIP OUT upper " << us << nl << endl;
    Info << "SIP OUT lower " << ls << nl << endl;
    Info << "SIP OUT source " << dgm.source() << endl;

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
