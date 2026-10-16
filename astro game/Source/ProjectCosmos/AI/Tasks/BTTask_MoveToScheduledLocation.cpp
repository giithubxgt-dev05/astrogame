// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: BT Task implementation - Move NPC to scheduled daily location

#include "AI/Tasks/BTTask_MoveToScheduledLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NPCCharacterBase.h"
#include "Planet/EnvironmentalSystem.h"

UBTTask_MoveToScheduledLocation::UBTTask_MoveToScheduledLocation()
{
	NodeName = TEXT("Move To Scheduled Location");
}

EBTNodeResult::Type UBTTask_MoveToScheduledLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(AIController->GetPawn());
	if (!NPC) return EBTNodeResult::Failed;

	UWorld* World = NPC->GetWorld();
	if (!World) return EBTNodeResult::Failed;

	UEnvironmentalSystem* EnvSystem = World->GetSubsystem<UEnvironmentalSystem>();
	float CurrentHour = EnvSystem ? EnvSystem->GetTimeOfDay() : 12.0f;

	FNPCScheduleEntry ScheduledEntry = NPC->GetCurrentScheduledActivity(CurrentHour);
	FVector TargetLocation = ScheduledEntry.Location.IsNone() ? NPC->HomeLocation : NPC->GetActorLocation();

	// If the schedule entry has a named location, look it up from the blackboard or world
	// For now, use home location as fallback
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(
		TargetLocation,
		AcceptableRadius,
		true,
		true,
		true,
		false
	);

	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBTNodeResult::Succeeded;
	}
	else if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

FString UBTTask_MoveToScheduledLocation::GetStaticDescription() const
{
	return TEXT("Move NPC to their currently scheduled daily location based on time of day.");
}
