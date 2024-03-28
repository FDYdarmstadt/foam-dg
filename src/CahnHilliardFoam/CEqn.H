
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
  dgScalarMatrix CEqn(
      dgm::dgCahnHilliard(c, u, psi, phi, Cn, D) //phi : volumetric face flux
  );

  converged = true;
} while (!converged);

// After solution of the CH equation:

//- Update FV fields from DG fields
c.SyncFromBoSSS();


//- Update mixture properties
rho == (scalar(1) + C) / 2. * rhoa + (scalar(1) - C) / 2. * rhob;
mu == (scalar(1) + C) / 2. * rhoa *nua + (scalar(1) - C) / 2. * rhob *nub;

//- Update mass flux from dgScalarMatrix CEqn
rhoPhi == phi / 2 * (rhob - rhoa) + phi * (rhob + rhoa) / 2;
