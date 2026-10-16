#include "ProceduralTerrainComponent.h"
#include "Math/UnrealMathUtility.h"

UProceduralTerrainComponent::UProceduralTerrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	TerrainSeed = 12345;
	BaseRoughness = 1.0f;
	CraterDensity = 0.0f;
	MountainScale = 1.0f;
	ValleyDepth = 1.0f;
	ResourceDepositDensity = 0.1f;
}

void UProceduralTerrainComponent::InitializeFromPlanetDefinition(UPlanetDefinition* PlanetDef)
{
	if (PlanetDef)
	{
		TerrainSeed = PlanetDef->TerrainSeedOffset;
		TerrainStyle = PlanetDef->TerrainStyle;

		switch (TerrainStyle)
		{
		case ETerrainStyle::CrateredHighlands:
			CraterDensity = 1.0f;
			MountainScale = 0.5f;
			ValleyDepth = 0.2f;
			BaseRoughness = 1.5f;
			break;
		case ETerrainStyle::DesertPlains:
			CraterDensity = 0.1f;
			MountainScale = 0.2f;
			ValleyDepth = 0.1f;
			BaseRoughness = 0.5f;
			break;
		case ETerrainStyle::VolcanicWasteland:
			CraterDensity = 0.3f;
			MountainScale = 2.0f;
			ValleyDepth = 1.5f;
			BaseRoughness = 2.0f;
			break;
		case ETerrainStyle::IcyMoon:
			CraterDensity = 0.4f;
			MountainScale = 0.3f;
			ValleyDepth = 0.8f;
			BaseRoughness = 0.7f;
			break;
		default:
			CraterDensity = 0.2f;
			MountainScale = 1.0f;
			ValleyDepth = 1.0f;
			BaseRoughness = 1.0f;
			break;
		}
	}
}

FTerrainSampleResult UProceduralTerrainComponent::GetTerrainParamsForLocation(FVector2D LatLon) const
{
	FTerrainSampleResult Result;
	
	// Multi-octave noise function simulation
	float NoiseVal = FMath::PerlinNoise2D(LatLon * 0.01f + FVector2D(TerrainSeed, TerrainSeed));
	float DetailNoise = FMath::PerlinNoise2D(LatLon * 0.1f + FVector2D(TerrainSeed * 2.0f, TerrainSeed * 2.0f));
	
	float Height = (NoiseVal * MountainScale * 1000.0f) + (DetailNoise * BaseRoughness * 100.0f);
	if (Height < 0)
	{
		Height *= ValleyDepth;
	}
	
	Result.Height = Height;
	Result.BiomeID = (Height > 500.0f) ? FName("Mountains") : FName("Plains");
	Result.SurfaceMaterial = (Height > 800.0f) ? FName("Rock") : FName("Dirt");
	
	// Simple resource presence check based on noise
	float ResourceNoise = FMath::PerlinNoise2D(LatLon * 0.5f - FVector2D(TerrainSeed, TerrainSeed));
	if (ResourceNoise > (1.0f - ResourceDepositDensity))
	{
		Result.ResourcePresent = FName("Iron");
	}
	else
	{
		Result.ResourcePresent = NAME_None;
	}

	return Result;
}

float UProceduralTerrainComponent::GetCraterDataAtLocation(FVector WorldPos) const
{
	if (CraterDensity <= 0.0f) return 0.0f;

	// SDF crater function simulation
	// Determine cell based on WorldPos scaled by crater density
	FVector ScaledPos = WorldPos * 0.001f * CraterDensity;
	FVector CellCenter = FVector(FMath::RoundToFloat(ScaledPos.X), FMath::RoundToFloat(ScaledPos.Y), 0.0f);
	
	// Use pseudo-randomness for this cell to decide if a crater is here
	float CellSeed = (CellCenter.X * 1337.0f) + (CellCenter.Y * 7331.0f) + TerrainSeed;
	FMath::RandInit((int32)CellSeed);
	
	if (FMath::FRand() < 0.3f) // 30% chance for crater in cell
	{
		float DistanceToCenter = FVector::Dist2D(ScaledPos, CellCenter);
		float CraterRadius = 0.4f + FMath::FRand() * 0.4f; // 0.4 to 0.8
		
		if (DistanceToCenter < CraterRadius)
		{
			// Normalized distance from center (0 = center, 1 = edge)
			float NormalizedDist = DistanceToCenter / CraterRadius;
			
			// Simple bowl shape: 1 at center (deepest), 0 at edge
			return 1.0f - (NormalizedDist * NormalizedDist);
		}
	}
	
	return 0.0f; // flat
}
