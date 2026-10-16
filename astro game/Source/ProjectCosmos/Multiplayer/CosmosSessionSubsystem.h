#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CosmosSessionSubsystem.generated.h"

UENUM(BlueprintType)
enum class ESessionType : uint8
{
	Solo,
	PrivateCoOp,
	PublicCoOp
};

UCLASS()
class PROJECTCOSMOS_API UCosmosSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// M1 STUB
	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Session")
	ESessionType SessionType = ESessionType::Solo;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Session")
	bool bSessionActive = false;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Session")
	void CreateSession(ESessionType Type, int32 MaxPlayers);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Session")
	void FindSessions();

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Session")
	void JoinSession(FString ID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Session")
	void EndSession();

	UFUNCTION(BlueprintPure, Category = "Cosmos|Session")
	ESessionType GetSessionType() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Session")
	bool IsSessionActive() const;
};
