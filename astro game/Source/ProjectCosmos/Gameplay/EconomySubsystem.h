#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/MissionSubsystem.h" // For EPlanetID
#include "EconomySubsystem.generated.h"

class UInventoryComponent; // Forward declaration for inventory

USTRUCT(BlueprintType)
struct FMarketListing
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StockQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAvailable;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, float, NewAmount);

UCLASS()
class PROJECTCOSMOS_API UEconomySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Economy")
	float PlayerCredits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Economy")
	TMap<FName, FMarketListing> MarketListings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Economy")
	float PriceVolatility = 0.1f;

	UPROPERTY(BlueprintAssignable, Category = "Cosmos|Economy")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Economy")
	float GetPlayerCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Economy")
	void AddCredits(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Economy")
	bool SpendCredits(float Amount);

	UFUNCTION(BlueprintPure, Category = "Cosmos|Economy")
	float GetBuyPrice(FName ItemID) const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Economy")
	float GetSellPrice(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Economy")
	bool BuyItem(FName ItemID, int32 Qty, UInventoryComponent* Inventory);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Economy")
	bool SellItem(FName ItemID, int32 Qty, UInventoryComponent* Inventory);

	UFUNCTION(BlueprintPure, Category = "Cosmos|Economy")
	bool IsAffordable(FName ItemID, int32 Qty) const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Economy")
	TArray<FMarketListing> GetMarketListings(EPlanetID PlanetID) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Economy")
	void InitializeDefaultMarket();
};
