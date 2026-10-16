// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: Research system - technology tree and unlock progression

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "ResearchSubsystem.generated.h"

UENUM(BlueprintType)
enum class EResearchBranch : uint8
{
	Spacecraft		UMETA(DisplayName = "Spacecraft"),
	Survival		UMETA(DisplayName = "Survival"),
	Science			UMETA(DisplayName = "Science"),
	Mining			UMETA(DisplayName = "Mining"),
	Exploration		UMETA(DisplayName = "Exploration"),
	Colonization	UMETA(DisplayName = "Colonization"),
	Communication	UMETA(DisplayName = "Communication"),
	AIRobotics		UMETA(DisplayName = "AI & Robotics")
};

USTRUCT(BlueprintType)
struct FResearchNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	FName NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	EResearchBranch Branch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	TArray<FName> Prerequisites;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	float ResearchCostCredits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	TMap<FName, int32> ResourceCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	float ResearchTimeSecs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	TArray<FGameplayTag> UnlockedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	TArray<FName> UnlockedItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cosmos|Research")
	TArray<FName> UnlockedShipUpgrades;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Research")
	bool bIsUnlocked = false;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Research")
	float ResearchProgress = 0.0f; // 0 to ResearchTimeSecs
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResearchCompleted, FName, NodeID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResearchProgress, FName, NodeID, float, Progress);

UCLASS()
class PROJECTCOSMOS_API UResearchSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Research tree data
	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Research")
	TMap<FName, FResearchNode> ResearchTree;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Research")
	TArray<FName> UnlockedNodes;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Research")
	FName ActiveResearchNodeID;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Research")
	float ActiveResearchProgress = 0.0f;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Cosmos|Research")
	FOnResearchCompleted OnResearchCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Cosmos|Research")
	FOnResearchProgress OnResearchProgress;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	bool CanResearch(FName NodeID) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	bool StartResearch(FName NodeID, class UInventoryComponent* PlayerInventory, float& InOutCredits);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	void TickResearch(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	void CompleteResearch(FName NodeID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	bool IsUnlocked(FName NodeID) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	TArray<FResearchNode> GetAvailableResearch() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	TArray<FResearchNode> GetResearchByBranch(EResearchBranch Branch) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	FResearchNode GetResearchNode(FName NodeID) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	TArray<FName> GetUnlockedItems() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Research")
	bool HasAbilityUnlocked(FGameplayTag AbilityTag) const;

private:
	void InitializeDefaultResearchTree();
	void AddResearchNode(FResearchNode Node);
	TArray<FName> AllUnlockedItems;
};
