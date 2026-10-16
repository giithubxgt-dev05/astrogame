// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: Behavior Tree Task - Move NPC to scheduled location

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToScheduledLocation.generated.h"

UCLASS()
class PROJECTCOSMOS_API UBTTask_MoveToScheduledLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToScheduledLocation();

	UPROPERTY(EditAnywhere, Category = "Cosmos|AI")
	float AcceptableRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Cosmos|AI")
	bool bUpdateTargetEveryFrame = false;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
