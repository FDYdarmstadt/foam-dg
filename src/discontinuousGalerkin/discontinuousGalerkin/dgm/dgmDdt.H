/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

InNamespace
    Foam::dgm

Description
    Calulate the matrix for the first temporal derivative.

SourceFiles
    dgmDdt.C

\*---------------------------------------------------------------------------*/

#ifndef dgmDdt_H
#define dgmDdt_H

#include "cellFieldsFwd.H"
#include "dgMatrices.H"
#include "geometricOneField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace dgm functions Declaration
\*---------------------------------------------------------------------------*/

namespace dgm
{
    template<class Type>
    tmp<dgMatrix<Type> > dgDdt
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word& name
    );

    template<class Type>
    tmp<dgMatrix<Type> > dgDdt
    (
        const geometricOneField&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&,
        const word& name
    );

//    template<class Type>
//    tmp<dgMatrix<Type> > dgDdt
//    (
//        const dimensionedScalar&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word& name
//    );
//
//    template<class Type>
//    tmp<dgMatrix<Type> > dgDdt
//    (
//        const volScalarField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&,
//        const word& name
//    );

    template<class Type>
    tmp<dgMatrix<Type> > dgDdt
    (
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

    template<class Type>
    tmp<dgMatrix<Type> > dgDdt
    (
        const geometricOneField&,
        const DgGeometricField<Type, dgPatchField, cellMesh>&
    );

//    template<class Type>
//    tmp<dgMatrix<Type> > dgDdt
//    (
//        const dimensionedScalar&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
//
//    template<class Type>
//    tmp<dgMatrix<Type> > dgDdt
//    (
//        const volScalarField&,
//        const GeometricField<Type, dgPatchField, cellMesh>&
//    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dgmDdt.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
