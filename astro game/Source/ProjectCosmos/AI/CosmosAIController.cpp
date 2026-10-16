#include "AI/CosmosAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ACosmosAIController::ACosmosAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void ACosmosAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
	}

	if (PerceptionComponent != nullptr)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACosmosAIController::OnTargetPerceptionUpdated);
	}
}

void ACosmosAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BlackboardData != nullptr && Blackboard != nullptr)
	{
		Blackboard->InitializeBlackboard(*BlackboardData);
		Blackboard->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
		Blackboard->SetValueAsName(TEXT("CurrentJobID"), TEXT("Idle"));
		Blackboard->SetValueAsName(TEXT("CurrentEmotionState"), TEXT("Neutral"));
		Blackboard->SetValueAsBool(TEXT("bIsAlerted"), false);
		Blackboard->SetValueAsBool(TEXT("bCanSeePlayer"), false);
	}
}

void ACosmosAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACosmosAIController::SetBlackboardPatrolTarget(FVector TargetLocation)
{
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsVector(TEXT("PatrolTarget"), TargetLocation);
	}
}

void ACosmosAIController::SetBlackboardPlayerTarget(AActor* TargetActor)
{
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsObject(TEXT("PlayerTarget"), TargetActor);
	}
}

void ACosmosAIController::SetAlertState(bool bIsAlerted)
{
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsBool(TEXT("bIsAlerted"), bIsAlerted);
	}
}

void ACosmosAIController::SetEmotionState(FName Emotion)
{
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsName(TEXT("CurrentEmotionState"), Emotion);
	}
}

float ACosmosAIController::GetDistanceToPlayer() const
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter != nullptr && GetPawn() != nullptr)
	{
		return FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	}
	return -1.0f;
}

void ACosmosAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Actor == PlayerCharacter)
	{
		if (Blackboard != nullptr)
		{
			Blackboard->SetValueAsBool(TEXT("bCanSeePlayer"), Stimulus.WasSuccessfullySensed());
			if (Stimulus.WasSuccessfullySensed())
			{
				SetBlackboardPlayerTarget(Actor);
			}
			else
			{
				SetBlackboardPlayerTarget(nullptr);
			}
		}
	}
}

void ACosmosAIController::UpdatePerception()
{
	if (PerceptionComponent != nullptr)
	{
		PerceptionComponent->RequestStimuliListenerUpdate();
	}
}
