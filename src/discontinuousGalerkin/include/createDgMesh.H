//
// createMesh.H
// ~~~~~~~~~~~~

    Foam::Info
        << "Create DG mesh for time = "
        << runTime.timeName() << Foam::nl << Foam::endl;

    Foam::dgMesh dgMesh(mesh);
