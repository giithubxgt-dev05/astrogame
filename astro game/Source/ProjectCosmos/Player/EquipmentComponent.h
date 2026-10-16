#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class UHazardComponent;

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    Spacesuit,
    Helmet,
    Backpack,
    Gloves,
    Boots,
    Accessory1,
    Accessory2,
    Handheld
};

USTRUCT(BlueprintType)
struct FEquipmentItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentSlot EquipmentSlot = EEquipmentSlot::Spacesuit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OxygenCapacity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThermalProtection = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RadiationProtection = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PressureProtection = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EnergyCapacity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSoftObjectPath MeshPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped = false;
};

// Dummy enum for Hazard Component
UENUM(BlueprintType)
enum class EHazardType : uint8
{
    Vacuum,
    Radiation,
    ExtremeHeat,
    ExtremeCold,
    ToxicGas
};

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEquipmentComponent();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Equipment")
    TMap<EEquipmentSlot, FEquipmentItem> EquippedItems;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Equipment")
    void EquipItem(FName ItemID, EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Equipment")
    void UnequipItem(EEquipmentSlot Slot);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    FEquipmentItem GetEquippedItem(EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    bool IsSlotOccupied(EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    float GetTotalOxygenCapacity() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    float GetTotalThermalProtection() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    float GetTotalRadiationProtection() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    float GetTotalPressureProtection() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    float GetTotalWeight() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Equipment")
    void NotifyHazardComponent();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Equipment")
    float GetHazardProtectionFactor(EHazardType HazardType) const;
};
