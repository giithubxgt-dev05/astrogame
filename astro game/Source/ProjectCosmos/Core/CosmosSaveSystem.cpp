#include "CosmosSaveSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UCosmosSaveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Setup AutoSave timer (every 300 seconds = 5 minutes)
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &UCosmosSaveSystem::AutoSave, 300.0f, true);
	}
}

void UCosmosSaveSystem::Deinitialize()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
	}

	Super::Deinitialize();
}

bool UCosmosSaveSystem::SaveGame(int32 SlotIndex)
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);

	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UCosmosSaveGame>(UGameplayStatics::CreateSaveGameObject(UCosmosSaveGame::StaticClass()));
	}

	return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

bool UCosmosSaveSystem::LoadGame(int32 SlotIndex)
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UCosmosSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		return CurrentSaveGame != nullptr;
	}

	return false;
}

bool UCosmosSaveSystem::DeleteSave(int32 SlotIndex)
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}

	return false;
}

bool UCosmosSaveSystem::DoesSlotExist(int32 SlotIndex) const
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

UCosmosSaveGame* UCosmosSaveSystem::GetSaveData() const
{
	return CurrentSaveGame;
}

void UCosmosSaveSystem::UpdateSaveData(const FCosmosPlayerSaveData& NewData)
{
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UCosmosSaveGame>(UGameplayStatics::CreateSaveGameObject(UCosmosSaveGame::StaticClass()));
	}

	CurrentSaveGame->SaveData = NewData;
}

void UCosmosSaveSystem::AutoSave()
{
	// Save to default slot 0 for autosave
	SaveGame(0);
}
