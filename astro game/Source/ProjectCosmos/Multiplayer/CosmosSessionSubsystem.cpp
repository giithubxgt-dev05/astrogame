#include "Multiplayer/CosmosSessionSubsystem.h"

void UCosmosSessionSubsystem::CreateSession(ESessionType Type, int32 MaxPlayers)
{
	// M1 STUB
	SessionType = Type;
	bSessionActive = true;
	UE_LOG(LogTemp, Warning, TEXT("UCosmosSessionSubsystem::CreateSession STUB CALLED"));
}

void UCosmosSessionSubsystem::FindSessions()
{
	// M1 STUB
	UE_LOG(LogTemp, Warning, TEXT("UCosmosSessionSubsystem::FindSessions STUB CALLED"));
}

void UCosmosSessionSubsystem::JoinSession(FString ID)
{
	// M1 STUB
	bSessionActive = true;
	UE_LOG(LogTemp, Warning, TEXT("UCosmosSessionSubsystem::JoinSession STUB CALLED"));
}

void UCosmosSessionSubsystem::EndSession()
{
	// M1 STUB
	bSessionActive = false;
	SessionType = ESessionType::Solo;
	UE_LOG(LogTemp, Warning, TEXT("UCosmosSessionSubsystem::EndSession STUB CALLED"));
}

ESessionType UCosmosSessionSubsystem::GetSessionType() const
{
	return SessionType;
}

bool UCosmosSessionSubsystem::IsSessionActive() const
{
	return bSessionActive;
}
