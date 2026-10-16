#include "Gameplay/EconomySubsystem.h"
// #include "InventoryComponent.h" // Uncomment when implemented

void UEconomySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PlayerCredits = 1000.0f; // Default starting credits
	InitializeDefaultMarket();
}

float UEconomySubsystem::GetPlayerCredits() const
{
	return PlayerCredits;
}

void UEconomySubsystem::AddCredits(float Amount)
{
	if (Amount > 0.0f)
	{
		PlayerCredits += Amount;
		OnCreditsChanged.Broadcast(PlayerCredits);
	}
}

bool UEconomySubsystem::SpendCredits(float Amount)
{
	if (Amount > 0.0f && PlayerCredits >= Amount)
	{
		PlayerCredits -= Amount;
		OnCreditsChanged.Broadcast(PlayerCredits);
		return true;
	}
	return false;
}

float UEconomySubsystem::GetBuyPrice(FName ItemID) const
{
	if (MarketListings.Contains(ItemID))
	{
		return MarketListings[ItemID].BuyPrice;
	}
	return 0.0f;
}

float UEconomySubsystem::GetSellPrice(FName ItemID) const
{
	if (MarketListings.Contains(ItemID))
	{
		return MarketListings[ItemID].SellPrice;
	}
	return 0.0f;
}

bool UEconomySubsystem::BuyItem(FName ItemID, int32 Qty, UInventoryComponent* Inventory)
{
	if (!MarketListings.Contains(ItemID)) return false;

	FMarketListing& Listing = MarketListings[ItemID];
	if (!Listing.bIsAvailable || Listing.StockQuantity < Qty) return false;

	float TotalCost = Listing.BuyPrice * Qty;
	if (SpendCredits(TotalCost))
	{
		Listing.StockQuantity -= Qty;
		// if (Inventory) Inventory->AddItem(ItemID, Qty);
		return true;
	}
	return false;
}

bool UEconomySubsystem::SellItem(FName ItemID, int32 Qty, UInventoryComponent* Inventory)
{
	if (!MarketListings.Contains(ItemID)) return false;

	// if (Inventory && Inventory->HasItem(ItemID, Qty))
	{
		FMarketListing& Listing = MarketListings[ItemID];
		float TotalEarnings = Listing.SellPrice * Qty;
		// Inventory->RemoveItem(ItemID, Qty);
		AddCredits(TotalEarnings);
		Listing.StockQuantity += Qty;
		return true;
	}
	return false;
}

bool UEconomySubsystem::IsAffordable(FName ItemID, int32 Qty) const
{
	float Cost = GetBuyPrice(ItemID) * Qty;
	return PlayerCredits >= Cost;
}

TArray<FMarketListing> UEconomySubsystem::GetMarketListings(EPlanetID PlanetID) const
{
	TArray<FMarketListing> Result;
	// Simplification: returning all listings regardless of planet for stub
	for (const auto& Pair : MarketListings)
	{
		Result.Add(Pair.Value);
	}
	return Result;
}

void UEconomySubsystem::InitializeDefaultMarket()
{
	FMarketListing Spacesuit;
	Spacesuit.ItemID = TEXT("Item_Spacesuit_Basic");
	Spacesuit.BuyPrice = 500.0f;
	Spacesuit.SellPrice = 500.0f * 0.6f;
	Spacesuit.StockQuantity = 10;
	Spacesuit.bIsAvailable = true;

	FMarketListing Oxygen;
	Oxygen.ItemID = TEXT("Item_OxygenTank");
	Oxygen.BuyPrice = 50.0f;
	Oxygen.SellPrice = 50.0f * 0.6f;
	Oxygen.StockQuantity = 100;
	Oxygen.bIsAvailable = true;

	FMarketListing Regolith;
	Regolith.ItemID = TEXT("Item_Regolith");
	Regolith.BuyPrice = 10.0f;
	Regolith.SellPrice = 10.0f * 0.6f;
	Regolith.StockQuantity = 500;
	Regolith.bIsAvailable = true;

	MarketListings.Add(Spacesuit.ItemID, Spacesuit);
	MarketListings.Add(Oxygen.ItemID, Oxygen);
	MarketListings.Add(Regolith.ItemID, Regolith);
}
