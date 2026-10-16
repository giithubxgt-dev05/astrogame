#pragma once

#include "CoreMinimal.h"
#include "Character/CosmosCharacter.h"
#include "GameplayTagContainer.h"
#include "NPCCharacterBase.generated.h"

UENUM(BlueprintType)
enum class ENPCRole : uint8
{
	Civilian,
	ShopKeeper,
	ScientistResearcher,
	SpaceportWorker,
	SecurityGuard,
	ColonistEngineer,
	Pilot
};

UENUM(BlueprintType)
enum class ENPCFaction : uint8
{
	EarthAuthority,
	FreeTravelers,
	ScienceCorps,
	IndependentTraders,
	Unknown
};

USTRUCT(BlueprintType)
struct FNPCDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LineID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ConditionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLineID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEndsConversation;
};

USTRUCT(BlueprintType)
struct FNPCScheduleEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartHour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndHour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Activity;
};

UCLASS()
class PROJECTCOSMOS_API ANPCCharacterBase : public ACosmosCharacter // Assuming implements ICosmosInteractable
{
	GENERATED_BODY()

public:
	ANPCCharacterBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	FText NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	ENPCRole NPCRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	ENPCFaction NPCFaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	TArray<FNPCDialogueLine> DialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	TArray<FNPCScheduleEntry> DailySchedule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|NPC")
	FName CurrentActivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|NPC")
	FVector HomeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	bool bIsShopkeeper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|NPC")
	TArray<FName> ShopInventory;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|NPC")
	FNPCDialogueLine StartDialogue(ACosmosCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|NPC")
	FNPCDialogueLine AdvanceDialogue(FName ChoiceID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|NPC")
	void EndDialogue();

	UFUNCTION(BlueprintPure, Category = "Cosmos|NPC")
	FNPCScheduleEntry GetCurrentScheduledActivity(float CurrentHour);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|NPC")
	void UpdateSchedule(float CurrentHour);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|NPC")
	bool SellItemToPlayer(FName ItemID, ACosmosCharacter* Player);

	// ICosmosInteractable overrides (assuming virtual in base class)
	UFUNCTION(BlueprintCallable, Category = "Cosmos|Interaction")
	bool IsInteractable() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Interaction")
	FText GetInteractPrompt() const;

private:
	FName CurrentDialogueNode;
};
