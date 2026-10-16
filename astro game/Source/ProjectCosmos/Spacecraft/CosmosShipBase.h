#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CosmosShipBase.generated.h"

// Forward declarations
class UStaticMeshComponent;
class UFlightPhysicsComponent;
class UNavigationComponent;
class ULandingSystemComponent;
class UShipUpgradeComponent;
class UInventoryComponent; // Assuming this exists or will be created
class UNiagaraComponent;
class UAudioComponent;
class ACosmosCharacter; // Assuming this exists

enum class EPlanetID : uint8;

UENUM(BlueprintType)
enum class EShipClass : uint8
{
    Explorer,
    Cargo,
    Research,
    Mining,
    Colony,
    Advanced
};

UENUM(BlueprintType)
enum class EShipState : uint8
{
    Grounded,
    TaxiingToLaunch,
    Launching,
    AtmosphericFlight,
    OrbitalFlight,
    TravelWarp,
    Approaching,
    Descending,
    Landing,
    Docked,
    Damaged
};

UENUM(BlueprintType)
enum class ELandingGearState : uint8
{
    Retracted,
    Deploying,
    Deployed,
    Retracting
};

USTRUCT(BlueprintType)
struct FShipStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxFuelKg = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentFuelKg = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCargo = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThrustKN = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxSpeedAtmo = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxSpeedVacuum = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HullIntegrity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ShieldStrength = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LifeSupportCapacity = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SensorRange = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HeatShieldRating = 50.0f;
};

USTRUCT(BlueprintType)
struct FShipUpgradeState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EngineLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FuelLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ShieldLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CargoLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SensorLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LifeSupportLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HeatShieldLevel = 1;
};

UCLASS()
class PROJECTCOSMOS_API ACosmosShipBase : public APawn
{
    GENERATED_BODY()

public:
    ACosmosShipBase();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void SetupPhysicsConstraints();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void OnPilotBoarded(ACosmosCharacter* NewPilot);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void OnPilotExited();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void LaunchSequence();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void LandingSequence(FVector TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void ActivateWarpTravel(EPlanetID Destination);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Spacecraft")
    float GetCurrentAltitudeKm() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Spacecraft")
    float GetCurrentVelocityKmh() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Spacecraft")
    void ConsumeFuel(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Spacecraft")
    bool IsFuelEmpty() const;

    UFUNCTION()
    void OnRep_ShipState();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Spacecraft")
    FVector GetPilotSeatSocketLocation() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Spacecraft")
    FVector GetExitSocketLocation() const;

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RequestLaunch();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RequestLanding(FVector TargetLocation);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UStaticMeshComponent* HullMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UFlightPhysicsComponent* FlightPhysicsComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UNavigationComponent* NavigationComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    ULandingSystemComponent* LandingSystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UShipUpgradeComponent* ShipUpgradeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UNiagaraComponent* EngineTrail;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UAudioComponent* EngineAudio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Spacecraft")
    EShipClass ShipClass;

    UPROPERTY(ReplicatedUsing = OnRep_ShipState, VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Spacecraft")
    EShipState ShipState;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Spacecraft")
    ACosmosCharacter* PilotCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Spacecraft")
    FShipStats ShipStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Spacecraft")
    FString ShipName;

    float FuelBurnRateKgPerSec = 10.0f;
    FTimerHandle LaunchSequenceTimerHandle;
    
    void FinishLaunchSequence();
};
