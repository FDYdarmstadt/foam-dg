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
    // Info << "vf.PatchFieldType" << endl;
    // Info << vf.PatchFieldType << endl;
    dgMatrix<Type>& dgm = tdgm();

    BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = dgm.GetBoSSSobject();
    BoSSS::Application::ExternalBinding::OpenFoamPatchField* bosssPtch = vf.bosssObjectPatchField_;

    BoSSS::Application::ExternalBinding::FixedOperators* BoSSSOp = new BoSSS::Application::ExternalBinding::FixedOperators();
    BoSSSOp->Laplacian(bosssMtx, bosssPtch);
    delete BoSSSOp;

    dgm.SetBoSSSobject(bosssMtx);

    // Info << "Setting up BCs" << endl;
    // // Setup BC's
    // forAll(mesh.mesh().boundaryMesh(), patchI)
    // {
    //     const dgPatchField<Type>& pf = vf.boundaryField()[patchI];

    //     FieldField<Field, scalar> vic = pf.valIntCoeffsLaplace();
    //     tmp<dgScalarField> vbc = pf.valBouCoeffsLaplace();


    //     Info << "gamma value: " << endl;
    //     forAll(vbc(), cellI)
    //     {
    //         auto tmp = - vbc()[cellI]*gamma.value();
    //         Info << tmp << " ";
    //         // bosssMtx->_SetRHScoordinate(0, cellI, 0, tmp);
    //         for (int j = 0; j < dgOrder::length; j++)
    //             bosssMtx->SetRHScoordinate(0, cellI, j, tmp[j]);
    //         // dgm.source()[cellI] -= vbc()[cellI]*gamma.value();

    //         forAll (vbc()[cellI], addrI)
    //         {
    //             forAll (vbc()[cellI], addrJ)
    //             {
    //                 label addr = addrI*vbc()[cellI].size() + addrJ;

    //                 int addrIInt = addrI;
    //                 int addrJInt = addrJ;
    //                 double inputBuffer[] = {-vic[cellI][addr]};
    //                 double* inputBufPtr = inputBuffer;
    //                 Info << -vic[cellI][addr] << endl;
    //                 // inputBuffer[0] = -vic[cellI][addr];
    //                 bosssMtx->AccBlock(addrIInt,addrJInt,1.0,inputBufPtr);
    //                 // ds[cellI](addrI, addrJ) -= vic[cellI][addr];
    //             }
    //         }
    //     }
    //     Info << endl;
    // }

    // TODO sync this stuff from BoSSS
    // Info << tdgm.GetBoSSSobject() << endl;
    // dgm.SyncFromBoSSS();

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
