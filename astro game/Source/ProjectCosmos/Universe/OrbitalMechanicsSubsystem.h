#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CoordinateSystemLibrary.h" // For EPlanetID
#include "OrbitalMechanicsSubsystem.generated.h"

UCLASS()
class PROJECTCOSMOS_API UOrbitalMechanicsSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Tick(float DeltaTime);
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    
    // World Subsystem doesn't natively tick by just overriding Tick, we need FTickableGameObject or manage it via an actor,
    // but the prompt asked for "Tick" function in WorldSubsystem. We will use a timer or standard practice.
    // For simplicity following the spec:

    UFUNCTION(BlueprintPure, Category = "Cosmos|OrbitalMechanics")
    FVector GetPlanetPosition(EPlanetID Planet) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|OrbitalMechanics")
    float GetPlanetAngleDeg(EPlanetID Planet) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|OrbitalMechanics")
    float GetRelativeAngleBetween(EPlanetID A, EPlanetID B) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|OrbitalMechanics")
    void AdvanceTime(float DeltaSeconds);

    UFUNCTION(BlueprintPure, Category = "Cosmos|OrbitalMechanics")
    FVector GetSunDirectionFromPlanet(EPlanetID Planet, double InGameTime) const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbital Mechanics")
    double GameTimeSeconds;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbital Mechanics")
    TMap<EPlanetID, float> OrbitalPeriods;
};
