label cCorr(0);
Switch converged(false);

do
{
    cCorr++;
    C.storePrevIter();
    psi.storePrevIter();

    if(mixedCH)
    {

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
        fvScalarMatrix CEqn
        (
            fvm::ddt(C)
            // - fvm::laplacian(epsSqr,C)
            // + fvm::Sp(dfdcCoeffs,C)
            // + dfdcSource
        );

        CEqn.relax();

        fvScalarMatrix psiEqn
        (
            fvm::Sp(1.0,psi)
            - dfdcSource

        );

        psiEqn.relax();

        // Prepare block system
        fvBlockMatrix<vector2> blockM(blockCH);

        // Insert equations into block Matrix
        blockM.insertEquation(0, CEqn);
        blockM.insertEquation(1, psiEqn);


        blockM.insertEquationCoupling
        (
            0,
            1,
            -fvm::laplacian(M,psi)
        );

        blockM.insertEquationCoupling
        (
            1,
            0,
             fvm::laplacian(epsSqr,C)
            - fvm::Sp(dfdcCoeffs,C)
        );

        //- Block coupled solver call
        blockM.solve();

        // Retrieve solution
        blockM.retrieveSolution(0, C.internalField());
        blockM.retrieveSolution(1, psi.internalField());
        C.correctBoundaryConditions();
        psi.correctBoundaryConditions();

        //- Update dfdc coeffs
        dfdcCoeffs =   fa*sqr(C);

        //- Update dfdc source
        dfdcSource =  -fb*C;

        //- Update potential
        psi = (dfdcCoeffs*C + dfdcSource) - fvc::laplacian(epsSqr,C);
        psi.correctBoundaryConditions();

        //- Compute new free energy
        scalar avgFreeEnergyNew
        (
            (
                fvc::domainIntegrate
                (
                    (fa/4.0)*sqr(sqr(C) -fb/fa)
                    + epsSqr/2.0*magSqr(fvc::grad(C))
                )/vol
            ).value()
        );

        scalar freeEnergyRes
        (
            mag(avgFreeEnergyNew-avgFreeEnergy)
            /(mag(avgFreeEnergy) + SMALL)
        );

        Info << "Residual on free energy : "<< freeEnergyRes << endl;

        avgFreeEnergy = avgFreeEnergyNew;
    }
    else
    {

        //- Create matrix for mu (positive definite)
        fvScalarMatrix psiEqn
        (
            - fvm::laplacian(M,psi)
        );

        //- Get diagonal (positive)
        volScalarField Apsi(psiEqn.A());

        //- Get off-diagonal terms
        volScalarField Hpsi(psiEqn.H());

        fvScalarMatrix CEqn
        (
            fvm::ddt(C)
          - fvm::laplacian(Apsi*epsSqr,C)
//          + (fvc::grad(Apsi*epsSqr)&fvc::grad(C))
          + fvm::Sp(Apsi*dfdcCoeffs,C)
        );

        CEqn.relax();

        //- Solve for C
        solve
        (
            CEqn
           ==
            Hpsi - Apsi*dfdcSource
        );

        //- Update dfdc coeffs
        dfdcCoeffs =   fa*sqr(C);

        //- Update dfdc source
        dfdcSource =  -fb*C;

        //- Update potential
        psi = (dfdcCoeffs*C + dfdcSource) - fvc::laplacian(epsSqr,C);
        psi.correctBoundaryConditions();

    }

# include "CHconvergence.H"

} while (!converged);
