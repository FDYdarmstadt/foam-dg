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

#include "dgSchemes.H"
#include "objectRegistry.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

Foam::debug::debugSwitch
Foam::dgSchemes::debug
(
    "dgSchemes",
    false
);

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

void Foam::dgSchemes::clear()
{
    dgDdtSchemes_.clear();
    defaultDgDdtScheme_.clear();
//    d2dt2Schemes_.clear();
//    defaultD2dt2Scheme_.clear();
//    interpolationSchemes_.clear();
//    defaultInterpolationScheme_.clear();
    dgDivSchemes_.clear(); // optional
    defaultDgDivScheme_.clear();
//    gradSchemes_.clear(); // optional
//    defaultGradScheme_.clear();
//    snGradSchemes_.clear();
//    defaultSnGradScheme_.clear();
    dgLaplacianSchemes_.clear(); // optional
    defaultDgLaplacianScheme_.clear();
    dgCahnHilliardSchemes_.clear(); // optional
    defaultDgCahnHilliardScheme_.clear();

    // Set up special handling for fluxRequired: do not clear
    // HJ, 25/Jul/2016
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dgSchemes::dgSchemes(const objectRegistry& obr)
:
    IOdictionary
    (
        IOobject
        (
            "dgSchemes",
            obr.time().system(),
            obr,
            IOobject::READ_IF_PRESENT_IF_MODIFIED,  // Allow default dictionary creation
            IOobject::NO_WRITE
        )
    ),
    dgDdtSchemes_
    (
        ITstream
        (
            objectPath() + "::dgDdtSchemes",
            tokenList()
        )()
    ),
    defaultDgDdtScheme_
    (
        dgDdtSchemes_.name() + "::default",
        tokenList()
    ),
//    d2dt2Schemes_
//    (
//        ITstream
//        (
//            objectPath() + "::d2dt2Schemes",
//            tokenList()
//        )()
//    ),
//    defaultD2dt2Scheme_
//    (
//        d2dt2Schemes_.name() + "::default",
//        tokenList()
//    ),
//    interpolationSchemes_
//    (
//        ITstream
//        (
//            objectPath() + "::interpolationSchemes",
//            tokenList()
//        )()
//    ),
//    defaultInterpolationScheme_
//    (
//        interpolationSchemes_.name() + "::default",
//        tokenList()
//    ),
    dgDivSchemes_
    (
        ITstream
        (
            objectPath() + "::dgDivSchemes",
            tokenList()
        )()
    ),
    defaultDgDivScheme_
    (
        dgDivSchemes_.name() + "::default",
        tokenList()
    ),
//    gradSchemes_
//    (
//        ITstream
//        (
//            objectPath() + "::gradSchemes",
//            tokenList()
//        )()
//    ),
//    defaultGradScheme_
//    (
//        gradSchemes_.name() + "::default",
//        tokenList()
//    ),
//    snGradSchemes_
//    (
//        ITstream
//        (
//            objectPath() + "::snGradSchemes",
//            tokenList()
//        )()
//    ),
//    defaultSnGradScheme_
//    (
//        snGradSchemes_.name() + "::default",
//        tokenList()
//    ),
    dgLaplacianSchemes_
    (
        ITstream
        (
            objectPath() + "::dgLaplacianSchemes",
            tokenList()
        )()
    ),
    defaultDgLaplacianScheme_
    (
        dgLaplacianSchemes_.name() + "::default",
        tokenList()
    ),
    dgCahnHilliardSchemes_
    (
        ITstream
        (
            objectPath() + "::dgCahnHilliardSchemes",
            tokenList()
        )()
    ),
    defaultDgCahnHilliardScheme_
    (
        dgCahnHilliardSchemes_.name() + "::default",
        tokenList()
    ),
    fluxRequired_(),  // Do not read flux required option
    defaultFluxRequired_(false)
{
    if (!headerOk())
    {
        if (debug)
        {
            InfoIn
            (
                "dgSchemes::dgSchemes(const objectRegistry& obr)"
            )   << "dgSchemes dictionary not found.  Creating default."
                << endl;
        }

        regIOobject::write();
    }

    read();
}

//     dgCahnHilliardSchemes_
//     (
//         ITstream
//         (
//             objectPath() + "::dgCahnHilliardSchemes",
//             tokenList()
//         )()
//     ),
//     defaultDgCahnHilliardScheme_
//     (
//         dgCahnHilliardSchemes_.name() + "::default",
//         tokenList()
//     ),
//     fluxRequired_(),  // Do not read flux required option
//     defaultFluxRequired_(false)
// {
//     if (!headerOk())
//     {
//         if (debug)
//         {
//             InfoIn
//             (
//                 "dgSchemes::dgSchemes(const objectRegistry& obr)"
//             )   << "dgSchemes dictionary not found.  Creating default."
//                 << endl;
//         }

//         regIOobject::write();
//     }

//     read();
// }

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::dgSchemes::read()
{
    if (regIOobject::read())
    {
        const dictionary& dict = schemesDict();

        // Persistent settings across reads is incorrect
        clear();

        if (dict.found("dgDdtSchemes"))
        {
            dgDdtSchemes_ = dict.subDict("dgDdtSchemes");
        }
        else if (dict.found("timeScheme"))
        {
            // For backward compatibility.
            // The timeScheme will be deprecated with warning or removed
            WarningIn("dgSchemes::read()")
                << "Using deprecated 'timeScheme' instead of 'dgDdtSchemes'"
                << nl << endl;

            word schemeName(dict.lookup("timeScheme"));

            if (schemeName == "EulerImplicit")
            {
                schemeName = "Euler";
            }
            else if (schemeName == "BackwardDifferencing")
            {
                schemeName = "backward";
            }
            else if (schemeName == "SteadyState")
            {
                schemeName = "steadyState";
            }
            else
            {
                FatalIOErrorIn("dgSchemes::read()", dict.lookup("timeScheme"))
                    << "\n    Only EulerImplicit, BackwardDifferencing and "
                       "SteadyState\n    are supported by the old timeScheme "
                       "specification.\n    Please use dgDdtSchemes instead."
                    << exit(FatalIOError);
            }

            dgDdtSchemes_.set("default", schemeName);

            dgDdtSchemes_.lookup("default")[0].lineNumber() =
                dict.lookup("timeScheme").lineNumber();
        }
        else
        {
            dgDdtSchemes_.set("default", "none");
        }

        if
        (
            dgDdtSchemes_.found("default")
         && word(dgDdtSchemes_.lookup("default")) != "none"
        )
        {
            defaultDgDdtScheme_ = dgDdtSchemes_.lookup("default");
        }
//
//
//        if (dict.found("d2dt2Schemes"))
//        {
//            d2dt2Schemes_ = dict.subDict("d2dt2Schemes");
//        }
//        else if (dict.found("timeScheme"))
//        {
//            // For backward compatibility.
//            // The timeScheme will be deprecated with warning or removed
//            WarningIn("dgSchemes::read()")
//                << "Using deprecated 'timeScheme' instead of 'd2dt2Schemes'"
//                << nl << endl;
//
//            word schemeName(dict.lookup("timeScheme"));
//
//            if (schemeName == "EulerImplicit")
//            {
//                schemeName = "Euler";
//            }
//            else if (schemeName == "SteadyState")
//            {
//                schemeName = "steadyState";
//            }
//
//            d2dt2Schemes_.set("default", schemeName);
//
//            d2dt2Schemes_.lookup("default")[0].lineNumber() =
//                dict.lookup("timeScheme").lineNumber();
//        }
//        else
//        {
//            d2dt2Schemes_.set("default", "none");
//        }
//
//        if
//        (
//            d2dt2Schemes_.found("default")
//         && word(d2dt2Schemes_.lookup("default")) != "none"
//        )
//        {
//            defaultD2dt2Scheme_ = d2dt2Schemes_.lookup("default");
//        }
//
//
//        if (dict.found("interpolationSchemes"))
//        {
//            interpolationSchemes_ = dict.subDict("interpolationSchemes");
//        }
//        else if (!interpolationSchemes_.found("default"))
//        {
//            interpolationSchemes_.add("default", "linear");
//        }
//
//        if
//        (
//            interpolationSchemes_.found("default")
//         && word(interpolationSchemes_.lookup("default")) != "none"
//        )
//        {
//            defaultInterpolationScheme_ =
//                interpolationSchemes_.lookup("default");
//        }
//
//
        if (dict.found("dgDivSchemes"))
        {
            dgDivSchemes_ = dict.subDict("dgDivSchemes");
        }

        if
        (
            dgDivSchemes_.found("default")
         && word(dgDivSchemes_.lookup("default")) != "none"
        )
        {
            defaultDgDivScheme_ = dgDivSchemes_.lookup("default");
        }
//
//        if (dict.found("gradSchemes"))
//        {
//            gradSchemes_ = dict.subDict("gradSchemes");
//        }
//
//        if
//        (
//            gradSchemes_.found("default")
//         && word(gradSchemes_.lookup("default")) != "none"
//        )
//        {
//            defaultGradScheme_ = gradSchemes_.lookup("default");
//        }
//
//
//        if (dict.found("snGradSchemes"))
//        {
//            snGradSchemes_ = dict.subDict("snGradSchemes");
//        }
//        else if (!snGradSchemes_.found("default"))
//        {
//            snGradSchemes_.add("default", "corrected");
//        }
//
//        if
//        (
//            snGradSchemes_.found("default")
//         && word(snGradSchemes_.lookup("default")) != "none"
//        )
//        {
//            defaultSnGradScheme_ = snGradSchemes_.lookup("default");
//        }
//
//
        if (dict.found("dgLaplacianSchemes"))
        {
            dgLaplacianSchemes_ = dict.subDict("dgLaplacianSchemes");
        }

        if
        (
            dgLaplacianSchemes_.found("default")
         && word(dgLaplacianSchemes_.lookup("default")) != "none"
        )
        {
            defaultDgLaplacianScheme_ = dgLaplacianSchemes_.lookup("default");
        }

        if (dict.found("dgCahnHilliardSchemes"))
        {
            dgCahnHilliardSchemes_ = dict.subDict("dgCahnHilliardSchemes");
        }

        if
        (
            dgCahnHilliardSchemes_.found("default")
         && word(dgCahnHilliardSchemes_.lookup("default")) != "none"
        )
        {
            defaultDgCahnHilliardScheme_ = dgCahnHilliardSchemes_.lookup("default");
        }
//
//
//        // if (dict.found("fluxRequired"))
//        // {
//        //     fluxRequired_ = dict.subDict("fluxRequired");
//
//        //     if
//        //     (
//        //         fluxRequired_.found("default")
//        //      && word(fluxRequired_.lookup("default")) != "none"
//        //     )
//        //     {
//        //         defaultFluxRequired_ = Switch(fluxRequired_.lookup("default"));
//        //     }
//        // }

        return true;
    }
    else
    {
        return false;
    }
}


const Foam::dictionary& Foam::dgSchemes::schemesDict() const
{
    if (found("select"))
    {
        return subDict(word(lookup("select")));
    }
    else
    {
        return *this;
    }
}


Foam::ITstream& Foam::dgSchemes::dgDdtScheme(const word& name) const
{
    if (debug)
    {
        Info<< "Lookup ddtScheme for " << name << endl;
    }

    if (dgDdtSchemes_.found(name) || defaultDgDdtScheme_.empty())
    {
        return dgDdtSchemes_.lookup(name);
    }
    else
    {
        const_cast<ITstream&>(defaultDgDdtScheme_).rewind();
        return const_cast<ITstream&>(defaultDgDdtScheme_);
    }
}
//
//
//Foam::ITstream& Foam::dgSchemes::d2dt2Scheme(const word& name) const
//{
//    if (debug)
//    {
//        Info<< "Lookup d2dt2Scheme for " << name << endl;
//    }
//
//    if (d2dt2Schemes_.found(name) || defaultD2dt2Scheme_.empty())
//    {
//        return d2dt2Schemes_.lookup(name);
//    }
//    else
//    {
//        const_cast<ITstream&>(defaultD2dt2Scheme_).rewind();
//        return const_cast<ITstream&>(defaultD2dt2Scheme_);
//    }
//}
//
//
//Foam::ITstream& Foam::dgSchemes::interpolationScheme(const word& name) const
//{
//    if (debug)
//    {
//        Info<< "Lookup interpolationScheme for " << name << endl;
//    }
//
//    if
//    (
//        interpolationSchemes_.found(name)
//     || defaultInterpolationScheme_.empty()
//    )
//    {
//        return interpolationSchemes_.lookup(name);
//    }
//    else
//    {
//        const_cast<ITstream&>(defaultInterpolationScheme_).rewind();
//        return const_cast<ITstream&>(defaultInterpolationScheme_);
//    }
//}
//
//
Foam::ITstream& Foam::dgSchemes::dgDivScheme(const word& name) const
{
    if (debug)
    {
        Info<< "Lookup divScheme for " << name << endl;
    }

    if (dgDivSchemes_.found(name) || defaultDgDivScheme_.empty())
    {
        return dgDivSchemes_.lookup(name);
    }
    else
    {
        const_cast<ITstream&>(defaultDgDivScheme_).rewind();
        return const_cast<ITstream&>(defaultDgDivScheme_);
    }
}
//
//
//Foam::ITstream& Foam::dgSchemes::gradScheme(const word& name) const
//{
//    if (debug)
//    {
//        Info<< "Lookup gradScheme for " << name << endl;
//    }
//
//    if (gradSchemes_.found(name) || defaultGradScheme_.empty())
//    {
//        return gradSchemes_.lookup(name);
//    }
//    else
//    {
//        const_cast<ITstream&>(defaultGradScheme_).rewind();
//        return const_cast<ITstream&>(defaultGradScheme_);
//    }
//}
//
//
//Foam::ITstream& Foam::dgSchemes::snGradScheme(const word& name) const
//{
//    if (debug)
//    {
//        Info<< "Lookup snGradScheme for " << name << endl;
//    }
//
//    if (snGradSchemes_.found(name) || defaultSnGradScheme_.empty())
//    {
//        return snGradSchemes_.lookup(name);
//    }
//    else
//    {
//        const_cast<ITstream&>(defaultSnGradScheme_).rewind();
//        return const_cast<ITstream&>(defaultSnGradScheme_);
//    }
//}


Foam::ITstream& Foam::dgSchemes::dgLaplacianScheme(const word& name) const
{
    if (debug)
    {
        Info<< "Lookup dgLaplacianScheme for " << name << endl;
    }

    if (dgLaplacianSchemes_.found(name) || defaultDgLaplacianScheme_.empty())
    {
        return dgLaplacianSchemes_.lookup(name);
    }
    else
    {
        const_cast<ITstream&>(defaultDgLaplacianScheme_).rewind();
        return const_cast<ITstream&>(defaultDgLaplacianScheme_);
    }
}

// Foam::ITstream& Foam::dgSchemes::dgCahnHilliardScheme(const word& name) const
// {
//     if (debug)
//     {
//         Info<< "Lookup dgCahnHilliardScheme for " << name << endl;
//     }

//     if (dgCahnHilliardSchemes_.found(name) || defaultDgCahnHilliardScheme_.empty())
//     {
//         return dgCahnHilliardSchemes_.lookup(name);
//     }
//     else
//     {
//         const_cast<ITstream&>(defaultDgCahnHilliardScheme_).rewind();
//         return const_cast<ITstream&>(defaultDgCahnHilliardScheme_);
//     }
// }


void Foam::dgSchemes::setFluxRequired(const word& name) const
{
    if (debug)
    {
        Info<< "Setting fluxRequired for " << name << endl;
    }

    fluxRequired_.add(name, true, true);
}


bool Foam::dgSchemes::fluxRequired(const word& name) const
{
    if (debug)
    {
        Info<< "Lookup fluxRequired for " << name << endl;
    }

    if (fluxRequired_.found(name))
    {
        return true;
    }
    else
    {
        return defaultFluxRequired_;
    }
}

bool Foam::dgSchemes::writeData(Ostream& os) const
{
    // Write dictionaries
    os << nl << "dgDdtSchemes";
    dgDdtSchemes_.write(os, true);
//
//    os << nl << "d2dt2Schemes";
//    d2dt2Schemes_.write(os, true);
//
//    os << nl << "interpolationSchemes";
//    interpolationSchemes_.write(os, true);
//
    os << nl << "dgDivSchemes";
    dgDivSchemes_.write(os, true);
//
//    os << nl << "gradSchemes";
//    gradSchemes_.write(os, true);
//
//    os << nl << "snGradSchemes";
//    snGradSchemes_.write(os, true);

    os << nl << "dgLaplacianSchemes";
    dgLaplacianSchemes_.write(os, true);

    os << nl << "dgCahnHilliardSchemes";
    dgCahnHilliardSchemes_.write(os, true);


    os << nl << "fluxRequired";
    fluxRequired_.write(os, true);

    return true;
}


// ************************************************************************* //
