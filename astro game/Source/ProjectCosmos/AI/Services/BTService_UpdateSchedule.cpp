// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: BT Service implementation - Updates NPC schedule state each frame

#include "AI/Services/BTService_UpdateSchedule.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NPCCharacterBase.h"
#include "Planet/EnvironmentalSystem.h"

UBTService_UpdateSchedule::UBTService_UpdateSchedule()
{
	NodeName = TEXT("Update NPC Schedule");
	Interval = 5.0f; // Check schedule every 5 seconds
	RandomDeviation = 1.0f;
}

void UBTService_UpdateSchedule::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(AIController->GetPawn());
	if (!NPC) return;

	UWorld* World = NPC->GetWorld();
	if (!World) return;

	UEnvironmentalSystem* EnvSystem = World->GetSubsystem<UEnvironmentalSystem>();
	float CurrentHour = EnvSystem ? EnvSystem->GetTimeOfDay() : 12.0f;

	// Update the NPC's current activity based on schedule
	NPC->UpdateSchedule(CurrentHour);

	// Update Blackboard with activity info
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		FNPCScheduleEntry Entry = NPC->GetCurrentScheduledActivity(CurrentHour);
		BB->SetValueAsName(TEXT("CurrentActivity"), Entry.Activity);
	}
}
