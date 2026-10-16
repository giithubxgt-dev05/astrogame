#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CosmosShipBase.h"
#include "ShipUpgradeComponent.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FUpgradeDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName UpgradeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CreditCost = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, float> StatBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, int32> RequiredResources;
};

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UShipUpgradeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShipUpgradeComponent();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Upgrades")
    bool CanUpgrade(FName UpgradeID, UInventoryComponent* PlayerInventory) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Upgrades")
    bool PerformUpgrade(FName UpgradeID, UInventoryComponent* PlayerInventory, UPARAM(ref) float& InOutCredits);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Upgrades")
    void ApplyUpgradesToShipStats(UPARAM(ref) FShipStats& Stats);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Upgrades")
    int32 GetUpgradeLevel(FName UpgradeID) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Upgrades")
    float GetUpgradeCost(FName UpgradeID) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Upgrades")
    TArray<FUpgradeDefinition> GetAllUpgrades() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
    FShipUpgradeState CurrentUpgrades;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
    TArray<FUpgradeDefinition> UpgradeDefinitions;
};
