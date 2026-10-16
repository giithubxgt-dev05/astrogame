#include "BaseManagerSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UBaseManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    BaseCounter = 0;
    ModuleCounter = 0;
}

void UBaseManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

FName UBaseManagerSubsystem::CreateBase(FString Name, EPlanetID PlanetID, FVector Location)
{
    FName NewBaseID = FName(*FString::Printf(TEXT("Base_%d"), ++BaseCounter));

    FBaseData NewBase;
    NewBase.BaseID = NewBaseID;
    NewBase.BaseName = Name;
    NewBase.PlanetID = PlanetID;
    NewBase.TotalPowerGeneration = 0.0f;
    NewBase.TotalPowerConsumption = 0.0f;
    NewBase.bHasLifeSupport = false;

    AllBases.Add(NewBaseID, NewBase);

    return NewBaseID;
}

bool UBaseManagerSubsystem::AddModule(FName BaseID, EBaseModuleType ModuleType, FVector Loc, FRotator Rot)
{
    if (FBaseData* BaseData = AllBases.Find(BaseID))
    {
        FBaseModule NewModule;
        NewModule.ModuleID = FName(*FString::Printf(TEXT("Module_%d"), ++ModuleCounter));
        NewModule.ModuleType = ModuleType;
        NewModule.WorldLocation = Loc;
        NewModule.WorldRotation = Rot;
        NewModule.PlanetID = BaseData->PlanetID;

        // Default power stats
        if (ModuleType == EBaseModuleType::SolarPanel)
        {
            NewModule.PowerGeneration = 50.0f;
        }
        else if (ModuleType == EBaseModuleType::PowerGenerator)
        {
            NewModule.PowerGeneration = 200.0f;
        }
        else if (ModuleType == EBaseModuleType::Habitat)
        {
            NewModule.PowerConsumption = 20.0f;
        }
        else if (ModuleType == EBaseModuleType::OxygenGenerator)
        {
            NewModule.PowerConsumption = 30.0f;
        }

        BaseData->Modules.Add(NewModule);

        UpdatePowerGrid(BaseID);
        OnBaseModuleAdded.Broadcast(BaseID, ModuleType);

        return true;
    }

    return false;
}

bool UBaseManagerSubsystem::RemoveModule(FName BaseID, FName ModuleID)
{
    if (FBaseData* BaseData = AllBases.Find(BaseID))
    {
        int32 Removed = BaseData->Modules.RemoveAll([&](const FBaseModule& M) {
            return M.ModuleID == ModuleID;
        });

        if (Removed > 0)
        {
            UpdatePowerGrid(BaseID);
            return true;
        }
    }
    return false;
}

FBaseData UBaseManagerSubsystem::GetBase(FName BaseID) const
{
    if (const FBaseData* BaseData = AllBases.Find(BaseID))
    {
        return *BaseData;
    }
    return FBaseData();
}

TArray<FBaseData> UBaseManagerSubsystem::GetBasesOnPlanet(EPlanetID PlanetID) const
{
    TArray<FBaseData> Result;
    for (const auto& Pair : AllBases)
    {
        if (Pair.Value.PlanetID == PlanetID)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

void UBaseManagerSubsystem::UpdatePowerGrid(FName BaseID)
{
    if (FBaseData* BaseData = AllBases.Find(BaseID))
    {
        BaseData->TotalPowerGeneration = 0.0f;
        BaseData->TotalPowerConsumption = 0.0f;
        BaseData->bHasLifeSupport = false;
        
        bool bHasOxygenGen = false;

        // Accumulate totals
        for (FBaseModule& Module : BaseData->Modules)
        {
            BaseData->TotalPowerGeneration += Module.PowerGeneration;
            BaseData->TotalPowerConsumption += Module.PowerConsumption;
        }

        bool bGridPowered = BaseData->TotalPowerGeneration >= BaseData->TotalPowerConsumption;

        // Apply power state
        for (FBaseModule& Module : BaseData->Modules)
        {
            Module.bIsPowered = bGridPowered || (Module.PowerGeneration > 0);
            Module.bIsOperational = Module.bIsPowered;

            if (Module.bIsOperational && Module.ModuleType == EBaseModuleType::OxygenGenerator)
            {
                bHasOxygenGen = true;
            }
        }

        BaseData->bHasLifeSupport = bHasOxygenGen && bGridPowered;

        float NetPower = BaseData->TotalPowerGeneration - BaseData->TotalPowerConsumption;
        OnBasePowerChanged.Broadcast(BaseID, NetPower);
    }
}

bool UBaseManagerSubsystem::IsBaseLifeSupported(FName BaseID) const
{
    if (const FBaseData* BaseData = AllBases.Find(BaseID))
    {
        return BaseData->bHasLifeSupport;
    }
    return false;
}

FBaseData UBaseManagerSubsystem::GetNearestBase(FVector WorldPos) const
{
    FBaseData NearestBase;
    float MinDistSq = MAX_FLT;

    for (const auto& Pair : AllBases)
    {
        if (Pair.Value.Modules.Num() > 0)
        {
            float DistSq = FVector::DistSquared(Pair.Value.Modules[0].WorldLocation, WorldPos);
            if (DistSq < MinDistSq)
            {
                MinDistSq = DistSq;
                NearestBase = Pair.Value;
            }
        }
    }

    return NearestBase;
}
