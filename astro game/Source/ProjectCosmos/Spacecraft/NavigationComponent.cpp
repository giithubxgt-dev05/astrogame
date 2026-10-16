#include "NavigationComponent.h"
#include "TravelSystemComponent.h"
#include "Components/PrimitiveComponent.h"

UNavigationComponent::UNavigationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    NavState = ENavState::Idle;
    SelectedDestination = EPlanetID::None;
}

void UNavigationComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UNavigationComponent::SelectDestination(EPlanetID Destination)
{
    if (AvailableDestinations.Contains(Destination))
    {
        SelectedDestination = Destination;
        NavState = ENavState::DestinationSelected;
        CurrentRoute = CalculateRoute(EPlanetID::Earth, Destination); // Example origin
        return true;
    }
    return false;
}

FNavRoute UNavigationComponent::CalculateRoute(EPlanetID InOrigin, EPlanetID InDestination)
{
    FNavRoute Route;
    Route.Origin = InOrigin;
    Route.Destination = InDestination;

    float DistanceAU = UCoordinateSystemLibrary::GetDistanceBetweenPlanetsAU(InOrigin, InDestination);
    Route.EstimatedTravelTimeSecs = DistanceAU * 60.0f; // 1 AU = 60s
    Route.DeltaVRequired = DistanceAU * 1000.0f; // Simplified

    UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    float ShipMass = PrimComp ? PrimComp->GetMass() : 10000.0f;

    Route.FuelRequired = Route.DeltaVRequired * ShipMass * 0.001f;

    NavState = ENavState::RoutePlotted;
    return Route;
}

void UNavigationComponent::BeginTravel()
{
    if (NavState == ENavState::RoutePlotted)
    {
        UTravelSystemComponent* TravelSystem = GetOwner()->FindComponentByClass<UTravelSystemComponent>();
        if (TravelSystem)
        {
            TravelSystem->BeginTravelTo(CurrentRoute.Destination);
            NavState = ENavState::Traveling;
        }
    }
}

ENavState UNavigationComponent::GetNavState() const
{
    return NavState;
}

FNavRoute UNavigationComponent::GetCurrentRoute() const
{
    return CurrentRoute;
}

TArray<EPlanetID> UNavigationComponent::GetAvailableDestinations() const
{
    return AvailableDestinations;
}

void UNavigationComponent::UnlockDestination(EPlanetID Destination)
{
    AvailableDestinations.AddUnique(Destination);
}

float UNavigationComponent::GetETA() const
{
    return CurrentRoute.EstimatedTravelTimeSecs; // Normally dynamic during travel
}

float UNavigationComponent::GetFuelCostForRoute(FNavRoute Route) const
{
    return Route.FuelRequired;
}
