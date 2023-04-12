{
    volScalarField rUA = 1.0/UEqn.A();
    surfaceScalarField rUAf = fvc::interpolate(rUA);

    U = rUA*UEqn.H();

    surfaceScalarField phiU
    (
        "phiU",
        (fvc::interpolate(U) & dgMesh.finVolMesh()->Sf())
      + fvc::ddtPhiCorr(rUA, rho, U, phi)
    );

    adjustPhi(phiU, U, pd);

    phi = phiU +
        (
            // Interfacial energy density
            fvc::interpolate(Psi)*fvc::snGrad(C)
          - ghf*fvc::snGrad(rho)
        )*rUAf*dgMesh.finVolMesh()->magSf();


    while (pimple.correctNonOrthogonal())
    {
        fvScalarMatrix pdEqn
        (
            fvm::laplacian(rUAf, pd) == fvc::div(phi)
        );

        pdEqn.setReference(pdRefCell, pdRefValue);

        pdEqn.solve
        (
            dgMesh.finVolMesh()->solutionDict().solver(pd.select(pimple.finalInnerIter()))
        );

        if (pimple.finalNonOrthogonalIter())
        {
            phi -= pdEqn.flux();
        }
    }

    U += rUA*fvc::reconstruct((phi - phiU)/rUAf);
    U.correctBoundaryConditions();
}
