label cCorr(0);
Switch converged(false);

C.SyncToBoSSS();

do {
  cCorr++;
  C.storePrevIter();
  // psi.storePrevIter();
  U.storePrevIter();


  U.SyncToBoSSS();
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
      // dgm::ddt(C)
      //     +
      dgm::dgCahnHilliard(C, U, Phi)
      // dgm::dgCahnHilliard(C, C)
      // dgm::dgLaplacian(C)
      // + fvm::Sp(dfdcCoeffs,C)
      // + dfdcSource
  );

  // double rhob_ = 1;
  // double rhoa_ = 2;

  // rhoPhi =
  //   CEqn().flux()/2 * (rhob_ - rhoa_)
  //   + CEqn.Phi * (rhob_ + rhoa_)/2;
  Info << CEqn.flux() << endl;
  // Info << U << endl;
  // Info << CEqn.flux_ << endl;

  // CEqn.solveBoSSS();
  C.SyncFromBoSSS();
  U.SyncFromBoSSS();


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
