// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: Research system implementation - technology tree unlock progression

#include "Gameplay/ResearchSubsystem.h"
#include "Player/InventoryComponent.h"
#include "Core/CosmosEventSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UResearchSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultResearchTree();
	UE_LOG(LogTemp, Log, TEXT("[ResearchSubsystem] Initialized with %d research nodes."), ResearchTree.Num());
}

void UResearchSubsystem::InitializeDefaultResearchTree()
{
	// ---- SURVIVAL BRANCH ----
	{
		FResearchNode Node;
		Node.NodeID = TEXT("SURV_BasicOxygen");
		Node.DisplayName = FText::FromString(TEXT("Basic Oxygen Recycling"));
		Node.Description = FText::FromString(TEXT("Improves oxygen efficiency in spacesuits by 25%."));
		Node.Branch = EResearchBranch::Survival;
		Node.ResearchCostCredits = 500.0f;
		Node.ResourceCost.Add(TEXT("Regolith"), 10);
		Node.ResearchTimeSecs = 120.0f;
		Node.UnlockedItems.Add(TEXT("OxygenRecycler_Mk2"));
		AddResearchNode(Node);
	}
	{
		FResearchNode Node;
		Node.NodeID = TEXT("SURV_ThermalInsulation");
		Node.DisplayName = FText::FromString(TEXT("Advanced Thermal Insulation"));
		Node.Description = FText::FromString(TEXT("Allows operation in more extreme temperatures."));
		Node.Branch = EResearchBranch::Survival;
		Node.Prerequisites.Add(TEXT("SURV_BasicOxygen"));
		Node.ResearchCostCredits = 1200.0f;
		Node.ResourceCost.Add(TEXT("Titanium"), 5);
		Node.ResearchTimeSecs = 300.0f;
		Node.UnlockedItems.Add(TEXT("ThermalSuit_Mk2"));
		AddResearchNode(Node);
	}
	{
		FResearchNode Node;
		Node.NodeID = TEXT("SURV_RadiationShielding");
		Node.DisplayName = FText::FromString(TEXT("Radiation Shielding"));
		Node.Description = FText::FromString(TEXT("Enables safe travel to high-radiation environments."));
		Node.Branch = EResearchBranch::Survival;
		Node.Prerequisites.Add(TEXT("SURV_BasicOxygen"));
		Node.ResearchCostCredits = 2000.0f;
		Node.ResourceCost.Add(TEXT("RareMinerals"), 3);
		Node.ResearchTimeSecs = 480.0f;
		Node.UnlockedItems.Add(TEXT("RadiationHelmet_Mk2"));
		AddResearchNode(Node);
	}

	// ---- SPACECRAFT BRANCH ----
	{
		FResearchNode Node;
		Node.NodeID = TEXT("SHIP_FuelEfficiency");
		Node.DisplayName = FText::FromString(TEXT("Fuel Efficiency Optimization"));
		Node.Description = FText::FromString(TEXT("Reduces fuel consumption by 20%."));
		Node.Branch = EResearchBranch::Spacecraft;
		Node.ResearchCostCredits = 1500.0f;
		Node.ResourceCost.Add(TEXT("Iron"), 20);
		Node.ResearchTimeSecs = 240.0f;
		Node.UnlockedShipUpgrades.Add(TEXT("FuelEfficiency_Mk2"));
		AddResearchNode(Node);
	}
	{
		FResearchNode Node;
		Node.NodeID = TEXT("SHIP_HeatShield");
		Node.DisplayName = FText::FromString(TEXT("Advanced Heat Shield"));
		Node.Description = FText::FromString(TEXT("Enables atmospheric entry at higher velocities."));
		Node.Branch = EResearchBranch::Spacecraft;
		Node.Prerequisites.Add(TEXT("SHIP_FuelEfficiency"));
		Node.ResearchCostCredits = 3000.0f;
		Node.ResourceCost.Add(TEXT("Titanium"), 15);
		Node.ResourceCost.Add(TEXT("RareMinerals"), 5);
		Node.ResearchTimeSecs = 600.0f;
		Node.UnlockedShipUpgrades.Add(TEXT("HeatShield_Mk2"));
		AddResearchNode(Node);
	}

	// ---- EXPLORATION BRANCH ----
	{
		FResearchNode Node;
		Node.NodeID = TEXT("EXPL_LongRangeScanner");
		Node.DisplayName = FText::FromString(TEXT("Long-Range Scanner"));
		Node.Description = FText::FromString(TEXT("Reveals resource deposits and POIs within 200km."));
		Node.Branch = EResearchBranch::Exploration;
		Node.ResearchCostCredits = 800.0f;
		Node.ResourceCost.Add(TEXT("Helium3"), 2);
		Node.ResearchTimeSecs = 180.0f;
		Node.UnlockedItems.Add(TEXT("LongRangeScanner"));
		AddResearchNode(Node);
	}

	// ---- SCIENCE BRANCH ----
	{
		FResearchNode Node;
		Node.NodeID = TEXT("SCI_GeologicalAnalysis");
		Node.DisplayName = FText::FromString(TEXT("Geological Analysis"));
		Node.Description = FText::FromString(TEXT("Identify mineral composition of surface samples."));
		Node.Branch = EResearchBranch::Science;
		Node.ResearchCostCredits = 600.0f;
		Node.ResearchTimeSecs = 150.0f;
		Node.UnlockedItems.Add(TEXT("GeologyScanner"));
		AddResearchNode(Node);
	}

	// ---- MINING BRANCH ----
	{
		FResearchNode Node;
		Node.NodeID = TEXT("MINE_AutomatedDrill");
		Node.DisplayName = FText::FromString(TEXT("Automated Drill System"));
		Node.Description = FText::FromString(TEXT("Mine resources 3x faster with automated equipment."));
		Node.Branch = EResearchBranch::Mining;
		Node.Prerequisites.Add(TEXT("SCI_GeologicalAnalysis"));
		Node.ResearchCostCredits = 2500.0f;
		Node.ResourceCost.Add(TEXT("Iron"), 30);
		Node.ResourceCost.Add(TEXT("Coal"), 20);
		Node.ResearchTimeSecs = 420.0f;
		Node.UnlockedItems.Add(TEXT("AutoDrill_Mk1"));
		AddResearchNode(Node);
	}

	// ---- COLONIZATION BRANCH ----
	{
		FResearchNode Node;
		Node.NodeID = TEXT("COL_HabitatDesign");
		Node.DisplayName = FText::FromString(TEXT("Habitat Module Design"));
		Node.Description = FText::FromString(TEXT("Unlocks construction of pressurized habitat modules."));
		Node.Branch = EResearchBranch::Colonization;
		Node.ResearchCostCredits = 5000.0f;
		Node.ResourceCost.Add(TEXT("Titanium"), 25);
		Node.ResourceCost.Add(TEXT("Silicates"), 40);
		Node.ResearchTimeSecs = 900.0f;
		Node.UnlockedItems.Add(TEXT("HabitatModule_Mk1"));
		AddResearchNode(Node);
	}

	UE_LOG(LogTemp, Log, TEXT("[ResearchSubsystem] Loaded %d default research nodes."), ResearchTree.Num());
}

void UResearchSubsystem::AddResearchNode(FResearchNode Node)
{
	ResearchTree.Add(Node.NodeID, Node);
}

bool UResearchSubsystem::CanResearch(FName NodeID) const
{
	if (!ResearchTree.Contains(NodeID)) return false;
	if (IsUnlocked(NodeID)) return false;

	const FResearchNode& Node = ResearchTree[NodeID];
	for (const FName& Prereq : Node.Prerequisites)
	{
		if (!IsUnlocked(Prereq)) return false;
	}
	return true;
}

bool UResearchSubsystem::StartResearch(FName NodeID, UInventoryComponent* PlayerInventory, float& InOutCredits)
{
	if (!CanResearch(NodeID)) return false;
	if (!ResearchTree.Contains(NodeID)) return false;

	const FResearchNode& Node = ResearchTree[NodeID];

	// Check credit cost
	if (InOutCredits < Node.ResearchCostCredits)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ResearchSubsystem] Not enough credits to research %s. Need %.0f, have %.0f"), *NodeID.ToString(), Node.ResearchCostCredits, InOutCredits);
		return false;
	}

	// Check resource costs
	if (PlayerInventory)
	{
		for (const auto& ResourcePair : Node.ResourceCost)
		{
			if (!PlayerInventory->HasItem(ResourcePair.Key, ResourcePair.Value))
			{
				UE_LOG(LogTemp, Warning, TEXT("[ResearchSubsystem] Missing resource %s x%d for research %s"), *ResourcePair.Key.ToString(), ResourcePair.Value, *NodeID.ToString());
				return false;
			}
		}

		// Consume resources
		for (const auto& ResourcePair : Node.ResourceCost)
		{
			PlayerInventory->RemoveItem(ResourcePair.Key, ResourcePair.Value);
		}
	}

	// Consume credits
	InOutCredits -= Node.ResearchCostCredits;

	// Start research
	ActiveResearchNodeID = NodeID;
	ActiveResearchProgress = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("[ResearchSubsystem] Started research: %s"), *NodeID.ToString());
	return true;
}

void UResearchSubsystem::TickResearch(float DeltaTime)
{
	if (ActiveResearchNodeID.IsNone()) return;
	if (!ResearchTree.Contains(ActiveResearchNodeID)) return;

	const FResearchNode& Node = ResearchTree[ActiveResearchNodeID];
	ActiveResearchProgress += DeltaTime;

	OnResearchProgress.Broadcast(ActiveResearchNodeID, ActiveResearchProgress / FMath::Max(Node.ResearchTimeSecs, 1.0f));

	if (ActiveResearchProgress >= Node.ResearchTimeSecs)
	{
		CompleteResearch(ActiveResearchNodeID);
		ActiveResearchNodeID = NAME_None;
		ActiveResearchProgress = 0.0f;
	}
}

void UResearchSubsystem::CompleteResearch(FName NodeID)
{
	if (!ResearchTree.Contains(NodeID)) return;

	FResearchNode& Node = ResearchTree[NodeID];
	Node.bIsUnlocked = true;
	UnlockedNodes.AddUnique(NodeID);

	// Track unlocked items
	for (const FName& Item : Node.UnlockedItems)
	{
		AllUnlockedItems.AddUnique(Item);
	}

	OnResearchCompleted.Broadcast(NodeID);
	UE_LOG(LogTemp, Log, TEXT("[ResearchSubsystem] Completed research: %s"), *NodeID.ToString());
}

bool UResearchSubsystem::IsUnlocked(FName NodeID) const
{
	return UnlockedNodes.Contains(NodeID);
}

TArray<FResearchNode> UResearchSubsystem::GetAvailableResearch() const
{
	TArray<FResearchNode> Available;
	for (const auto& Pair : ResearchTree)
	{
		if (CanResearch(Pair.Key))
		{
			Available.Add(Pair.Value);
		}
	}
	return Available;
}

TArray<FResearchNode> UResearchSubsystem::GetResearchByBranch(EResearchBranch Branch) const
{
	TArray<FResearchNode> Result;
	for (const auto& Pair : ResearchTree)
	{
		if (Pair.Value.Branch == Branch)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

FResearchNode UResearchSubsystem::GetResearchNode(FName NodeID) const
{
	if (ResearchTree.Contains(NodeID))
	{
		return ResearchTree[NodeID];
	}
	return FResearchNode();
}

TArray<FName> UResearchSubsystem::GetUnlockedItems() const
{
	return AllUnlockedItems;
}

bool UResearchSubsystem::HasAbilityUnlocked(FGameplayTag AbilityTag) const
{
	for (const FName& NodeID : UnlockedNodes)
	{
		if (ResearchTree.Contains(NodeID))
		{
			if (ResearchTree[NodeID].UnlockedAbilities.HasTag(AbilityTag))
			{
				return true;
			}
		}
	}
	return false;
}
