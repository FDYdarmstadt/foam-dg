/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.0
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

Namespace
    Foam::dgc

Description
    Namespace of functions to calculate explicit derivatives.

\*---------------------------------------------------------------------------*/

#ifndef dgc_H
#define dgc_H

#include "dg.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#include "surfaceInterpolate.H"
#include "dgcVolumeIntegrate.H"
//#include "fvcSurfaceIntegrate.H"
//#include "fvcAverage.H"
//#include "fvcReconstruct.H"
//#include "fvcDdt.H"
//#include "fvcDDtTotal.H"
//#include "fvcD2dt2.H"
//#include "fvcDiv.H"
//#include "fvcFlux.H"
//#include "fvcGrad.H"
//#include "fvcMagSqrGradGrad.H"
//#include "fvcSnGrad.H"
//#include "fvcCurl.H"
#include "dgcLaplacian.H"
//#include "fvcSup.H"
//#include "fvcMeshPhi.H"
//
//#include "fvcAdjDiv.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
