#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoordinateSystemLibrary.h" // For EPlanetID
#include "TravelSystemComponent.generated.h"

UENUM(BlueprintType)
enum class ETravelPhase : uint8
{
    None,
    PreWarp_Orbital,
    WarpAcceleration,
    WarpCruise,
    WarpDeceleration,
    Arriving,
    Complete
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTravelPhaseChanged, ETravelPhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrived, EPlanetID, Planet);

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UTravelSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTravelSystemComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Travel")
    void BeginTravelTo(EPlanetID Dest);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Travel")
    void SkipToArrival();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Travel")
    float GetTravelProgress() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Travel")
    ETravelPhase GetCurrentPhase() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Travel")
    FString GetTravelStats() const;

    UFUNCTION()
    void OnArrivalComplete();

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Travel")
    FOnTravelPhaseChanged OnTravelPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Travel")
    FOnArrived OnArrived;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel")
    ETravelPhase TravelPhase;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel")
    EPlanetID Origin;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel")
    EPlanetID Destination;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel")
    float TravelProgress;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel")
    float EstimatedTimeRemaining;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
    float TravelSpeedMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
    bool bSkipCinematic;

    FTimerHandle TravelPhaseTimerHandle;
    float TotalTravelTime;
    
    void AdvanceTravelPhase();
};
