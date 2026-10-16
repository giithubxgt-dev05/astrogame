#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CosmosGameInstance.generated.h"

class UCosmosSaveGame;

UCLASS(ClassGroup=(ProjectCosmos))
class PROJECTCOSMOS_API UCosmosGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|GameInstance")
	int32 WorldSeed;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|GameInstance")
	FString CurrentSaveSlot;

	UPROPERTY(BlueprintReadWrite, Category="Cosmos|GameInstance")
	bool bIsMultiplayerSession;

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameInstance")
	void LoadSaveData(const FString& SaveSlot);

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameInstance")
	void WriteSaveData();

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameInstance")
	UCosmosSaveGame* GetSaveData() const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameInstance")
	void TransitionToLevel(FName LevelName);

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameInstance")
	void StartSinglePlayerSession();

protected:
	UPROPERTY()
	UCosmosSaveGame* CurrentSaveData;
};
