#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionSubsystem.generated.h"

// Assuming EPlanetID is defined elsewhere, if not, placeholder
UENUM(BlueprintType)
enum class EPlanetID : uint8
{
	Earth,
	Moon,
	Mars,
	Unknown
};

UENUM(BlueprintType)
enum class EMissionStatus : uint8
{
	Available,
	Active,
	Completed,
	Failed,
	Hidden
};

UENUM(BlueprintType)
enum class EMissionObjectiveType : uint8
{
	ReachLocation,
	CollectResource,
	TalkToNPC,
	SurviveHazard,
	LandOnPlanet,
	DiscoverLocation,
	BuildModule,
	CraftItem
};

USTRUCT(BlueprintType)
struct FMissionObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMissionObjectiveType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted;
};

USTRUCT(BlueprintType)
struct FMissionDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MissionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMissionStatus Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMissionObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> Rewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CreditReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> PrerequisiteMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> UnlockedByMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GiverNPCID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlanetID TargetPlanetID;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionStatusChanged, FName, MissionID, EMissionStatus, Status);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveProgress, FName, MissionID, FName, ObjectiveID);

UCLASS()
class PROJECTCOSMOS_API UMissionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Missions")
	TMap<FName, FMissionDefinition> AllMissions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Missions")
	TArray<FName> ActiveMissions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Missions")
	TArray<FName> CompletedMissions;

	UPROPERTY(BlueprintAssignable, Category = "Cosmos|Missions")
	FOnMissionStatusChanged OnMissionStatusChanged;

	UPROPERTY(BlueprintAssignable, Category = "Cosmos|Missions")
	FOnObjectiveProgress OnObjectiveProgress;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	bool AcceptMission(FName MissionID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	void AbandonMission(FName MissionID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	void UpdateObjectiveProgress(FName MissionID, FName ObjectiveID, int32 Progress);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	void CheckMissionCompletion(FName MissionID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	void CompleteMission(FName MissionID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	void FailMission(FName MissionID);

	UFUNCTION(BlueprintPure, Category = "Cosmos|Missions")
	TArray<FMissionDefinition> GetActiveMissions() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Missions")
	TArray<FMissionDefinition> GetAvailableMissions(EPlanetID Planet) const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Missions")
	FMissionDefinition GetMission(FName MissionID) const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Missions")
	FMissionDefinition GetPrimaryMission() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Missions")
	void LoadDefaultMissions();
};
