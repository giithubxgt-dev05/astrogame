#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CosmosGameMode.generated.h"

UCLASS(ClassGroup=(ProjectCosmos))
class PROJECTCOSMOS_API ACosmosGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACosmosGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Cosmos|GameMode")
	TSubclassOf<class APawn> DefaultPlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category="Cosmos|GameMode")
	TSubclassOf<class APawn> DefaultShipClass;

	UPROPERTY(EditDefaultsOnly, Category="Cosmos|GameMode")
	FVector SpaceportSpawnLocation;

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameMode")
	void HandlePlayerDeath(class AController* PlayerController);

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameMode")
	void RespawnPlayer(class AController* PlayerController);

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameMode")
	FName GetCurrentPlanetID() const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameMode")
	void SetActivePlanet(FName NewPlanetID);

protected:
	FName CurrentPlanetID;
};
