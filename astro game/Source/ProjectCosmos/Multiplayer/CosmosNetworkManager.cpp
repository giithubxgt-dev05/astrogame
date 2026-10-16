#include "Multiplayer/CosmosNetworkManager.h"

void UCosmosNetworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	bIsMultiplayerEnabled = false;
	MaxPlayers = 8;
	ConnectedPlayers.Add(TEXT("LocalPlayer"));
}

void UCosmosNetworkManager::HostSession(int32 InMaxPlayers, bool bIsPublic)
{
	// M1 STUB: This function is a no-op until M6 multiplayer implementation
	UE_LOG(LogTemp, Warning, TEXT("UCosmosNetworkManager::HostSession STUB CALLED (M6 TODO)"));
}

void UCosmosNetworkManager::JoinSession(FString SessionID)
{
	// M1 STUB: This function is a no-op until M6 multiplayer implementation
	UE_LOG(LogTemp, Warning, TEXT("UCosmosNetworkManager::JoinSession STUB CALLED (M6 TODO)"));
}

void UCosmosNetworkManager::LeaveSession()
{
	// M1 STUB: This function is a no-op until M6 multiplayer implementation
	UE_LOG(LogTemp, Warning, TEXT("UCosmosNetworkManager::LeaveSession STUB CALLED (M6 TODO)"));
}

TArray<FString> UCosmosNetworkManager::GetConnectedPlayers() const
{
	// M1 STUB: Returns only the local player
	return ConnectedPlayers;
}

FString UCosmosNetworkManager::GetCurrentSessionID() const
{
	// M1 STUB: This function is a no-op until M6 multiplayer implementation
	return TEXT("SinglePlayer_M1");
}

bool UCosmosNetworkManager::IsInMultiplayerSession() const
{
	// M1 STUB: Always false in M1
	return false;
}

void UCosmosNetworkManager::BroadcastPlayerAction(FName ActionID, FVector Location)
{
	// M1 STUB: This function is a no-op until M6 multiplayer implementation
	UE_LOG(LogTemp, Log, TEXT("UCosmosNetworkManager::BroadcastPlayerAction STUB CALLED - Action: %s"), *ActionID.ToString());
}

bool UCosmosNetworkManager::IsServer() const
{
	// M1 STUB: Returns true always in M1, checks authority in M6
	return true;
}

int32 UCosmosNetworkManager::GetPing() const
{
	// M1 STUB: Returns 0 in M1
	return 0;
}
