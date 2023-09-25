
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
Info << "max(Psi)" << max(Psi) << endl;
Info << "min(Psi)" << min(Psi) << endl;


//- Update mixture properties
// rho == (scalar(1) + C) / 2. * rhoa + (scalar(1) - C) / 2. * rhob;
// mu == (scalar(1) + C) / 2. * rhoa *nua + (scalar(1) - C) / 2. * rhob *nub;
rho == (scalar(1) + C) / 2. * rhoa + (scalar(1) - C) / 2. * rhob; // TODO = or == ???
mu == (scalar(1) + C) / 2. * rhoa *nua + (scalar(1) - C) / 2. * rhob *nub;

//- Update mass flux from dgScalarMatrix CEqn
// rhoPhi = CEqn.Flux() / 2 * (rhob - rhoa) + phi * (rhob + rhoa) / 2;
rhoPhi == phi / 2 * (rhob - rhoa) + phi * (rhob + rhoa) / 2; // TODO is phi really what Holger means with CEqn.Flux()?

Info << "max(phi): " << max(phi) << endl;
Info << "min(phi): " << min(phi) << endl;

Info << "max(mu): " << max(mu) << endl;
Info << "min(mu): " << min(mu) << endl;
