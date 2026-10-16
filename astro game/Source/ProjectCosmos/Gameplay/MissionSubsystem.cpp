#include "Gameplay/MissionSubsystem.h"

void UMissionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadDefaultMissions();
}

bool UMissionSubsystem::AcceptMission(FName MissionID)
{
	if (AllMissions.Contains(MissionID))
	{
		FMissionDefinition& Mission = AllMissions[MissionID];
		if (Mission.Status == EMissionStatus::Available)
		{
			Mission.Status = EMissionStatus::Active;
			ActiveMissions.AddUnique(MissionID);
			OnMissionStatusChanged.Broadcast(MissionID, EMissionStatus::Active);
			return true;
		}
	}
	return false;
}

void UMissionSubsystem::AbandonMission(FName MissionID)
{
	if (ActiveMissions.Contains(MissionID))
	{
		ActiveMissions.Remove(MissionID);
		if (AllMissions.Contains(MissionID))
		{
			AllMissions[MissionID].Status = EMissionStatus::Available;
			OnMissionStatusChanged.Broadcast(MissionID, EMissionStatus::Available);
		}
	}
}

void UMissionSubsystem::UpdateObjectiveProgress(FName MissionID, FName ObjectiveID, int32 Progress)
{
	if (AllMissions.Contains(MissionID))
	{
		FMissionDefinition& Mission = AllMissions[MissionID];
		for (FMissionObjective& Obj : Mission.Objectives)
		{
			if (Obj.ObjectiveID == ObjectiveID)
			{
				Obj.CurrentCount = FMath::Clamp(Obj.CurrentCount + Progress, 0, Obj.RequiredCount);
				if (Obj.CurrentCount >= Obj.RequiredCount)
				{
					Obj.bCompleted = true;
				}
				OnObjectiveProgress.Broadcast(MissionID, ObjectiveID);
				CheckMissionCompletion(MissionID);
				break;
			}
		}
	}
}

void UMissionSubsystem::CheckMissionCompletion(FName MissionID)
{
	if (AllMissions.Contains(MissionID))
	{
		FMissionDefinition& Mission = AllMissions[MissionID];
		bool bAllComplete = true;
		for (const FMissionObjective& Obj : Mission.Objectives)
		{
			if (!Obj.bCompleted)
			{
				bAllComplete = false;
				break;
			}
		}

		if (bAllComplete)
		{
			CompleteMission(MissionID);
		}
	}
}

void UMissionSubsystem::CompleteMission(FName MissionID)
{
	if (ActiveMissions.Contains(MissionID))
	{
		ActiveMissions.Remove(MissionID);
		CompletedMissions.AddUnique(MissionID);

		if (AllMissions.Contains(MissionID))
		{
			AllMissions[MissionID].Status = EMissionStatus::Completed;
			OnMissionStatusChanged.Broadcast(MissionID, EMissionStatus::Completed);
		}
	}
}

void UMissionSubsystem::FailMission(FName MissionID)
{
	if (ActiveMissions.Contains(MissionID))
	{
		ActiveMissions.Remove(MissionID);

		if (AllMissions.Contains(MissionID))
		{
			AllMissions[MissionID].Status = EMissionStatus::Failed;
			OnMissionStatusChanged.Broadcast(MissionID, EMissionStatus::Failed);
		}
	}
}

TArray<FMissionDefinition> UMissionSubsystem::GetActiveMissions() const
{
	TArray<FMissionDefinition> Result;
	for (FName MissionID : ActiveMissions)
	{
		if (AllMissions.Contains(MissionID))
		{
			Result.Add(AllMissions[MissionID]);
		}
	}
	return Result;
}

TArray<FMissionDefinition> UMissionSubsystem::GetAvailableMissions(EPlanetID Planet) const
{
	TArray<FMissionDefinition> Result;
	for (const auto& Pair : AllMissions)
	{
		if (Pair.Value.Status == EMissionStatus::Available && Pair.Value.TargetPlanetID == Planet)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

FMissionDefinition UMissionSubsystem::GetMission(FName MissionID) const
{
	if (AllMissions.Contains(MissionID))
	{
		return AllMissions[MissionID];
	}
	return FMissionDefinition();
}

FMissionDefinition UMissionSubsystem::GetPrimaryMission() const
{
	// Assuming the first active mission is primary for this stub
	if (ActiveMissions.Num() > 0)
	{
		return GetMission(ActiveMissions[0]);
	}
	return FMissionDefinition();
}

void UMissionSubsystem::LoadDefaultMissions()
{
	FMissionDefinition Mission1;
	Mission1.MissionID = TEXT("COSMOS_INTRO_01");
	Mission1.Title = FText::FromString(TEXT("Talk to Spaceport Director NPC"));
	Mission1.Status = EMissionStatus::Available;
	
	FMissionDefinition Mission2;
	Mission2.MissionID = TEXT("COSMOS_INTRO_02");
	Mission2.Title = FText::FromString(TEXT("Purchase spacesuit from shop"));
	Mission2.Status = EMissionStatus::Hidden;

	FMissionDefinition Mission3;
	Mission3.MissionID = TEXT("COSMOS_INTRO_03");
	Mission3.Title = FText::FromString(TEXT("Board your ship"));
	Mission3.Status = EMissionStatus::Hidden;
	
	FMissionDefinition Mission4;
	Mission4.MissionID = TEXT("COSMOS_INTRO_04");
	Mission4.Title = FText::FromString(TEXT("Launch from Earth Spaceport"));
	Mission4.Status = EMissionStatus::Hidden;
	
	FMissionDefinition Mission5;
	Mission5.MissionID = TEXT("COSMOS_INTRO_05");
	Mission5.Title = FText::FromString(TEXT("Reach Moon orbit"));
	Mission5.Status = EMissionStatus::Hidden;
	
	FMissionDefinition Mission6;
	Mission6.MissionID = TEXT("COSMOS_INTRO_06");
	Mission6.Title = FText::FromString(TEXT("Land on the Moon"));
	Mission6.Status = EMissionStatus::Hidden;
	
	FMissionDefinition Mission7;
	Mission7.MissionID = TEXT("COSMOS_INTRO_07");
	Mission7.Title = FText::FromString(TEXT("Collect 5 units of Regolith"));
	Mission7.Status = EMissionStatus::Hidden;
	FMissionObjective Obj7;
	Obj7.ObjectiveID = TEXT("OBJ_CollectRegolith");
	Obj7.Type = EMissionObjectiveType::CollectResource;
	Obj7.TargetID = TEXT("Item_Regolith");
	Obj7.RequiredCount = 5;
	Obj7.CurrentCount = 0;
	Obj7.bCompleted = false;
	Mission7.Objectives.Add(Obj7);
	
	FMissionDefinition Mission8;
	Mission8.MissionID = TEXT("COSMOS_INTRO_08");
	Mission8.Title = FText::FromString(TEXT("Return to Earth"));
	Mission8.Status = EMissionStatus::Hidden;

	AllMissions.Add(Mission1.MissionID, Mission1);
	AllMissions.Add(Mission2.MissionID, Mission2);
	AllMissions.Add(Mission3.MissionID, Mission3);
	AllMissions.Add(Mission4.MissionID, Mission4);
	AllMissions.Add(Mission5.MissionID, Mission5);
	AllMissions.Add(Mission6.MissionID, Mission6);
	AllMissions.Add(Mission7.MissionID, Mission7);
	AllMissions.Add(Mission8.MissionID, Mission8);
}
