#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CosmosNetworkManager.generated.h"

UCLASS()
class PROJECTCOSMOS_API UCosmosNetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Network")
	bool bIsMultiplayerEnabled = false;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Network")
	int32 MaxPlayers = 8;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Network")
	FString CurrentSessionID;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Network")
	TArray<FString> ConnectedPlayers;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Network")
	void HostSession(int32 InMaxPlayers, bool bIsPublic);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Network")
	void JoinSession(FString SessionID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Network")
	void LeaveSession();

	UFUNCTION(BlueprintPure, Category = "Cosmos|Network")
	TArray<FString> GetConnectedPlayers() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Network")
	FString GetCurrentSessionID() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Network")
	bool IsInMultiplayerSession() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Network")
	void BroadcastPlayerAction(FName ActionID, FVector Location);

	UFUNCTION(BlueprintPure, Category = "Cosmos|Network")
	bool IsServer() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Network")
	int32 GetPing() const;
};
