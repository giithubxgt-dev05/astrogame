#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CosmosSaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemSave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FName ItemID;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	int32 Quantity;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	int32 SlotIndex;

	FInventoryItemSave()
		: ItemID(NAME_None), Quantity(0), SlotIndex(0)
	{}
};

USTRUCT(BlueprintType)
struct FCosmosPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	int32 Version;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FName CurrentPlanetID;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	TArray<FInventoryItemSave> InventoryItems;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FName EquippedSpacesuitID;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FName ShipID;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FVector ShipLocation;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	TMap<FName, int32> ShipUpgrades;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	float Credits;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	TArray<FName> DiscoveredLocations;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	TArray<FName> CompletedMissions;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	TArray<FName> ResearchUnlocks;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	float PlaytimeSecs;

	FCosmosPlayerSaveData()
		: Version(1), PlayerLocation(FVector::ZeroVector), CurrentPlanetID(NAME_None),
		  EquippedSpacesuitID(NAME_None), ShipID(NAME_None), ShipLocation(FVector::ZeroVector),
		  Credits(0.0f), PlaytimeSecs(0.0f)
	{}
};

UCLASS(ClassGroup=(ProjectCosmos))
class PROJECTCOSMOS_API UCosmosSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Cosmos|SaveData")
	FCosmosPlayerSaveData SaveData;
};

UCLASS(ClassGroup=(ProjectCosmos))
class PROJECTCOSMOS_API UCosmosSaveSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="Cosmos|SaveSystem")
	bool SaveGame(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category="Cosmos|SaveSystem")
	bool LoadGame(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category="Cosmos|SaveSystem")
	bool DeleteSave(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category="Cosmos|SaveSystem")
	bool DoesSlotExist(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|SaveSystem")
	UCosmosSaveGame* GetSaveData() const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|SaveSystem")
	void UpdateSaveData(const FCosmosPlayerSaveData& NewData);

protected:
	UFUNCTION()
	void AutoSave();

private:
	UPROPERTY()
	UCosmosSaveGame* CurrentSaveGame;

	FTimerHandle AutoSaveTimerHandle;
};
