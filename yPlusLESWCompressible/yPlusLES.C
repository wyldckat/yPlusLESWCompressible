/*---------------------------------------------------------------------------*\
=========                 |
\\      /  F ield         | Unsupported Contributions for OpenFOAM
 \\    /   O peration     |
  \\  /    A nd           | Copyright (C) 2013 Bruno Santos
   \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    yPlusLESWCompressible

Description
    Calculates and reports yPlus for all wall patches, for the specified times
    when using LES turbulence models.

    Default behaviour assumes operating in incompressible mode.
    Use the -compressible option for compressible LES cases.

Modifications
    2013-05-26 - Variant created by Bruno Santos (wyldckat@github).

    Code based on the original "yPlusLES" from OpenFOAM 2.2.x, as well as:
     - from "yPlusRAS", got the information for the basis of the changes.
     - from "src/postProcessing/functionObjects/utilities/yPlusLES" got how
       the compressible version of yPlusLES is calculated.
     - from "rhoPimpleFoam", got some ideas on how to set-up the compressible
       part.
       
    This file was backported from 2.2.x to 2.1.x.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

#include "incompressible/singlePhaseTransportModel/singlePhaseTransportModel.H"
#include "incompressible/LES/LESModel/LESModel.H"

#include "compressible/LES/LESModel/LESModel.H"

#include "nearWallDist.H"
#include "wallDist.H"
#include "wallFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void calcIncompressibleYPlus
(
    const fvMesh& mesh,
    const Time& runTime,
    const volVectorField& U,
    volScalarField& yPlus
)
{

    #include "createPhi.H"

    singlePhaseTransportModel laminarTransport(U, phi);

    autoPtr<incompressible::LESModel> sgsModel
    (
        incompressible::LESModel::New(U, phi, laminarTransport)
    );

    volScalarField::GeometricBoundaryField d = nearWallDist(mesh).y();
    volScalarField nuEff(sgsModel->nuEff());

    const fvPatchList& patches = mesh.boundary();

    const volScalarField nuLam(sgsModel->nu());

    bool foundNutPatch = false;
    forAll(patches, patchi)
    {
        const fvPatch& currPatch = patches[patchi];

        if (isA<wallFvPatch>(currPatch))
        {
            foundNutPatch = true;

            yPlus.boundaryField()[patchi] =
                d[patchi]
                *sqrt
                (
                    nuEff.boundaryField()[patchi]
                    *mag(U.boundaryField()[patchi].snGrad())
                )
                /nuLam.boundaryField()[patchi];

            const scalarField& Yp = yPlus.boundaryField()[patchi];

            Info<< "Patch " << patchi
                << " named " << currPatch.name()
                << " y+ : min: " << gMin(Yp) << " max: " << gMax(Yp)
                << " average: " << gAverage(Yp) << nl << endl;
        }
    }

    if (!foundNutPatch)
    {
        Info<< "    no " << wallFvPatch::typeName << " patches"
            << endl;
    }
}


void calcCompressibleYPlus
(
    const fvMesh& mesh,
    const Time& runTime,
    const volVectorField& U,
    volScalarField& yPlus
)
{
    autoPtr<basicThermo> pThermo
    (
        basicThermo::New(mesh)
    );
    basicThermo& thermo = pThermo();

    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        thermo.rho()
    );
    
    #include "compressibleCreatePhi.H"
    
    Info<< "Creating turbulence model\n" << endl;
    autoPtr<compressible::LESModel> sgsModel
    (
        compressible::LESModel::New
        (
            rho,
            U,
            phi,
            thermo
        )
    );

    volScalarField::GeometricBoundaryField d = nearWallDist(mesh).y();
    volScalarField muEff(sgsModel->muEff());

    const fvPatchList& patches = mesh.boundary();

    const volScalarField muLam(sgsModel->mu());

    bool foundNutPatch = false;
    forAll(patches, patchi)
    {
        const fvPatch& currPatch = patches[patchi];

        if (isA<wallFvPatch>(currPatch))
        {
            foundNutPatch = true;

            yPlus.boundaryField()[patchi] =
                d[patchi]
                *sqrt
                (
                    muEff.boundaryField()[patchi]
                    *mag(U.boundaryField()[patchi].snGrad())
                    *rho.boundaryField()[patchi]
                )
                /muLam.boundaryField()[patchi];

            const scalarField& Yp = yPlus.boundaryField()[patchi];

            Info<< "Patch " << patchi
                << " named " << currPatch.name()
                << " y+ : min: " << gMin(Yp) << " max: " << gMax(Yp)
                << " average: " << gAverage(Yp) << nl << endl;
        }
    }

    if (!foundNutPatch)
    {
        Info<< "    no " << wallFvPatch::typeName << " patches"
            << endl;
    }
}


int main(int argc, char *argv[])
{
    timeSelector::addOptions();

    #include "addRegionOption.H"

    argList::addBoolOption
    (
        "compressible",
        "calculate compressible y+"
    );

    #include "setRootCase.H"
    #include "createTime.H"
    instantList timeDirs = timeSelector::select0(runTime, args);
    #include "createNamedMesh.H"

    const bool compressible = args.optionFound("compressible");

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;
        fvMesh::readUpdateState state = mesh.readUpdate();

        // Wall distance
        if (timeI == 0 || state != fvMesh::UNCHANGED)
        {
            Info<< "Calculating wall distance\n" << endl;
            wallDist y(mesh, true);
            Info<< "Writing wall distance to field "
                << y.name() << nl << endl;
            y.write();
        }


        volScalarField yPlus
        (
            IOobject
            (
                "yPlus",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionedScalar("yPlus", dimless, 0.0)
        );

        IOobject UHeader
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (UHeader.headerOk())
        {
            Info<< "Reading field U\n" << endl;
            volVectorField U(UHeader, mesh);

            if (compressible)
            {
                calcCompressibleYPlus(mesh, runTime, U, yPlus);
            }
            else
            {
                calcIncompressibleYPlus(mesh, runTime, U, yPlus);
            }
        }
        else
        {
            Info<< "    no U field" << endl;
        }
        
        Info<< "Writing yPlus to field "
            << yPlus.name() << nl << endl;

        yPlus.write();
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
