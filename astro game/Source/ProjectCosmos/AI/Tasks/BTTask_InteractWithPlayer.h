// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: Behavior Tree Task - Interact with Player (start dialogue)

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InteractWithPlayer.generated.h"

UCLASS()
class PROJECTCOSMOS_API UBTTask_InteractWithPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_InteractWithPlayer();

	// Blackboard key for the player actor target
	UPROPERTY(EditAnywhere, Category = "Cosmos|AI")
	FBlackboardKeySelector PlayerTargetKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
