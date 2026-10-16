#include "SurvivalComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Dummy declarations
class UPlanetManager : public UWorldSubsystem {
public:
    float GetAtmosphere() const { return 0.0f; } // vacuum
    float GetTemperature() const { return -50.0f; }
    bool HasRadiation() const { return true; }
};

class UHazardComponent : public UActorComponent {
public:
    float GetOxygenProtection() const { return 0.0f; }
    float GetThermalProtection() const { return 0.0f; }
    float GetRadiationProtection() const { return 0.0f; }
};

USurvivalComponent::USurvivalComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TimeSinceLastTick = 0.0f;
}

void USurvivalComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeStats();
}

void USurvivalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bSurvivalEnabled) return;

    TimeSinceLastTick += DeltaTime;
    if (TimeSinceLastTick >= TickInterval)
    {
        ApplyEnvironmentalEffects();
        TimeSinceLastTick = 0.0f;
    }
}

void USurvivalComponent::InitializeStats()
{
    // Oxygen
    FSurvivalStatConfig OxygenConfig;
    OxygenConfig.StatID = ESurvivalStat::Oxygen;
    OxygenConfig.MaxValue = 100.0f;
    OxygenConfig.CurrentValue = 100.0f;
    Stats.Add(ESurvivalStat::Oxygen, OxygenConfig);

    // Temperature
    FSurvivalStatConfig TempConfig;
    TempConfig.StatID = ESurvivalStat::Temperature;
    TempConfig.MaxValue = 100.0f;
    TempConfig.CurrentValue = 37.0f;
    Stats.Add(ESurvivalStat::Temperature, TempConfig);

    // Radiation
    FSurvivalStatConfig RadConfig;
    RadConfig.StatID = ESurvivalStat::Radiation;
    RadConfig.MaxValue = 100.0f;
    RadConfig.CurrentValue = 0.0f; // Accumulates
    Stats.Add(ESurvivalStat::Radiation, RadConfig);

    // Suit Integrity
    FSurvivalStatConfig SuitConfig;
    SuitConfig.StatID = ESurvivalStat::SuitIntegrity;
    SuitConfig.MaxValue = 100.0f;
    SuitConfig.CurrentValue = 100.0f;
    Stats.Add(ESurvivalStat::SuitIntegrity, SuitConfig);

    // Energy
    FSurvivalStatConfig EnergyConfig;
    EnergyConfig.StatID = ESurvivalStat::Energy;
    EnergyConfig.MaxValue = 100.0f;
    EnergyConfig.CurrentValue = 100.0f;
    Stats.Add(ESurvivalStat::Energy, EnergyConfig);
}

void USurvivalComponent::SetStatValue(ESurvivalStat StatID, float Value)
{
    if (Stats.Contains(StatID))
    {
        FSurvivalStatConfig& Config = Stats[StatID];
        Config.CurrentValue = FMath::Clamp(Value, 0.0f, Config.MaxValue);
        OnStatChanged.Broadcast(StatID, Config.CurrentValue);

        if (Config.CurrentValue <= Config.CriticalThreshold && Config.CurrentValue > 0.0f)
        {
            OnStatCritical.Broadcast(StatID);
        }
        else if (Config.CurrentValue <= 0.0f)
        {
            OnStatDepleted.Broadcast(StatID);
            // Broadcast death via EventSubsystem (mock)
            // UEventSubsystem* EventSys = GetWorld()->GetSubsystem<UEventSubsystem>();
            // EventSys->BroadcastPlayerDeath(GetOwner());
        }
    }
}

float USurvivalComponent::GetStatValue(ESurvivalStat StatID) const
{
    if (const FSurvivalStatConfig* Config = Stats.Find(StatID))
    {
        return Config->CurrentValue;
    }
    return 0.0f;
}

float USurvivalComponent::GetStatPercent(ESurvivalStat StatID) const
{
    if (const FSurvivalStatConfig* Config = Stats.Find(StatID))
    {
        if (Config->MaxValue > 0.0f)
        {
            return Config->CurrentValue / Config->MaxValue;
        }
    }
    return 0.0f;
}

void USurvivalComponent::AddOxygen(float Amount)
{
    if (Stats.Contains(ESurvivalStat::Oxygen))
    {
        SetStatValue(ESurvivalStat::Oxygen, GetStatValue(ESurvivalStat::Oxygen) + Amount);
    }
}

void USurvivalComponent::AddEnergy(float Amount)
{
    if (Stats.Contains(ESurvivalStat::Energy))
    {
        SetStatValue(ESurvivalStat::Energy, GetStatValue(ESurvivalStat::Energy) + Amount);
    }
}

void USurvivalComponent::RepairSuit(float Amount)
{
    if (Stats.Contains(ESurvivalStat::SuitIntegrity))
    {
        SetStatValue(ESurvivalStat::SuitIntegrity, GetStatValue(ESurvivalStat::SuitIntegrity) + Amount);
    }
}

void USurvivalComponent::ApplyEnvironmentalEffects()
{
    if (UWorld* World = GetWorld())
    {
        UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>();
        UHazardComponent* HazComp = GetOwner()->FindComponentByClass<UHazardComponent>();

        if (PlanetManager)
        {
            float Atmos = PlanetManager->GetAtmosphere();
            float Temp = PlanetManager->GetTemperature();
            bool bHasRad = PlanetManager->HasRadiation();

            float OxyProt = HazComp ? HazComp->GetOxygenProtection() : 0.0f;
            float ThermProt = HazComp ? HazComp->GetThermalProtection() : 0.0f;
            float RadProt = HazComp ? HazComp->GetRadiationProtection() : 0.0f;

            // Vacuum -> Deplete Oxygen
            if (Atmos < 0.2f && OxyProt < 1.0f)
            {
                float DepleteAmount = OxygenDepletionRate * TickInterval * (1.0f - OxyProt);
                SetStatValue(ESurvivalStat::Oxygen, GetStatValue(ESurvivalStat::Oxygen) - DepleteAmount);
            }

            // Extreme Temp -> Damage Suit
            if ((Temp < -20.0f || Temp > 50.0f) && ThermProt < 1.0f)
            {
                float Damage = 2.0f * TickInterval * (1.0f - ThermProt);
                SetStatValue(ESurvivalStat::SuitIntegrity, GetStatValue(ESurvivalStat::SuitIntegrity) - Damage);
            }

            // Radiation -> Accumulate
            if (bHasRad && RadProt < 1.0f)
            {
                float RadDose = 5.0f * TickInterval * (1.0f - RadProt);
                float CurRad = GetStatValue(ESurvivalStat::Radiation);
                SetStatValue(ESurvivalStat::Radiation, CurRad + RadDose);
                
                if (CurRad > RadiationDamageThreshold)
                {
                    // Starts damaging suit or health
                    SetStatValue(ESurvivalStat::SuitIntegrity, GetStatValue(ESurvivalStat::SuitIntegrity) - 1.0f * TickInterval);
                }
            }
        }
    }
}

bool USurvivalComponent::IsStatCritical(ESurvivalStat StatID) const
{
    if (const FSurvivalStatConfig* Config = Stats.Find(StatID))
    {
        return Config->CurrentValue <= Config->CriticalThreshold;
    }
    return false;
}

TMap<ESurvivalStat, FSurvivalStatConfig> USurvivalComponent::GetAllStats() const
{
    return Stats;
}
