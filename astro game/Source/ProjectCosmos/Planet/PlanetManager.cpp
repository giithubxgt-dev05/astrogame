#include "PlanetManager.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UPlanetManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentPlanetID = EPlanetID::Earth;
}

void UPlanetManager::SetCurrentPlanet(EPlanetID NewPlanet)
{
	if (CurrentPlanetID != NewPlanet)
	{
		CurrentPlanetID = NewPlanet;
		
		if (TSoftObjectPtr<UPlanetDefinition>* SoftDefPtr = PlanetDefinitions.Find(NewPlanet))
		{
			LoadedPlanetDef = SoftDefPtr->LoadSynchronous();
		}
		
		ApplyPlanetSettingsToWorld();
		OnPlanetChanged.Broadcast(LoadedPlanetDef);
	}
}

UPlanetDefinition* UPlanetManager::GetCurrentPlanet() const
{
	return LoadedPlanetDef;
}

UPlanetDefinition* UPlanetManager::GetPlanetDefinition(EPlanetID PlanetID) const
{
	if (const TSoftObjectPtr<UPlanetDefinition>* SoftDefPtr = PlanetDefinitions.Find(PlanetID))
	{
		return SoftDefPtr->LoadSynchronous();
	}
	return nullptr;
}

float UPlanetManager::GetCurrentGravityScale() const
{
	if (LoadedPlanetDef)
	{
		return LoadedPlanetDef->GetGravityScale();
	}
	return 1.0f;
}

float UPlanetManager::GetCurrentAtmosphereDensity() const
{
	if (LoadedPlanetDef)
	{
		return LoadedPlanetDef->AtmosphereDensityKgM3;
	}
	return 1.2f;
}

TArray<FPlanetHazard> UPlanetManager::GetCurrentHazards() const
{
	if (LoadedPlanetDef)
	{
		return LoadedPlanetDef->Hazards;
	}
	return TArray<FPlanetHazard>();
}

bool UPlanetManager::IsCurrentPlanetHazardous(EHazardType Hazard) const
{
	if (LoadedPlanetDef)
	{
		return LoadedPlanetDef->IsHazardous(Hazard);
	}
	return false;
}

float UPlanetManager::GetCurrentTemperature(float Altitude) const
{
	if (!LoadedPlanetDef)
	{
		return 15.0f;
	}
	
	// Basic altitude interpolation: lower temperature as we go up. Space temp is around -270C
	float SurfaceTemp = LoadedPlanetDef->AvgTemperatureCelsius;
	float SpaceTemp = -270.0f;
	float AtmosphereThickness = 100000.0f; // 100 km roughly
	
	float Alpha = FMath::Clamp(Altitude / AtmosphereThickness, 0.0f, 1.0f);
	return FMath::Lerp(SurfaceTemp, SpaceTemp, Alpha);
}

void UPlanetManager::ApplyPlanetSettingsToWorld()
{
	if (!LoadedPlanetDef) return;
	
	UWorld* World = GetWorld();
	if (!World) return;
	
	AWorldSettings* WorldSettings = World->GetWorldSettings();
	if (WorldSettings)
	{
		WorldSettings->bGlobalGravitySet = true;
		WorldSettings->GlobalGravityZ = -LoadedPlanetDef->GravityMS2 * 100.0f; // cm/s^2
	}
	
	// TODO: Apply Sky atmosphere, Post Process, etc. via local post process volume or global volume
}

TArray<EPlanetID> UPlanetManager::GetAllPlanetIDs() const
{
	TArray<EPlanetID> IDs;
	PlanetDefinitions.GetKeys(IDs);
	return IDs;
}

FText UPlanetManager::GetPlanetDisplayName(EPlanetID PlanetID) const
{
	UPlanetDefinition* Def = GetPlanetDefinition(PlanetID);
	if (Def)
	{
		return Def->DisplayName;
	}
	return FText::FromString("Unknown");
}
