// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: PowerGrid component - manages base power distribution

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerGridComponent.generated.h"

UENUM(BlueprintType)
enum class EPowerStatus : uint8
{
	Optimal		UMETA(DisplayName = "Optimal"),
	Strained	UMETA(DisplayName = "Strained"),
	Shortage	UMETA(DisplayName = "Power Shortage"),
	Blackout	UMETA(DisplayName = "Blackout"),
	Offline		UMETA(DisplayName = "Offline")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerStatusChanged, EPowerStatus, NewStatus, float, NetPowerKW);

UCLASS(ClassGroup = (ProjectCosmos), meta = (BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UPowerGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPowerGridComponent();

	UPROPERTY(BlueprintAssignable, Category = "Cosmos|BaseBuilding")
	FOnPowerStatusChanged OnPowerStatusChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
	float TotalGenerationKW = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
	float TotalConsumptionKW = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
	float StoredEnergyKWh = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
	float MaxStorageKWh = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
	EPowerStatus PowerStatus = EPowerStatus::Offline;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	void RegisterGenerator(FName ModuleID, float GenerationKW);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	void RegisterConsumer(FName ModuleID, float ConsumptionKW);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	void UnregisterModule(FName ModuleID);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	void AddBattery(float CapacityKWh);

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	float GetNetPowerKW() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	float GetPowerEfficiencyPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	bool IsPowered() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	EPowerStatus GetPowerStatus() const;

	UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
	FString GetPowerStatusString() const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TMap<FName, float> Generators;   // ModuleID -> KW generated
	TMap<FName, float> Consumers;    // ModuleID -> KW consumed
	float BatteryCapacityKWh = 0.0f;

	void RecalculatePower();
	EPowerStatus DeterminePowerStatus() const;
};
