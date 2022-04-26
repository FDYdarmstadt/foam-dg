    Info<< "Reading transportProperties\n" << endl;

    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );

    Info<< "Reading field psi\n" << endl;

    volScalarField psi
    (
        IOobject
        (
            "psi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field C\n" << endl;

    volScalarField C
    (
        IOobject
        (
            "C",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading coefficient fa\n" << endl;

    dimensionedScalar fa
    (
        transportProperties.lookup("fa")
    );

    Info<< "Reading coefficient fb\n" << endl;

    dimensionedScalar fb
    (
        transportProperties.lookup("fb")
    );

    Info<< "Reading coefficient epsilon\n" << endl;

    dimensionedScalar epsilon
    (
        transportProperties.lookup("epsilon")
    );

    dimensionedScalar epsSqr(sqr(epsilon));

    Info<< "Reading coefficient M\n" << endl;

    dimensionedScalar M
    (
        transportProperties.lookup("M")
    );

    dictionary chDict(mesh.solutionDict().subDict("ChanHilliardSolution"));
    label nCCorr
    (
        readLabel(chDict.lookup("nCCorr"))
    );

    scalar psiTol
    (
        readScalar(chDict.lookup("psiTol"))
    );

    scalar CTol
    (
        readScalar(chDict.lookup("CTol"))
    );

    Switch mixedCH
    (
        chDict.lookup("mixedFormulation")
    );

    volScalarField dfdcCoeffs(fa*sqr(C));
    volScalarField dfdcSource(-fb*C);

    //- Update potential
    psi = (dfdcCoeffs*C + dfdcSource) - fvc::laplacian(epsSqr,C);

    // Working coupled solution field
    Info<< "Creating field blockCH\n" << endl;
    volVector2Field blockCH
    (
        IOobject
        (
            "blockCH",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedVector2("zero", dimless, vector2::zero)
    );

    scalar vol(gSum(mesh.V()));
    scalar avgFreeEnergy
    (
        (
            fvc::domainIntegrate
            (
                (fa/4.0)*sqr(sqr(C) -fb/fa)
                + epsSqr/2.0*magSqr(fvc::grad(C))
            )/vol
        ).value()
    );
    scalar avgMass((fvc::domainIntegrate(C)/vol).value());
