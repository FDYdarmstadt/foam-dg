
label cCorr(0);
Switch converged(false);

// C.SyncToBoSSS();


do {

  u.importFromVolVecField();
  cCorr++;
  C.storePrevIter();
  // psi.storePrevIter();
  U.storePrevIter();


  // //- Assemble equations
  // fvScalarMatrix CEqn
  // (
  //     - fvm::laplacian(epsSqr,C)
  //     + fvm::Sp(dfdcCoeffs,C)
  //     + dfdcSource
  // );
  //
  // CEqn.relax();
  //
  // fvScalarMatrix psiEqn
  // (
  //     - fvm::laplacian(M,psi)
  // );
  //
  // psiEqn.relax();
  //
  // // Prepare block system
  // fvBlockMatrix<vector2> blockM(blockCH);
  //
  // // Insert equations into block Matrix
  // blockM.insertEquation(0, CEqn);
  // blockM.insertEquation(1, psiEqn);
  //
  // scalarField BA(mesh.nCells(), -1.0);
  //
  // blockM.insertEquationCoupling(0, 1, BA);
  //
  // blockM.insertEquationCoupling
  // (
  //     1,
  //     0,
  //     fvm::ddt(C)
  // );


  // //- Assemble equations
  dgScalarMatrix CEqn(
      dgm::dgCahnHilliard(c, u, psi, phi, Cn, D) //phi : volumetric face flux
  );

  // rhoPhi =
  //   // Flux/2 * 0
  //   Phi * rhob_;
  //   // Flux/2 * (rhob - rhoa)
  //   // + Phi * (rhob + rhoa)/2;

  // fvVectorMatrix UEqn
  //   (
  //     fvm::ddt(rho, U.volVecField())
  //     + fvm::div(rhoPhi, U.volVecField())
  //     // - fvm::laplacian(muEff, U)
  //     // - (fvc::grad(U) & fvc::grad(muEff))
  //   //- fvc::div(muEff*(fvc::interpolate(dev(fvc::grad(U))) & mesh.Sf()))
  //   );

  //   UEqn.relax();
  //   // UEqn.solve();

  //   U.importFromVolVecField();
  //   U.SyncToBoSSS();


  // CEqn.relax();

  // fvScalarMatrix psiEqn
  // (
  //     fvm::Sp(1.0,psi)
  //     - dfdcSource

  // );

  // psiEqn.relax();

  // Prepare block system
  // fvBlockMatrix<vector2> blockM(blockCH);

  // // Insert equations into block Matrix
  // blockM.insertEquation(0, CEqn);
  // blockM.insertEquation(1, psiEqn);

  // blockM.insertEquationCoupling
  // (
  //     0,
  //     1,
  //     -fvm::laplacian(M,psi)
  // );

  // blockM.insertEquationCoupling
  // (
  //     1,
  //     0,
  //      fvm::laplacian(epsSqr,C)
  //     - fvm::Sp(dfdcCoeffs,C)
  // );

  // //- Block coupled solver call
  // blockM.solve();

  // // Retrieve solution
  // blockM.retrieveSolution(0, C.internalField());
  // blockM.retrieveSolution(1, psi.internalField());
  // C.correctBoundaryConditions();
  // psi.correctBoundaryConditions();

  // //- Update dfdc coeffs
  // dfdcCoeffs =   fa*sqr(C);

  // //- Update dfdc source
  // dfdcSource =  -fb*C;

  // //- Update potential
  // psi = (dfdcCoeffs*C + dfdcSource) - fvc::laplacian(epsSqr,C);
  // psi.correctBoundaryConditions();

  //- Compute new free energy
  // scalar avgFreeEnergyNew(
  //     (fvc::domainIntegrate((fa / 4.0) * sqr(sqr(C) - fb / fa) +
  //                           epsSqr / 2.0 * magSqr(fvc::grad(C))) /
  //      vol)
  //         .value());

  // scalar freeEnergyRes(mag(avgFreeEnergyNew - avgFreeEnergy) /
  //                      (mag(avgFreeEnergy) + SMALL));

  // Info << "Residual on free energy : " << freeEnergyRes << endl;

  // avgFreeEnergy = avgFreeEnergyNew;

// # include "CHconvergence.H"

  converged = true;
} while (!converged);

// After solution of the CH equation:

//- Update FV fields from DG fields
c.SyncFromBoSSS();


Info << Psi << endl;

//- Update mixture properties
// rho == (scalar(1) + C) / 2. * rhoa + (scalar(1) - C) / 2. * rhob;
// mu == (scalar(1) + C) / 2. * rhoa *nua + (scalar(1) - C) / 2. * rhob *nub;
rho = (scalar(1) + C) / 2. * rhoa + (scalar(1) - C) / 2. * rhob; // TODO = or == ???
mu = (scalar(1) + C) / 2. * rhoa *nua + (scalar(1) - C) / 2. * rhob *nub;

//- Update mass flux from dgScalarMatrix CEqn
// rhoPhi = CEqn.Flux() / 2 * (rhob - rhoa) + phi * (rhob + rhoa) / 2;
rhoPhi = phi / 2 * (rhob - rhoa) + phi * (rhob + rhoa) / 2; // TODO is phi really what Holger means with CEqn.Flux()?
