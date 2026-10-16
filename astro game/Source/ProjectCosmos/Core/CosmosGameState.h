#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CosmosGameState.generated.h"

UCLASS(ClassGroup=(ProjectCosmos))
class PROJECTCOSMOS_API ACosmosGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACosmosGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Cosmos|GameState")
	FName CurrentPlanetID;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Cosmos|GameState")
	float ServerTimestamp;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Cosmos|GameState")
	int32 ActivePlayerCount;

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameState")
	FName GetCurrentPlanetDefinition() const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameState")
	void BroadcastPlanetChange(FName NewPlanetID);

	UFUNCTION(BlueprintCallable, Category="Cosmos|GameState")
	int32 GetWorldSeed() const;

	virtual void Tick(float DeltaSeconds) override;

protected:
	int32 WorldSeed;
};
