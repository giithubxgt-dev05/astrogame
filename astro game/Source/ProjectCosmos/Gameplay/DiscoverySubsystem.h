#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/MissionSubsystem.h" // For EPlanetID
#include "DiscoverySubsystem.generated.h"

UENUM(BlueprintType)
enum class EDiscoveryType : uint8
{
	GeologicalFormation,
	MineralDeposit,
	Cave,
	AbandonedFacility,
	ResearchStation,
	AncientStructure,
	RareResource,
	NewLocation,
	FirstLanding
};

USTRUCT(BlueprintType)
struct FDiscoveryRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DiscoveryID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDiscoveryType DiscoveryType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlanetID PlanetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DiscoveryTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DiscoveredByPlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFirstDiscovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScientificValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AssociatedResourceID;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewDiscovery, FDiscoveryRecord, Record);

UCLASS()
class PROJECTCOSMOS_API UDiscoverySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Discovery")
	TArray<FDiscoveryRecord> AllDiscoveries;

	UPROPERTY(BlueprintAssignable, Category = "Cosmos|Discovery")
	FOnNewDiscovery OnNewDiscovery;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Discovery")
	bool RecordDiscovery(FDiscoveryRecord Record);

	UFUNCTION(BlueprintPure, Category = "Cosmos|Discovery")
	TArray<FDiscoveryRecord> GetDiscoveriesForPlanet(EPlanetID PlanetID) const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Discovery")
	TArray<FDiscoveryRecord> GetAllDiscoveries() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Discovery")
	bool HasDiscovered(FName DiscoveryID) const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Discovery")
	int32 GetDiscoveryCount() const;

	UFUNCTION(BlueprintPure, Category = "Cosmos|Discovery")
	FDiscoveryRecord GetFirstLandingRecord(EPlanetID PlanetID) const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|Discovery")
	void RecordFirstLanding(EPlanetID PlanetID, FVector Location);
};
