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

#include "bosssCahnHilliardScheme.H"
//#include "surfaceInterpolate.H"
//#include "dgcDiv.H"
#include "dgcVolumeIntegrate.H"
#include "dgMatrices.H"
#include "ExpandTensorNField.H"
#include "cellFields.H"
#include "linear.H"

#include "BoSSScpp.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// template<class Type, class GType>


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class VType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
bosssCahnHilliardScheme<Type, VType>::dgcCahnHilliard
(
    // const dimensionedScalar& gamma,
    DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    DgGeometricField<Type, dgPatchField, cellMesh>& phif,
    surfaceScalarField& Flux
)
{
    
    // Laplace consists of four terms:
    // Volume term, consistency term, symmetry term and penalty term

    // Volume term is (gradU * gradV):
    tmp<DgGeometricField<Type, dgPatchField, cellMesh> > tCahnHilliardVol
    (
        // The term is squared in volumeIntegrateGrad - KEEP IN MIND
        dgc::volumeIntegrateGrad(vf)
    );

    tCahnHilliardVol().rename("dgCahnHilliard(" + vf.name() + " " + Uf.name() + ')');

    return tCahnHilliardVol;
}

template<class Type, class VType>
tmp<dgMatrix<Type> >
bosssCahnHilliardScheme<Type, VType>::dgmCahnHilliard
(
    // const dimensionedScalar& gamma,
    DgGeometricField<Type, dgPatchField, cellMesh>& vf,
    DgGeometricField<VType, dgPatchField, cellMesh>& Uf,
    // const DgGeometricField<VType, dgPatchField, cellMesh>& phif
    DgGeometricField<Type, dgPatchField, cellMesh>& phif,
    surfaceScalarField& Flux
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

    // tmp<dgMatrix<VType> > Utdgm
    // (
    //     new dgMatrix<VType>
    //     (
    //         Uf,
    //         Uf.dimensions()
    //     )
    // );
    // Info << "vf.PatchFieldType" << endl;
    // Info << vf.PatchFieldType << endl;
    // dgMatrix<Type>& Udgm = Utdgm();

    BoSSS::Application::ExternalBinding::OpenFoamMatrix* bosssMtx = dgm.GetBoSSSobject();
    // BoSSS::Application::ExternalBinding::OpenFoamMatrix* UbosssMtx = Udgm.GetBoSSSobject();
    BoSSS::Application::ExternalBinding::OpenFoamPatchField* bosssPtch = vf.bosssObjectPatchField_;
    BoSSS::Application::ExternalBinding::OpenFoamPatchField* bosssPtchU = Uf.bosssObjectPatchField_;
    BoSSS::Application::ExternalBinding::OpenFoamDGField* U = Uf.bosssObject_;
    // cellScalarField Phi(vf);

    // int nFaces = mesh().nFaces();
    int nFaces = mesh().nInternalFaces();
    double* flxArray = (double*) malloc(sizeof(double) * nFaces);
    for (int iF = 0; iF < nFaces; iF++){
        Info << Flux[iF] << endl;
        flxArray[iF] = Flux[iF];
        Info << flxArray[iF] << endl << endl;
    }
    BoSSS::Foundation::Grid::OpenFOAMGrid* ofMesh = mesh.GetBoSSSobject();
    BoSSS::Application::ExternalBinding::OpenFoamSurfaceField* Flx = new BoSSS::Application::ExternalBinding::OpenFoamSurfaceField(ofMesh, flxArray, nFaces);

    BoSSS::Application::ExternalBinding::FixedOperators* BoSSSOp = new BoSSS::Application::ExternalBinding::FixedOperators();
    // BoSSSOp->CahnHilliard(bosssMtx, UbosssMtx, bosssPtch, bosssPtchU);
    double dt = vf.time().deltaT().value();
    BoSSSOp->CahnHilliard(bosssMtx, Flx, U, bosssPtch, bosssPtchU, dt);
    BoSSS::Application::ExternalBinding::OpenFoamDGField* PhiDGField = BoSSSOp->GetMu();
    // BoSSS::Application::ExternalBinding::OpenFoamDGField* FluxDGField = BoSSSOp->GetFlux();
    vf.SyncFromBoSSS();
    Uf.SyncFromBoSSS();
    phif.SyncFromBoSSSDGField(PhiDGField);
    // Flux.SyncFromBoSSSDGField(FluxDGField);

    // label J = mesh().nCells();
    // dgm.flux_ = Field<Vector<Type>>(J);
    // OpenFoamDGField *bo = dgf.GetBoSSSobject();

    Uf.SyncFromBoSSS();

    Flux = linearInterpolate(Uf.volVecField() * vf.volScaField()) & Uf.dgmesh().finVolMesh()->Sf();

    // forAll(Flux.mesh().interfaces(), interfaceI){
    //     int cellA = Flux.mesh().faceOwner()[interfaceI];
    //     double fA = U[cellA] &
    //     Flux[interfaceI] = Flux.
    // }
    // for (label j = 0; j < J; j++) {
    //   // dgVector cellValue = dgf[j];
    //   // int N = Flux[j].size();
    //   int N = 3;
    //   // int N = 1;
    //   for (int n = 0; n < N; n++) {
    //     for (int d = 0; d < dgOrder::length; d++) {
    //       Info << "flux: " << endl;
    //       Info << Flux[j] << endl;
    //       Info << "Uf: " << endl;
    //       Info << Uf[j][0] << endl;
    //       // Flux[j][n][d] = FluxDGField->GetDGcoordinate(n, j, d);
    //     }
    //   }
    // }
    delete BoSSSOp;

    dgm.SetBoSSSobject(bosssMtx);

    return tdgm;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
