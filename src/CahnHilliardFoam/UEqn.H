    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(rhoPhi, U)
      - fvm::laplacian(mu, U)
      - (fvc::grad(U) & fvc::grad(mu))
    //- fvc::div(mu*(fvc::interpolate(dev(fvc::grad(U))) & mesh.Sf()))
    );

    UEqn.relax();

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                  fvc::interpolate(Psi)*fvc::snGrad(C)
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(pd)
                )*dgMesh.finVolMesh()->magSf()
            )
        );
    }
