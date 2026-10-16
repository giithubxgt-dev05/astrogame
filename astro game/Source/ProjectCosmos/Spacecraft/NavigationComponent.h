#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoordinateSystemLibrary.h" // Includes EPlanetID
#include "NavigationComponent.generated.h"

UENUM(BlueprintType)
enum class ENavState : uint8
{
    Idle,
    DestinationSelected,
    RoutePlotted,
    Traveling,
    Arrived
};

USTRUCT(BlueprintType)
struct FNavRoute
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlanetID Origin = EPlanetID::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlanetID Destination = EPlanetID::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EstimatedTravelTimeSecs = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DeltaVRequired = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FuelRequired = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestinationArrived, EPlanetID, Planet);

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UNavigationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNavigationComponent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Cosmos|Navigation")
    bool SelectDestination(EPlanetID Destination);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Navigation")
    FNavRoute CalculateRoute(EPlanetID InOrigin, EPlanetID InDestination);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Navigation")
    void BeginTravel();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Navigation")
    ENavState GetNavState() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Navigation")
    FNavRoute GetCurrentRoute() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Navigation")
    TArray<EPlanetID> GetAvailableDestinations() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Navigation")
    void UnlockDestination(EPlanetID Destination);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Navigation")
    float GetETA() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Navigation")
    float GetFuelCostForRoute(FNavRoute Route) const;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Navigation")
    FOnDestinationArrived OnDestinationArrived;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
    ENavState NavState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
    FNavRoute CurrentRoute;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
    EPlanetID SelectedDestination;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
    TArray<EPlanetID> AvailableDestinations;
};
