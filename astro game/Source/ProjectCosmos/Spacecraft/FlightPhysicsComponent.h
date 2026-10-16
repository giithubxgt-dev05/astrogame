#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlightPhysicsComponent.generated.h"

UENUM(BlueprintType)
enum class EFlightMode : uint8
{
    Atmospheric,
    Vacuum,
    Reentry
};

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UFlightPhysicsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFlightPhysicsComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void SetThrust(float NormalizedThrust);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void SetThrustVector(FVector NewThrustVector);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void ApplyAtmosphericDrag(UPrimitiveComponent* TargetComp, float AtmoDensity, FVector Velocity);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void ApplyReentryHeat(float VelocityKms, float AtmoDensity);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void SetFlightMode(EFlightMode NewMode);

    UFUNCTION(BlueprintPure, Category = "Cosmos|FlightPhysics")
    FVector GetDragForce(FVector Velocity, float AtmoDensity) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void ApplyGravity(float GravityScale);

    UFUNCTION(BlueprintPure, Category = "Cosmos|FlightPhysics")
    float GetCurrentHeatLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|FlightPhysics")
    void Autostabilize(UPrimitiveComponent* TargetComp);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    EFlightMode FlightMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    FVector ThrustVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    float CurrentThrust;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    float MaxThrust;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    float AtmosphericDragCoeff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    float ReferenceArea;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    bool bAtmosphericFlightEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    float CurrentHeatLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
    bool bAutostabilize;
};
