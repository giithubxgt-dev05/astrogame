#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CosmosAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;

UCLASS()
class PROJECTCOSMOS_API ACosmosAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACosmosAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|AI")
	UBlackboardData* BlackboardData;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|AI")
	void SetBlackboardPatrolTarget(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|AI")
	void SetBlackboardPlayerTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|AI")
	void SetAlertState(bool bIsAlerted);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|AI")
	void SetEmotionState(FName Emotion);

	UFUNCTION(BlueprintPure, Category = "Cosmos|AI")
	float GetDistanceToPlayer() const;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|AI")
	void UpdatePerception();
};
