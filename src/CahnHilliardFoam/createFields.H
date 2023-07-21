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
        *dgMesh.finVolMesh()
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
        // mesh
        *dgMesh.finVolMesh()
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
        // mesh
        *dgMesh.finVolMesh()
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
        // mesh
        *dgMesh.finVolMesh()
    );

    // Order parameter c
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
        &C
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
        &U
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
        &Psi
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

    // Need to store rho for ddt(rho, U)
    // volScalarField rho
    // (
    //     IOobject
    //     (
    //         "rho",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::READ_IF_PRESENT
    //     ),
    //     // C*rhoa + (scalar(1) - C)*rhob,
    //     // C.volScaField()*rhoa, // TODO
    //     // C.volScaField().boundaryField().types()
    //     *dgMesh.finVolMesh()
    // );

    // rho.oldTime();

    // surfaceScalarField rhoPhi
    // (
    //     IOobject
    //     (
    //         "rho*phi",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::NO_READ,
    //         IOobject::NO_WRITE
    //     ),
    //     // rhoa*Phi.volScaField()
    //     linearInterpolate(U.volVecField()) & dgMesh.finVolMesh()->Sf()
    // );



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

    // if (runTime.timeName() == "0")
    // {
    //   Info << "Initializing DG fields from FV fields" << endl;
    //   c.importFromVolScaField();
    //   u.importFromVolVecField();
    // }

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
        linearInterpolate(u.volVecField()) & dgMesh.finVolMesh()->Sf()
        // linearInterpolate(U) & dgMesh.finVolMesh()->Sf()
    );

    // Info << "phi[iF]" << endl;
    // int nFaces = dgMesh.finVolMesh()->nInternalFaces();
    // for (int iF = 0; iF < nFaces; iF++){
    //     Info << phi[iF] << endl;
    // }

    // surfaceVectorField phiUOnly
    // (
    //     IOobject
    //     (
    //         "phi",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::READ_IF_PRESENT,
    //         IOobject::AUTO_WRITE
    //     ),
    //     linearInterpolate(u.volVecField())
    // );

    // Info << "phiUOnly[iF]" << endl;
    // for (int iF = 0; iF < nFaces; iF++){
    //     Info << phiUOnly[iF] << endl;
    // }

    // surfaceVectorField phiSurfaceMeshOnly
    // (
    //     IOobject
    //     (
    //         "phi",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::READ_IF_PRESENT,
    //         IOobject::AUTO_WRITE
    //     ),
    //     dgMesh.finVolMesh()->Sf()
    // );

    // Info << "phiSurfaceMeshOnly[iF]" << endl;
    // for (int iF = 0; iF < nFaces; iF++){
    //     Info << phiSurfaceMeshOnly[iF] << endl;
    // }
    // Info << "Done" << endl;

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
    volScalarField gh("gh", g & dgMesh.finVolMesh()->C());
    surfaceScalarField ghf("gh", g & dgMesh.finVolMesh()->Cf());

    label pdRefCell = 0;
    scalar pdRefValue = 0.0;
    setRefCell(pd, pimple.dict(), pdRefCell, pdRefValue);
    dgMesh.finVolMesh()->schemesDict().setFluxRequired(pd.name());

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
