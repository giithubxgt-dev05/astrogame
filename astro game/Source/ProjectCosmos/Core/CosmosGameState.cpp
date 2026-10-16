#include "CosmosGameState.h"
#include "Net/UnrealNetwork.h"

ACosmosGameState::ACosmosGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentPlanetID = NAME_None;
	ServerTimestamp = 0.0f;
	ActivePlayerCount = 0;
	WorldSeed = 12345;
}

void ACosmosGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACosmosGameState, CurrentPlanetID);
	DOREPLIFETIME(ACosmosGameState, ServerTimestamp);
	DOREPLIFETIME(ACosmosGameState, ActivePlayerCount);
}

FName ACosmosGameState::GetCurrentPlanetDefinition() const
{
	return CurrentPlanetID;
}

void ACosmosGameState::BroadcastPlanetChange(FName NewPlanetID)
{
	CurrentPlanetID = NewPlanetID;
}

int32 ACosmosGameState::GetWorldSeed() const
{
	return WorldSeed;
}

void ACosmosGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		ServerTimestamp += DeltaSeconds;
	}
}
