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

#include "upwindDivScheme.H"
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



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<DgGeometricField<Type, dgPatchField, cellMesh> >
upwindDivScheme<Type, GType>::dgcDiv
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

    tLaplacianVol().rename("dgDiv(" + vf.name() + ')');

    return tLaplacianVol;
}
//
//
template<class Type, class GType>
tmp<dgMatrix<Type> >
upwindDivScheme<Type, GType>::dgmDiv
(
    const DgGeometricField<dgVector, dgPatchField, cellMesh>& vvf,
    const DgGeometricField<Type, dgPatchField, cellMesh>& vsf
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

    // Check if this is weighted or unweighted
    const PtrList<scalarField>& polyEval = polynomials.gaussPtsEval();
    const PtrList<scalarField>& polyEvalW = polynomials.wtdGaussEval();
    const PtrList<scalarField>& polyGEval = polynomials.wtdGaussGradEval();

    typename CoeffField<VectorN<scalar, Type::coeffLength> >::squareTypeField& ds
        = dgm.diag().asSquare();

    Field<scalarField> gradCoeffs(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsNeg(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsPos(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsCrssPos(ds[0].size(), scalarField(dim, 0.0));
    Field<scalarField> surfaceCoeffsCrssNeg(ds[0].size(), scalarField(dim, 0.0));

    forAll(vsf, cellI)
    {
        // Calculate velocity integral, velocity left and right edge
        forAll (polynomials, modJ)
        {
            forAll (polynomials, modI)
            {
                // Go over all Gaussian points for the whole mesh and add
                for (label ptI = 1; ptI < (polyEval.size() - 1); ptI++)
                {
                    // Zeroth entry is for coordinate -1
                    scalar velocity = 0.0;

                    forAll (polynomials, inner)
                    {
                        velocity += vvf[cellI][0][inner]*polyEval[ptI][inner];
                    }

                    ds[cellI](modJ, modI) += -
                       mag(velocity)*polyEval[ptI][modI]*polyGEval[ptI][modJ];

                    if (mag(ds[cellI](modJ, modI)) < SMALL)
                    {
                        ds[cellI](modJ, modI) = 0;
                    }
                }
            }
        }
    }
//        Info << "DIV DIAGONAL IS: " << dgm.diag() << endl;

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

    // I evaluate polyEval in gauss points and (-1) and (1) so that Ptr
    // size is length+2
    const vectorField& faceSf = mesh().faceAreas();

    const labelList& neiList = mesh().faceNeighbour();

    // Go over all internal faces (only those that have nei)
    forAll (neiList, faceI)
    {
        scalar velocityOwn = 0.0;
        scalar velocityNei = 0.0;

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

        label gaussPt = 0;
        label  gaussPtNei = gaussEdgeEval.size() - 1;
        label ownCo = -1;
        label neiCo = 1;

        Field<scalarField>& surfaceCoeffsOwn = surfaceCoeffsNeg;
        Field<scalarField>& surfaceCoeffsNei = surfaceCoeffsPos;
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
            ownCo = 1;
            neiCo = -1;
        }
        // Owner local coord = -1 on face
        else
        {
            gaussPt = 0;
            gaussPtNei = gaussEdgeEval.size() - 1;
            surfaceCoeffsOwn = surfaceCoeffsPos;
            surfaceCoeffsNei = surfaceCoeffsNeg;
            surfaceCoeffsCrssOwn = surfaceCoeffsCrssNeg;
            surfaceCoeffsCrssNei = surfaceCoeffsCrssPos;
        }

        vector gaussPtVec (ownCo, 0, 0);
        vector gaussPtVecNei (neiCo, 0, 0);
        vector faceUOwn = polynomials.evaluate(vvf[own], gaussPtVec);
        vector faceUNei = polynomials.evaluate(vvf[nei], gaussPtVecNei);


        // Evaluate flux direction on the face, add contribution to according
        // cell
        scalar fluxDirOwn = faceSf[faceI] & faceUOwn;
        scalar fluxDirNei = faceSf[faceI] & faceUNei;

        scalar faceFlux = 0.5*(fluxDirOwn + fluxDirNei);
        scalar uw = pos(faceFlux);


        // This should be outside of loops
        const scalarField& polyEval = gaussEdgeEval[gaussPt];
        const scalarField& polyEvalNei = gaussEdgeEval[gaussPtNei];


        scalarList Aown(mtxSize, 0.0);
        scalarList Anei(mtxSize, 0.0);
        scalarList AownOffd(mtxSize, 0.0);
        scalarList AneiOffd(mtxSize, 0.0);


        forAll(polynomials, modJ)
        {
            velocityOwn += vvf[own][0][modJ]*polyEval[modJ];
            velocityNei += vvf[nei][0][modJ]*polyEvalNei[modJ];
        }

        scalar w = pos(faceFlux);

        // Testing part
        if (w < 1.0)
        {
            Info << "SET TO CENTRAL DIFFERENCE." << endl;
        }
        else
        {
            Info << "SET TO UPWIND." << endl;
        }

        forAll(polynomials, modJ)
        {
            forAll(polynomials, modI)
            {
                label coeff = modJ*polynomials.size() + modI;
                scalarList vectorSize (3, 0.0);


                Aown[coeff] =
                 (w)*faceFlux*polyEval[modJ]*polyEval[modI];

                Anei[coeff] +=
                - (1 - w)*faceFlux*polyEvalNei[modJ]*polyEvalNei[modI];

                AownOffd[coeff] =
                - (1 - w)*faceFlux*polyEvalNei[modJ]*polyEvalNei[modI];

                AneiOffd[coeff] =
                - (w)*faceFlux*polyEvalNei[modJ]*polyEval[modI];
            }
        }

        // For owner, calculate diag and off-diag contrib
        forAll (polyEval, coeffJ)
        {
            forAll (polyEval, coeffI)
            {
                label mtxCoeff = coeffJ*polyEval.size() + coeffI;

                ds[own](coeffJ, coeffI) += Aown[mtxCoeff];
                ds[nei](coeffJ, coeffI) += Anei[mtxCoeff];

                ls[faceI](coeffJ, coeffI) += AneiOffd[mtxCoeff];
                us[faceI](coeffJ, coeffI) += AownOffd[mtxCoeff];
            }
        }
    }
//    Info << "DIV FACE DIAGONAL IS: " << dgm.diag() << nl << endl;
}


    // Setup BC's
    forAll(mesh.mesh().boundaryMesh(), patchI)
    {
        if (!isA<emptyPolyPatch>(mesh.mesh().boundaryMesh()[patchI]))
        {
        const dgPatchField<Type>& psf = vsf.boundaryField()[patchI];
        const dgPatchField<dgVector>& pvf = vvf.boundaryField()[patchI];

        FieldField<Field, scalar> vic = psf.valIntCoeffsDiv(pvf);
        tmp<dgScalarField> vbc = psf.valBouCoeffsDiv(pvf);


        forAll(ds, cellI)
        {
            if (vbc().size())
            {
                dgm.source()[cellI] -= vbc()[cellI];
            }

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

//    Info << "DIV OUT diag " << ds << nl << endl;
//    Info << "DIV OUT source " << dgm.source() << endl;


    return tdgm;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
