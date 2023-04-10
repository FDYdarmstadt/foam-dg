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
        fvc::domainIntegrate(mag(c-c.prevIter())).value()
        /fvc::domainIntegrate(mag(c)).value()
    );

    scalar resU
    (
        fvc::domainIntegrate(mag(u-u.prevIter())).value()
        /fvc::domainIntegrate(mag(u)).value()
    );

    Info<< "Residuals on u: " << resU
        << "    Residuals on c: " << resC;

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
