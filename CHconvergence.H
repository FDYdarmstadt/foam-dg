//- Check convergence
{
    //- Residuals on psi
    scalar resPsi
    (
        fvc::domainIntegrate(mag(psi-psi.prevIter())).value()
        /fvc::domainIntegrate(mag(psi)).value()
    );

    //- Residuals on C
    scalar resC
    (
        fvc::domainIntegrate(mag(C-C.prevIter())).value()
        /fvc::domainIntegrate(mag(C)).value()
    );

    Info<< "Residuals on psi: " << resPsi
        << "    Residuals on C: " << resC;

    if(resPsi < psiTol && resC < CTol )
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
