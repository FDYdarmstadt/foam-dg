//- Check convergence
{
    //- Residuals on psi
    // scalar resPsi
    // (
    //     fvc::domainIntegrate(mag(psi-psi.prevIter())).value()
    //     /fvc::domainIntegrate(mag(psi)).value()
    // );

    //- Residuals on C
    scalar resC
    (
        fvc::domainIntegrate(mag(C-C.prevIter())).value()
        /fvc::domainIntegrate(mag(C)).value()
    );

    scalar resU
    (
        fvc::domainIntegrate(mag(U-U.prevIter())).value()
        /fvc::domainIntegrate(mag(U)).value()
    );

    Info<< "Residuals on U: " << resU
        << "    Residuals on C: " << resC;

    if(resU < UTol && resC < CTol )
    {
        converged = true;
        Info<<"\nCahn-Hilliard corrector converged in "
            << cCorr << " iterations" << endl;
    }
    else if(cCorr == nCCorr)
    {
        converged = true;
        Info<<"\nCahn-Hilliard corrector NOT converged in "
            << nCCorr << " iterations" << endl;
    }

    Info<<endl;

}
