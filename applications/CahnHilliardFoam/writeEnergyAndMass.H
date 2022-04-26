{

    avgMass = (fvc::domainIntegrate(C)/vol).value();

    Info<< "Average free Energy : "<< avgFreeEnergy << endl;

    Info<< "Average mass : "<< avgMass << endl;
}
