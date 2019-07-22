    Info<< "Reading field T\n" << endl;

    cellScalarField T
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        dgMesh
    );

    volScalarField Tvol
    (
        IOobject
        (
            "Tvol",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        fvMesh,
        dimensionedScalar("Tvol", T.dimensions(), 0.0)
    );

//    Info<< "Reading transportProperties\n" << endl;
//
//    IOdictionary transportProperties
//    (
//        IOobject
//        (
//            "transportProperties",
//            runTime.constant(),
//            mesh,
//            IOobject::MUST_READ,
//            IOobject::NO_WRITE
//        )
//    );
//
//
//    Info<< "Reading diffusivity DT\n" << endl;
//
//    dimensionedScalar DT
////    dimensionedDgScalar DT
//    (
//        transportProperties.lookup("DT")
//    );
