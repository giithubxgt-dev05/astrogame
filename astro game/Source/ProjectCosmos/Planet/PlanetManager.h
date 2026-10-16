#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlanetDefinition.h"
#include "PlanetManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlanetChanged, UPlanetDefinition*, NewPlanetDef);

UCLASS()
class PROJECTCOSMOS_API UPlanetManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Planet")
	FOnPlanetChanged OnPlanetChanged;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Planet")
	void SetCurrentPlanet(EPlanetID NewPlanet);

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	UPlanetDefinition* GetCurrentPlanet() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	UPlanetDefinition* GetPlanetDefinition(EPlanetID PlanetID) const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	float GetCurrentGravityScale() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	float GetCurrentAtmosphereDensity() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	TArray<FPlanetHazard> GetCurrentHazards() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	bool IsCurrentPlanetHazardous(EHazardType Hazard) const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	float GetCurrentTemperature(float Altitude) const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Planet")
	void ApplyPlanetSettingsToWorld();

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	TArray<EPlanetID> GetAllPlanetIDs() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Planet")
	FText GetPlanetDisplayName(EPlanetID PlanetID) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|Planet")
	TMap<EPlanetID, TSoftObjectPtr<UPlanetDefinition>> PlanetDefinitions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Planet")
	EPlanetID CurrentPlanetID = EPlanetID::Unknown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Planet")
	UPlanetDefinition* LoadedPlanetDef = nullptr;
};
