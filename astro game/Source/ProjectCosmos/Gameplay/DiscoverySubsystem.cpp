#include "Gameplay/DiscoverySubsystem.h"

void UDiscoverySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UDiscoverySubsystem::RecordDiscovery(FDiscoveryRecord Record)
{
	if (HasDiscovered(Record.DiscoveryID))
	{
		return false;
	}

	AllDiscoveries.Add(Record);
	OnNewDiscovery.Broadcast(Record);
	return true;
}

TArray<FDiscoveryRecord> UDiscoverySubsystem::GetDiscoveriesForPlanet(EPlanetID PlanetID) const
{
	TArray<FDiscoveryRecord> Result;
	for (const FDiscoveryRecord& Record : AllDiscoveries)
	{
		if (Record.PlanetID == PlanetID)
		{
			Result.Add(Record);
		}
	}
	return Result;
}

TArray<FDiscoveryRecord> UDiscoverySubsystem::GetAllDiscoveries() const
{
	return AllDiscoveries;
}

bool UDiscoverySubsystem::HasDiscovered(FName DiscoveryID) const
{
	for (const FDiscoveryRecord& Record : AllDiscoveries)
	{
		if (Record.DiscoveryID == DiscoveryID)
		{
			return true;
		}
	}
	return false;
}

int32 UDiscoverySubsystem::GetDiscoveryCount() const
{
	return AllDiscoveries.Num();
}

FDiscoveryRecord UDiscoverySubsystem::GetFirstLandingRecord(EPlanetID PlanetID) const
{
	for (const FDiscoveryRecord& Record : AllDiscoveries)
	{
		if (Record.PlanetID == PlanetID && Record.DiscoveryType == EDiscoveryType::FirstLanding)
		{
			return Record;
		}
	}
	return FDiscoveryRecord();
}

void UDiscoverySubsystem::RecordFirstLanding(EPlanetID PlanetID, FVector Location)
{
	FName LandingID = FName(*FString::Printf(TEXT("FirstLanding_%d"), (int32)PlanetID));
	if (!HasDiscovered(LandingID))
	{
		FDiscoveryRecord Record;
		Record.DiscoveryID = LandingID;
		Record.DiscoveryType = EDiscoveryType::FirstLanding;
		Record.Title = FText::FromString(TEXT("First Landing"));
		Record.Description = FText::FromString(TEXT("Achieved first landing on this planet."));
		Record.PlanetID = PlanetID;
		Record.WorldLocation = Location;
		Record.bIsFirstDiscovery = true;
		Record.DiscoveredByPlayerName = TEXT("Player"); // In multiplayer, use real name

		RecordDiscovery(Record);
	}
}
