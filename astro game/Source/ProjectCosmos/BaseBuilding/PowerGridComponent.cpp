// Copyright 2026 Project Cosmos. All Rights Reserved.
// Module: ProjectCosmos
// Purpose: PowerGrid component implementation - base power distribution

#include "BaseBuilding/PowerGridComponent.h"
#include "Engine/World.h"

UPowerGridComponent::UPowerGridComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 2.0f; // Recalculate every 2 seconds
}

void UPowerGridComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculatePower();
}

void UPowerGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float NetPowerKW = GetNetPowerKW();

	// Charge or drain battery storage
	if (NetPowerKW > 0.0f)
	{
		// Surplus power: charge battery
		float ChargeAmount = NetPowerKW * (DeltaTime / 3600.0f); // KWh
		StoredEnergyKWh = FMath::Clamp(StoredEnergyKWh + ChargeAmount, 0.0f, MaxStorageKWh);
	}
	else if (NetPowerKW < 0.0f && StoredEnergyKWh > 0.0f)
	{
		// Power deficit: draw from battery
		float DrawAmount = FMath::Abs(NetPowerKW) * (DeltaTime / 3600.0f);
		StoredEnergyKWh = FMath::Clamp(StoredEnergyKWh - DrawAmount, 0.0f, MaxStorageKWh);
	}

	EPowerStatus NewStatus = DeterminePowerStatus();
	if (NewStatus != PowerStatus)
	{
		PowerStatus = NewStatus;
		OnPowerStatusChanged.Broadcast(PowerStatus, NetPowerKW);
	}
}

void UPowerGridComponent::RegisterGenerator(FName ModuleID, float GenerationKW)
{
	Generators.Add(ModuleID, GenerationKW);
	RecalculatePower();
}

void UPowerGridComponent::RegisterConsumer(FName ModuleID, float ConsumptionKW)
{
	Consumers.Add(ModuleID, ConsumptionKW);
	RecalculatePower();
}

void UPowerGridComponent::UnregisterModule(FName ModuleID)
{
	Generators.Remove(ModuleID);
	Consumers.Remove(ModuleID);
	RecalculatePower();
}

void UPowerGridComponent::AddBattery(float CapacityKWh)
{
	BatteryCapacityKWh += CapacityKWh;
	MaxStorageKWh = BatteryCapacityKWh;
}

float UPowerGridComponent::GetNetPowerKW() const
{
	return TotalGenerationKW - TotalConsumptionKW;
}

float UPowerGridComponent::GetPowerEfficiencyPercent() const
{
	if (TotalConsumptionKW <= 0.0f) return 100.0f;
	return FMath::Clamp((TotalGenerationKW / TotalConsumptionKW) * 100.0f, 0.0f, 100.0f);
}

bool UPowerGridComponent::IsPowered() const
{
	return PowerStatus != EPowerStatus::Blackout && PowerStatus != EPowerStatus::Offline;
}

EPowerStatus UPowerGridComponent::GetPowerStatus() const
{
	return PowerStatus;
}

FString UPowerGridComponent::GetPowerStatusString() const
{
	switch (PowerStatus)
	{
	case EPowerStatus::Optimal:   return TEXT("Optimal");
	case EPowerStatus::Strained:  return TEXT("Strained");
	case EPowerStatus::Shortage:  return TEXT("Power Shortage");
	case EPowerStatus::Blackout:  return TEXT("Blackout");
	case EPowerStatus::Offline:   return TEXT("Offline");
	default:                      return TEXT("Unknown");
	}
}

void UPowerGridComponent::RecalculatePower()
{
	TotalGenerationKW = 0.0f;
	for (const auto& Gen : Generators)
	{
		TotalGenerationKW += Gen.Value;
	}

	TotalConsumptionKW = 0.0f;
	for (const auto& Con : Consumers)
	{
		TotalConsumptionKW += Con.Value;
	}

	PowerStatus = DeterminePowerStatus();
}

EPowerStatus UPowerGridComponent::DeterminePowerStatus() const
{
	if (TotalGenerationKW <= 0.0f && StoredEnergyKWh <= 0.0f)
	{
		return (TotalConsumptionKW > 0.0f) ? EPowerStatus::Offline : EPowerStatus::Offline;
	}

	float NetKW = GetNetPowerKW();

	if (NetKW >= 0.0f || StoredEnergyKWh > 0.0f)
	{
		float Efficiency = GetPowerEfficiencyPercent();
		if (Efficiency >= 110.0f) return EPowerStatus::Optimal;
		if (Efficiency >= 90.0f) return EPowerStatus::Optimal;
		if (Efficiency >= 70.0f) return EPowerStatus::Strained;
		return EPowerStatus::Shortage;
	}

	return EPowerStatus::Blackout;
}
