#include "CosmosGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Forward declaration of UCosmosSaveGame to avoid include error before it's created.
// This will work as long as UCosmosSaveGame inherits from USaveGame.
#include "CosmosSaveSystem.h"

void UCosmosGameInstance::Init()
{
	Super::Init();

	WorldSeed = FMath::RandRange(1000, 99999);
	CurrentSaveSlot = TEXT("SaveSlot_0");
	bIsMultiplayerSession = false;
	
	LoadSaveData(CurrentSaveSlot);
}

void UCosmosGameInstance::LoadSaveData(const FString& SaveSlot)
{
	CurrentSaveSlot = SaveSlot;
	if (UGameplayStatics::DoesSaveGameExist(CurrentSaveSlot, 0))
	{
		CurrentSaveData = Cast<UCosmosSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));
	}
	else
	{
		// Create new save data
		CurrentSaveData = Cast<UCosmosSaveGame>(UGameplayStatics::CreateSaveGameObject(UCosmosSaveGame::StaticClass()));
	}
}

void UCosmosGameInstance::WriteSaveData()
{
	if (CurrentSaveData)
	{
		UGameplayStatics::SaveGameToSlot(CurrentSaveData, CurrentSaveSlot, 0);
	}
}

UCosmosSaveGame* UCosmosGameInstance::GetSaveData() const
{
	return CurrentSaveData;
}

void UCosmosGameInstance::TransitionToLevel(FName LevelName)
{
	if (GetWorld())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}

void UCosmosGameInstance::StartSinglePlayerSession()
{
	bIsMultiplayerSession = false;
	TransitionToLevel(FName("EarthLevel")); // Default start level
}
