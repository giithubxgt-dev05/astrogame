#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BaseManagerSubsystem.generated.h"

UENUM(BlueprintType)
enum class EBaseModuleType : uint8
{
    Habitat UMETA(DisplayName = "Habitat"),
    PowerGenerator UMETA(DisplayName = "Power Generator"),
    SolarPanel UMETA(DisplayName = "Solar Panel"),
    Battery UMETA(DisplayName = "Battery"),
    OxygenGenerator UMETA(DisplayName = "Oxygen Generator"),
    WaterProcessor UMETA(DisplayName = "Water Processor"),
    Storage UMETA(DisplayName = "Storage"),
    Laboratory UMETA(DisplayName = "Laboratory"),
    MiningFacility UMETA(DisplayName = "Mining Facility"),
    CommunicationTower UMETA(DisplayName = "Communication Tower"),
    LandingPad UMETA(DisplayName = "Landing Pad"),
    VehicleGarage UMETA(DisplayName = "Vehicle Garage")
};

UENUM(BlueprintType)
enum class EPlanetID : uint8
{
    Earth,
    Moon,
    Mars,
    Unknown
};

USTRUCT(BlueprintType)
struct FBaseModule
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName ModuleID;

    UPROPERTY(BlueprintReadWrite)
    EBaseModuleType ModuleType;

    UPROPERTY(BlueprintReadWrite)
    FVector WorldLocation;

    UPROPERTY(BlueprintReadWrite)
    FRotator WorldRotation;

    UPROPERTY(BlueprintReadWrite)
    bool bIsPowered = false;

    UPROPERTY(BlueprintReadWrite)
    bool bIsOperational = false;

    UPROPERTY(BlueprintReadWrite)
    float PowerConsumption = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    float PowerGeneration = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    FString OwnerPlayerName;

    UPROPERTY(BlueprintReadWrite)
    EPlanetID PlanetID;
};

USTRUCT(BlueprintType)
struct FBaseData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName BaseID;

    UPROPERTY(BlueprintReadWrite)
    FString BaseName;

    UPROPERTY(BlueprintReadWrite)
    EPlanetID PlanetID;

    UPROPERTY(BlueprintReadWrite)
    TArray<FBaseModule> Modules;

    UPROPERTY(BlueprintReadWrite)
    float TotalPowerGeneration = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    float TotalPowerConsumption = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    bool bHasLifeSupport = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBaseModuleAdded, FName, BaseID, EBaseModuleType, ModuleType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasePowerChanged, FName, BaseID, float, NetPower);

UCLASS()
class PROJECTCOSMOS_API UBaseManagerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    FName CreateBase(FString Name, EPlanetID PlanetID, FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    bool AddModule(FName BaseID, EBaseModuleType ModuleType, FVector Loc, FRotator Rot);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    bool RemoveModule(FName BaseID, FName ModuleID);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    FBaseData GetBase(FName BaseID) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    TArray<FBaseData> GetBasesOnPlanet(EPlanetID PlanetID) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    void UpdatePowerGrid(FName BaseID);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    bool IsBaseLifeSupported(FName BaseID) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    FBaseData GetNearestBase(FVector WorldPos) const;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|BaseBuilding")
    FOnBaseModuleAdded OnBaseModuleAdded;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|BaseBuilding")
    FOnBasePowerChanged OnBasePowerChanged;

protected:
    UPROPERTY()
    TMap<FName, FBaseData> AllBases;

    UPROPERTY()
    FName ActiveBaseID;

    int32 BaseCounter;
    int32 ModuleCounter;
};
