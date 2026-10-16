#include "TravelSystemComponent.h"
#include "CoordinateSystemLibrary.h"
#include "TimerManager.h"

UTravelSystemComponent::UTravelSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TravelPhase = ETravelPhase::None;
    TravelSpeedMultiplier = 1.0f;
    bSkipCinematic = false;
    TravelProgress = 0.0f;
    EstimatedTimeRemaining = 0.0f;
}

void UTravelSystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTravelSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (TravelPhase >= ETravelPhase::WarpAcceleration && TravelPhase <= ETravelPhase::WarpDeceleration)
    {
        EstimatedTimeRemaining -= DeltaTime * TravelSpeedMultiplier;
        TravelProgress = FMath::Clamp(1.0f - (EstimatedTimeRemaining / TotalTravelTime), 0.0f, 1.0f);
    }
}

void UTravelSystemComponent::BeginTravelTo(EPlanetID Dest)
{
    Destination = Dest;
    Origin = EPlanetID::Earth; // Assuming earth for now, should pull from Ship's current loc

    float DistanceAU = UCoordinateSystemLibrary::GetDistanceBetweenPlanetsAU(Origin, Destination);
    TotalTravelTime = DistanceAU * 60.0f; // 1 AU = 60s at 1x
    EstimatedTimeRemaining = TotalTravelTime;

    TravelPhase = ETravelPhase::PreWarp_Orbital;
    OnTravelPhaseChanged.Broadcast(TravelPhase);

    if (!bSkipCinematic)
    {
        GetWorld()->GetTimerManager().SetTimer(TravelPhaseTimerHandle, this, &UTravelSystemComponent::AdvanceTravelPhase, 3.0f, false);
    }
    else
    {
        SkipToArrival();
    }
}

void UTravelSystemComponent::AdvanceTravelPhase()
{
    switch (TravelPhase)
    {
        case ETravelPhase::PreWarp_Orbital:
            TravelPhase = ETravelPhase::WarpAcceleration;
            GetWorld()->GetTimerManager().SetTimer(TravelPhaseTimerHandle, this, &UTravelSystemComponent::AdvanceTravelPhase, 5.0f, false);
            break;
        case ETravelPhase::WarpAcceleration:
            TravelPhase = ETravelPhase::WarpCruise;
            GetWorld()->GetTimerManager().SetTimer(TravelPhaseTimerHandle, this, &UTravelSystemComponent::AdvanceTravelPhase, FMath::Max(1.0f, (TotalTravelTime - 10.0f) / TravelSpeedMultiplier), false);
            break;
        case ETravelPhase::WarpCruise:
            TravelPhase = ETravelPhase::WarpDeceleration;
            GetWorld()->GetTimerManager().SetTimer(TravelPhaseTimerHandle, this, &UTravelSystemComponent::AdvanceTravelPhase, 5.0f, false);
            break;
        case ETravelPhase::WarpDeceleration:
            TravelPhase = ETravelPhase::Arriving;
            GetWorld()->GetTimerManager().SetTimer(TravelPhaseTimerHandle, this, &UTravelSystemComponent::OnArrivalComplete, 3.0f, false);
            break;
        default:
            break;
    }
    
    OnTravelPhaseChanged.Broadcast(TravelPhase);
}

void UTravelSystemComponent::SkipToArrival()
{
    GetWorld()->GetTimerManager().ClearTimer(TravelPhaseTimerHandle);
    TravelProgress = 1.0f;
    EstimatedTimeRemaining = 0.0f;
    TravelPhase = ETravelPhase::Arriving;
    OnTravelPhaseChanged.Broadcast(TravelPhase);
    OnArrivalComplete();
}

float UTravelSystemComponent::GetTravelProgress() const
{
    return TravelProgress;
}

ETravelPhase UTravelSystemComponent::GetCurrentPhase() const
{
    return TravelPhase;
}

FString UTravelSystemComponent::GetTravelStats() const
{
    return FString::Printf(TEXT("Phase: %d, ETA: %.1fs"), (int32)TravelPhase, EstimatedTimeRemaining);
}

void UTravelSystemComponent::OnArrivalComplete()
{
    TravelPhase = ETravelPhase::Complete;
    OnTravelPhaseChanged.Broadcast(TravelPhase);
    
    // In full implementation, call PlanetManager->SetCurrentPlanet(Destination)
    
    OnArrived.Broadcast(Destination);
    TravelPhase = ETravelPhase::None;
}
