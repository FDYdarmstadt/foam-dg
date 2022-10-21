#include "linear.H"

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

    // Info<< "Reading field psi\n" << endl;

    // volScalarField psi
    // (
    //     IOobject
    //     (
    //         "psi",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::MUST_READ,
    //         IOobject::AUTO_WRITE
    //     ),
    //     mesh
    // );


    Info<< "Reading field C\n" << endl;

    cellScalarField C
    (
        IOobject
        (
            "C",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh
    );

    Info<< "Reading field Phi\n" << endl;

    cellScalarField Phi
    (
        IOobject
        (
            "Phi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh
    );

    Info<< "Reading field U\n" << endl;

    cellVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh
    );


    // fvMesh myMesh(
    //     IOobject
    //     (
    //         "myMesh",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::NO_READ,
    //         IOobject::NO_WRITE
    //     )
    // );

    // volVectorField fvU
    // (
    //     IOobject
    //     (
    //         "fvU",
    //         runTime.timeName(),
    //         // myMesh,
    //         *dgMesh.finVolMesh(),
    //         IOobject::MUST_READ,
    //         IOobject::NO_WRITE
    //     ),
    //     *dgMesh.finVolMesh()
    // );

    surfaceScalarField Flux
    (
        IOobject
        (
            "Flux",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        // linearInterpolate(fvU) & dgMesh.finVolMesh()->Sf()
        linearInterpolate(U.volVecField()) & dgMesh.finVolMesh()->Sf()
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

    // dictionary chDict(mesh.solutionDict().subDict("CahnHilliardSolution"));
    // label nCCorr
    // (
    //     readLabel(chDict.lookup("nCCorr"))
    // );

    // scalar psiTol
    // (
    //     readScalar(chDict.lookup("psiTol"))
    // );

    scalar CTol
    (
        readScalar(transportProperties.lookup("CTol"))
    );

    scalar UTol
    (
        readScalar(transportProperties.lookup("UTol"))
    );

    C.importFromVolScaField();
    U.importFromVolVecField();

    // Switch mixedCH
    // (
    //     transportProperties.lookup("mixedFormulation")
    // );

    // cellScalarField dfdcCoeffs(fa*sqr(C));
    // cellScalarField dfdcSource(-fb*C);

    //- Update potential
    // psi = (dfdcCoeffs*C + dfdcSource) - fvc::laplacian(epsSqr,C);

    // Working coupled solution field
    // Info<< "Creating field blockCH\n" << endl;
    // volVector2Field blockCH
    // (
    //     IOobject
    //     (
    //         "blockCH",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::NO_READ,
    //         IOobject::NO_WRITE
    //     ),
    //     mesh,
    //     dimensionedVector2("zero", dimless, vector2::zero)
    // );

    // scalar vol(gSum(mesh.V()));
    // scalar avgFreeEnergy
    // (
    //     (
    //         fvc::domainIntegrate
    //         (
    //             (fa/4.0)*sqr(sqr(C) -fb/fa)
    //             + epsSqr/2.0*magSqr(fvc::grad(C))
    //         )/vol
    //     ).value()
    // );
    // scalar avgMass((fvc::domainIntegrate(C)/vol).value());
