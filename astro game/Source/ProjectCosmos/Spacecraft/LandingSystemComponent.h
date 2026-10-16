#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CosmosShipBase.h" // For ELandingGearState
#include "LandingSystemComponent.generated.h"

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API ULandingSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULandingSystemComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void DeployLandingGear();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void RetractLandingGear();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void SetLandingTarget(FVector Target);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void ActivateAssistedLanding();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void DeactivateAssistedLanding();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void UpdateRadarAltitude();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Landing")
    ELandingGearState GetLandingGearState() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Landing")
    bool IsLandingSafe() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void OnLandingComplete();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Landing")
    void PIDAssistedDescent(float DeltaTime);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    ELandingGearState LandingGearState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    bool bAssistedLandingActive;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    float RadarAltitudeM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landing")
    float LandingGearDeployAltitudeM;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    float DescentRateMs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    bool bLandingTargetSet;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    FVector LandingTarget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landing")
    bool bLandingComplete;
};
