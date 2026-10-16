#include "ShipUpgradeComponent.h"
// #include "InventoryComponent.h" // Expected to exist

UShipUpgradeComponent::UShipUpgradeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UShipUpgradeComponent::CanUpgrade(FName UpgradeID, UInventoryComponent* PlayerInventory) const
{
    // Need Inventory implementation to check resources.
    // For now, simple credit check simulator.
    return true; 
}

bool UShipUpgradeComponent::PerformUpgrade(FName UpgradeID, UInventoryComponent* PlayerInventory, float& InOutCredits)
{
    for (FUpgradeDefinition& Def : UpgradeDefinitions)
    {
        if (Def.UpgradeID == UpgradeID)
        {
            if (Def.Level < Def.MaxLevel && InOutCredits >= Def.CreditCost)
            {
                // Consume resources and credits
                InOutCredits -= Def.CreditCost;
                Def.Level++;
                
                // Update internal state
                if (UpgradeID == "Engine") CurrentUpgrades.EngineLevel = Def.Level;
                else if (UpgradeID == "Fuel") CurrentUpgrades.FuelLevel = Def.Level;
                else if (UpgradeID == "Shield") CurrentUpgrades.ShieldLevel = Def.Level;
                else if (UpgradeID == "Cargo") CurrentUpgrades.CargoLevel = Def.Level;
                else if (UpgradeID == "Sensor") CurrentUpgrades.SensorLevel = Def.Level;
                else if (UpgradeID == "LifeSupport") CurrentUpgrades.LifeSupportLevel = Def.Level;
                else if (UpgradeID == "HeatShield") CurrentUpgrades.HeatShieldLevel = Def.Level;
                
                return true;
            }
        }
    }
    return false;
}

void UShipUpgradeComponent::ApplyUpgradesToShipStats(FShipStats& Stats)
{
    // Apply bonuses. Assuming linear 10% bonus per level above 1
    Stats.ThrustKN *= (1.0f + (CurrentUpgrades.EngineLevel - 1) * 0.1f);
    Stats.MaxSpeedVacuum *= (1.0f + (CurrentUpgrades.EngineLevel - 1) * 0.1f);
    
    Stats.MaxFuelKg *= (1.0f + (CurrentUpgrades.FuelLevel - 1) * 0.2f);
    Stats.ShieldStrength *= (1.0f + (CurrentUpgrades.ShieldLevel - 1) * 0.15f);
    Stats.MaxCargo *= (1.0f + (CurrentUpgrades.CargoLevel - 1) * 0.25f);
    Stats.SensorRange *= (1.0f + (CurrentUpgrades.SensorLevel - 1) * 0.2f);
    Stats.LifeSupportCapacity += (CurrentUpgrades.LifeSupportLevel - 1) * 2;
    Stats.HeatShieldRating *= (1.0f + (CurrentUpgrades.HeatShieldLevel - 1) * 0.1f);
}

int32 UShipUpgradeComponent::GetUpgradeLevel(FName UpgradeID) const
{
    for (const FUpgradeDefinition& Def : UpgradeDefinitions)
    {
        if (Def.UpgradeID == UpgradeID)
        {
            return Def.Level;
        }
    }
    return 1;
}

float UShipUpgradeComponent::GetUpgradeCost(FName UpgradeID) const
{
    for (const FUpgradeDefinition& Def : UpgradeDefinitions)
    {
        if (Def.UpgradeID == UpgradeID)
        {
            return Def.CreditCost * Def.Level; // Cost scales with level
        }
    }
    return 0.0f;
}

TArray<FUpgradeDefinition> UShipUpgradeComponent::GetAllUpgrades() const
{
    return UpgradeDefinitions;
}
