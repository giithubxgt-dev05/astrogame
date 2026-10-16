#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CosmosEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPlanetChanged, FName, NewPlanetID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceCollected, FName, ResourceID, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipStateChanged, FName, ShipState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSurvivalThreatChanged, FName, ThreatType, float, Severity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiscoveryMade, FName, DiscoveryID, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionUpdated, FName, MissionID, FName, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDied, FString, Cause);

UCLASS(ClassGroup=(ProjectCosmos))
class PROJECTCOSMOS_API UCosmosEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnPlayerPlanetChanged OnPlayerPlanetChanged;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnResourceCollected OnResourceCollected;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnShipStateChanged OnShipStateChanged;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnSurvivalThreatChanged OnSurvivalThreatChanged;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnDiscoveryMade OnDiscoveryMade;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnMissionUpdated OnMissionUpdated;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Events")
	FOnPlayerDied OnPlayerDied;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastPlanetChange(FName NewPlanetID);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastResourceCollected(FName ResourceID, int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastShipStateChanged(FName ShipState);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastSurvivalThreatChanged(FName ThreatType, float Severity);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastDiscoveryMade(FName DiscoveryID, FVector Location);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastMissionUpdated(FName MissionID, FName NewStatus);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Events")
	void BroadcastPlayerDied(FString Cause);
};
