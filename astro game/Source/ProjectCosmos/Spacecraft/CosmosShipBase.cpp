#include "CosmosShipBase.h"
#include "FlightPhysicsComponent.h"
#include "NavigationComponent.h"
#include "LandingSystemComponent.h"
#include "ShipUpgradeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"

// Note: ACosmosCharacter, UInventoryComponent, EPlanetID should be defined elsewhere.

ACosmosShipBase::ACosmosShipBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    HullMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HullMesh"));
    RootComponent = HullMesh;
    
    FlightPhysicsComponent = CreateDefaultSubobject<UFlightPhysicsComponent>(TEXT("FlightPhysicsComponent"));
    NavigationComponent = CreateDefaultSubobject<UNavigationComponent>(TEXT("NavigationComponent"));
    LandingSystemComponent = CreateDefaultSubobject<ULandingSystemComponent>(TEXT("LandingSystemComponent"));
    ShipUpgradeComponent = CreateDefaultSubobject<UShipUpgradeComponent>(TEXT("ShipUpgradeComponent"));
    
    // InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    
    EngineTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EngineTrail"));
    EngineTrail->SetupAttachment(RootComponent);
    
    EngineAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
    EngineAudio->SetupAttachment(RootComponent);
    
    ShipClass = EShipClass::Explorer;
    ShipState = EShipState::Grounded;
    ShipName = TEXT("Cosmos Vessel");
}

void ACosmosShipBase::BeginPlay()
{
    Super::BeginPlay();
    SetupPhysicsConstraints();
}

void ACosmosShipBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (ShipState == EShipState::Launching || ShipState == EShipState::AtmosphericFlight || ShipState == EShipState::OrbitalFlight)
    {
        ConsumeFuel(DeltaTime);
    }
}

void ACosmosShipBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACosmosShipBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACosmosShipBase, ShipState);
    DOREPLIFETIME(ACosmosShipBase, PilotCharacter);
}

void ACosmosShipBase::SetupPhysicsConstraints()
{
    if (HullMesh)
    {
        HullMesh->SetSimulatePhysics(true);
        HullMesh->SetEnableGravity(true);
    }
}

void ACosmosShipBase::OnPilotBoarded(ACosmosCharacter* NewPilot)
{
    if (HasAuthority())
    {
        PilotCharacter = NewPilot;
    }
}

void ACosmosShipBase::OnPilotExited()
{
    if (HasAuthority())
    {
        PilotCharacter = nullptr;
    }
}

void ACosmosShipBase::LaunchSequence()
{
    if (HasAuthority())
    {
        ShipState = EShipState::Launching;
        if (EngineAudio) EngineAudio->Play();
        if (EngineTrail) EngineTrail->Activate();
        
        if (FlightPhysicsComponent)
        {
            FlightPhysicsComponent->SetThrust(1.0f);
            FlightPhysicsComponent->SetThrustVector(FVector::UpVector);
        }
        
        GetWorld()->GetTimerManager().SetTimer(LaunchSequenceTimerHandle, this, &ACosmosShipBase::FinishLaunchSequence, 30.0f, false);
    }
}

void ACosmosShipBase::FinishLaunchSequence()
{
    if (HasAuthority())
    {
        ShipState = EShipState::OrbitalFlight;
        // Fire event / notify
    }
}

void ACosmosShipBase::LandingSequence(FVector TargetLocation)
{
    if (HasAuthority())
    {
        ShipState = EShipState::Descending;
        if (LandingSystemComponent)
        {
            LandingSystemComponent->SetLandingTarget(TargetLocation);
            LandingSystemComponent->ActivateAssistedLanding();
        }
    }
}

void ACosmosShipBase::ActivateWarpTravel(EPlanetID Destination)
{
    if (HasAuthority())
    {
        ShipState = EShipState::TravelWarp;
        // Warp travel logic integration with TravelSystemComponent
    }
}

float ACosmosShipBase::GetCurrentAltitudeKm() const
{
    return GetActorLocation().Z / 100000.0f;
}

float ACosmosShipBase::GetCurrentVelocityKmh() const
{
    if (HullMesh)
    {
        return HullMesh->GetComponentVelocity().Size() * 0.036f; // cm/s to km/h
    }
    return 0.0f;
}

void ACosmosShipBase::ConsumeFuel(float DeltaTime)
{
    if (ShipStats.CurrentFuelKg > 0)
    {
        ShipStats.CurrentFuelKg -= FuelBurnRateKgPerSec * DeltaTime;
        if (ShipStats.CurrentFuelKg < 0)
        {
            ShipStats.CurrentFuelKg = 0;
        }
    }
}

bool ACosmosShipBase::IsFuelEmpty() const
{
    return ShipStats.CurrentFuelKg <= 0.0f;
}

void ACosmosShipBase::OnRep_ShipState()
{
    // Handle VFX/Audio changes based on state replication
}

FVector ACosmosShipBase::GetPilotSeatSocketLocation() const
{
    if (HullMesh && HullMesh->DoesSocketExist("PilotSeat"))
    {
        return HullMesh->GetSocketLocation("PilotSeat");
    }
    return GetActorLocation();
}

FVector ACosmosShipBase::GetExitSocketLocation() const
{
    if (HullMesh && HullMesh->DoesSocketExist("Exit"))
    {
        return HullMesh->GetSocketLocation("Exit");
    }
    return GetActorLocation() + FVector(200.0f, 0, 0);
}

void ACosmosShipBase::Server_RequestLaunch_Implementation()
{
    LaunchSequence();
}

bool ACosmosShipBase::Server_RequestLaunch_Validate()
{
    return ShipState == EShipState::Grounded || ShipState == EShipState::TaxiingToLaunch;
}

void ACosmosShipBase::Server_RequestLanding_Implementation(FVector TargetLocation)
{
    LandingSequence(TargetLocation);
}

bool ACosmosShipBase::Server_RequestLanding_Validate(FVector TargetLocation)
{
    return ShipState == EShipState::OrbitalFlight || ShipState == EShipState::AtmosphericFlight;
}
