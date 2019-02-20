    Info<< "Reading field T\n" << endl;


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
