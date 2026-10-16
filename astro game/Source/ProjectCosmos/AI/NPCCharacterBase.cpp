#include "AI/NPCCharacterBase.h"
#include "AI/CosmosAIController.h"

ANPCCharacterBase::ANPCCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsShopkeeper = false;
	NPCFaction = ENPCFaction::Unknown;
	NPCRole = ENPCRole::Civilian;
}

void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	HomeLocation = GetActorLocation();
}

void ANPCCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FNPCDialogueLine ANPCCharacterBase::StartDialogue(ACosmosCharacter* Player)
{
	for (const FNPCDialogueLine& Line : DialogueLines)
	{
		// In a full implementation, you'd check tags against player state
		if (Line.ConditionTag.IsValid())
		{
			// Example placeholder tag check logic
		}
		CurrentDialogueNode = Line.LineID;
		return Line;
	}

	FNPCDialogueLine EmptyLine;
	EmptyLine.bEndsConversation = true;
	return EmptyLine;
}

FNPCDialogueLine ANPCCharacterBase::AdvanceDialogue(FName ChoiceID)
{
	for (const FNPCDialogueLine& Line : DialogueLines)
	{
		if (Line.LineID == ChoiceID)
		{
			CurrentDialogueNode = Line.LineID;
			return Line;
		}
	}

	FNPCDialogueLine EmptyLine;
	EmptyLine.bEndsConversation = true;
	return EmptyLine;
}

void ANPCCharacterBase::EndDialogue()
{
	CurrentDialogueNode = NAME_None;
}

FNPCScheduleEntry ANPCCharacterBase::GetCurrentScheduledActivity(float CurrentHour)
{
	for (const FNPCScheduleEntry& Entry : DailySchedule)
	{
		if (CurrentHour >= Entry.StartHour && CurrentHour < Entry.EndHour)
		{
			return Entry;
		}
	}

	FNPCScheduleEntry DefaultEntry;
	DefaultEntry.Activity = TEXT("Idle");
	DefaultEntry.Location = TEXT("Home");
	return DefaultEntry;
}

void ANPCCharacterBase::UpdateSchedule(float CurrentHour)
{
	FNPCScheduleEntry Entry = GetCurrentScheduledActivity(CurrentHour);
	CurrentActivity = Entry.Activity;

	ACosmosAIController* AIController = Cast<ACosmosAIController>(GetController());
	if (AIController != nullptr)
	{
		// Very simplified logic: moving to home location for the demo
		if (Entry.Location == TEXT("Home"))
		{
			AIController->MoveToLocation(HomeLocation);
		}
		else
		{
			// Resolve named locations via subsystem
			// AIController->MoveToLocation(ResolvedLocation);
		}
	}
}

bool ANPCCharacterBase::SellItemToPlayer(FName ItemID, ACosmosCharacter* Player)
{
	if (!bIsShopkeeper) return false;

	if (ShopInventory.Contains(ItemID))
	{
		// Process sale via EconomySubsystem
		return true;
	}
	return false;
}

bool ANPCCharacterBase::IsInteractable() const
{
	return true;
}

FText ANPCCharacterBase::GetInteractPrompt() const
{
	return FText::Format(FText::FromString("Talk to {0}"), NPCName);
}
