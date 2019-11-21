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

\*---------------------------------------------------------------------------*/

#include "dg.H"
#include "HashTable.H"
//#include "linear.H"
#include "dgMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace dg
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<dgLaplacianScheme<Type, GType> > dgLaplacianScheme<Type, GType>::New
(
    const dgMesh& mesh,
    Istream& schemeData
)
{
    if (dg::debug)
    {
        Info<< "dgLaplacianScheme<Type, GType>::New(const dgMesh&, Istream&) : "
               "constructing dgLaplacianScheme<Type, GType>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorIn
        (
            "dgLaplacianScheme<Type, GType>::New(const dgMesh&, Istream&)",
            schemeData
        )   << "Laplacian scheme not specified" << nl << nl
            << "Valid dgLaplacian schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorIn
        (
            "dgLaplacianScheme<Type, GType>::New(const dgMesh&, Istream&)",
            schemeData
        )   << "Unknown dgLaplacian scheme " << schemeName << nl << nl
            << "Valid dgLaplacian schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type, class GType>
dgLaplacianScheme<Type, GType>::~dgLaplacianScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template<class Type, class GType>
//tmp<dgMatrix<Type> >
//dgLaplacianScheme<Type, GType>::dgmLaplacian
//(
//    const GeometricField<GType, dgPatchField, cellMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgmLaplacian(vf);//tinterpGammaScheme_().interpolate(gamma)(), vf);
//}
//
//
//template<class Type, class GType>
//tmp<GeometricField<Type, dgPatchField, cellMesh> >
//dgLaplacianScheme<Type, GType>::dgcLaplacian
//(
//    const GeometricField<GType, dgPatchField, cellMesh>& gamma,
//    const GeometricField<Type, dgPatchField, cellMesh>& vf
//)
//{
//    return dgcLaplacian(vf);//tinterpGammaScheme_().interpolate(gamma)(), vf);
//}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace dg

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
