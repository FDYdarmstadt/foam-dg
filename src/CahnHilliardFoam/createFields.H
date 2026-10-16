#include "linear.H"

    Info<< "Reading FV field p\n" << endl;
    volScalarField pd
    (
        IOobject
        (
            "pd",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    // Order paremeter C
    Info<< "Reading FV field C\n" << endl;
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

    // Chemical potential Psi
    Info<< "Reading FV field Psi\n" << endl;
    volScalarField Psi
    (
        IOobject
        (
            "Psi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading FV field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    // Order parameter c
    // The DG counterparts (c, u, psi) below are the fields actually passed to
    // BoSSS; the plain FV fields above (C, U, Psi) are kept in sync with them
    // via importFromVolVecField()/SyncFromBoSSS(), see CEqn.H.
    Info<< "Reading DG field c\n" << endl;

    cellScalarField c
    (
        IOobject
        (
            "c",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh,
        C,
        "c" // Name of the DG field in BoSSS
    );

    Info<< "Reading DG field u\n" << endl;

    cellVectorField u
    (
        IOobject
        (
            "u",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh,
        U
    );

    // Chemical potential psi
    Info<< "Reading DG field psi\n" << endl;

    cellScalarField psi
    (
        IOobject
        (
            "psi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh,
        Psi
    );


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

    dimensionedScalar rhoa
    (
        transportProperties.lookup("rhoa")
    );
    dimensionedScalar rhob
    (
        transportProperties.lookup("rhob")
    );

    dimensionedScalar nua
    (
        transportProperties.lookup("nua")
    );
    dimensionedScalar nub
    (
        transportProperties.lookup("nub")
    );
    dimensionedScalar sigma
    (
        transportProperties.lookup("sigma")
    );
    dimensionedScalar Cn
    (
        transportProperties.lookup("Cn")
    );
    dimensionedScalar D
    (
        transportProperties.lookup("D")
    );

    // Face flux; u is the DG velocity field synced from BoSSS each time step
    // (see CEqn.H), so phi is reconstructed from it rather than from U.
    surfaceScalarField phi
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(u.volVecField()) & mesh.Sf()
    );

    // Mixture density
    // Need to store rho for ddt(rho, U)
    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT
        ),
        (scalar(1) + C)/2.*rhoa + (scalar(1) - C)/2.*rhob,
        C.boundaryField().types()
    );
    rho.oldTime();

    // Mixture viscosity
    volScalarField mu
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT
        ),
        (scalar(1) + C)/2.*rhoa*nua + (scalar(1) - C)/2.*rhob*nub,
        C.boundaryField().types()
    );

    // Mass flux
    // Initialisation does not matter because rhoPhi is reset after the
    // alpha1 solution before it is used in the U equation.
    surfaceScalarField rhoPhi
    (
        IOobject
        (
            "rho*phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rhoa*phi
    );


    Info<< "Calculating field g.h\n" << endl;
    volScalarField gh("gh", g & mesh.C());
    surfaceScalarField ghf("gh", g & mesh.Cf());

    label pdRefCell = 0;
    scalar pdRefValue = 0.0;
    setRefCell(pd, pimple.dict(), pdRefCell, pdRefValue);
    mesh.schemesDict().setFluxRequired(pd.name());

    scalar pRefValue = 0.0;



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

    scalar CTol
    (
        readScalar(transportProperties.lookup("CTol"))
    );

    scalar UTol
    (
        readScalar(transportProperties.lookup("UTol"))
    );
