    fvMesh& mesh = fluidRegions[i];

    combustionModels::rhoCombustionModel& reaction = reactionFluid[i];
    rhoReactionThermo& thermo = reaction.thermo();
    PtrList<volScalarField>& Y = reactionFluid[i].thermo().composition().Y(); 
    thermo.validate(args.executable(), "h", "e");

    volScalarField& rho = rhoFluid[i];
    volVectorField& U = UFluid[i];
    surfaceScalarField& phi = phiFluid[i];

    compressible::turbulenceModel& turb = turbulence[i];
    volScalarField& K = KFluid[i];
    volScalarField& dpdt = dpdtFluid[i];
    volScalarField& Qdot = QdotFluid[i];

    volScalarField& p = thermo.p();
    const volScalarField& psi = thermo.psi();

    volScalarField& p_rgh = p_rghFluid[i];

    const dimensionedVector& g = gFluid[i];
    const volScalarField& gh = ghFluid[i];
    const surfaceScalarField& ghf = ghfFluid[i];

    radiation::radiationModel& rad = radiation[i];

    IOMRFZoneList& MRF = MRFfluid[i];
    fv::options& fvOptions = fluidFvOptions[i];
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields = fieldsFluid[i];
    
    const dimensionedScalar initialMass
    (
        "initialMass",
        dimMass,
        initialMassFluid[i]
    );

    const bool frozenFlow = frozenFlowFluid[i];

    basicMultiComponentMixture& composition = reactionFluid[i].thermo().composition();	
 
    const word inertSpecie(thermo.lookup("inertSpecie"));
    if (!composition.species().found(inertSpecie))
    {
        FatalIOErrorIn(args.executable().c_str(), thermo)
            << "Inert specie " << inertSpecie << " not found in available species "
            << composition.species()
            << exit(FatalIOError);
    }

    const label inertIndex(composition.species()[inertSpecie]);   

    bool& resReachedFluid = residualReachedFluid[i];
    bool& residualControlUsed = residualControlUsedFluid[i];

