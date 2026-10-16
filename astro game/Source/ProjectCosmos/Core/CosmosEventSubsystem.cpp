#include "CosmosEventSubsystem.h"

void UCosmosEventSubsystem::BroadcastPlanetChange(FName NewPlanetID)
{
	OnPlayerPlanetChanged.Broadcast(NewPlanetID);
}

void UCosmosEventSubsystem::BroadcastResourceCollected(FName ResourceID, int32 Amount)
{
	OnResourceCollected.Broadcast(ResourceID, Amount);
}

void UCosmosEventSubsystem::BroadcastShipStateChanged(FName ShipState)
{
	OnShipStateChanged.Broadcast(ShipState);
}

void UCosmosEventSubsystem::BroadcastSurvivalThreatChanged(FName ThreatType, float Severity)
{
	OnSurvivalThreatChanged.Broadcast(ThreatType, Severity);
}

void UCosmosEventSubsystem::BroadcastDiscoveryMade(FName DiscoveryID, FVector Location)
{
	OnDiscoveryMade.Broadcast(DiscoveryID, Location);
}

void UCosmosEventSubsystem::BroadcastMissionUpdated(FName MissionID, FName NewStatus)
{
	OnMissionUpdated.Broadcast(MissionID, NewStatus);
}

void UCosmosEventSubsystem::BroadcastPlayerDied(FString Cause)
{
	OnPlayerDied.Broadcast(Cause);
}
