
    Foam::fvMesh fvMesh
    (
        Foam::IOobject
        (
            Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            Foam::IOobject::MUST_READ
        )
    );


    const dgBase& polynomials = dgMesh.polynomials();

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

    forAll (fvMesh.C(), cellI)
    {
        vector coord (0, 0, 0);

        scalarField polyEval = polynomials.evaluate(coord);

        forAll (polyEval, modI)
        {
            Tvol[cellI] += T[cellI][modI]*polyEval[modI];
//                          *dgMesh.cellScaleCoeffs()[cellI];
        }
    }

    Info<< nl << "- - - - - - - - - - - -" << nl << nl
        << "Tvol: " << Tvol.internalField() << nl << endl;


    Info<< "Test harness successfully finished." << endl;

