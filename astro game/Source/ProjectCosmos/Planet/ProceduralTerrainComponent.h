#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlanetDefinition.h"
#include "ProceduralTerrainComponent.generated.h"

USTRUCT(BlueprintType)
struct PROJECTCOSMOS_API FTerrainSampleResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Cosmos|Terrain")
	float Height = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Cosmos|Terrain")
	FName BiomeID;

	UPROPERTY(BlueprintReadOnly, Category="Cosmos|Terrain")
	FName SurfaceMaterial;

	UPROPERTY(BlueprintReadOnly, Category="Cosmos|Terrain")
	FName ResourcePresent;
};

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UProceduralTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProceduralTerrainComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	int32 TerrainSeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	ETerrainStyle TerrainStyle = ETerrainStyle::EarthLike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	float BaseRoughness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	float CraterDensity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	float MountainScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	float ValleyDepth = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|TerrainParams")
	float ResourceDepositDensity = 0.2f;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Terrain")
	void InitializeFromPlanetDefinition(UPlanetDefinition* PlanetDef);

	UFUNCTION(BlueprintPure, Category="Cosmos|Terrain")
	FTerrainSampleResult GetTerrainParamsForLocation(FVector2D LatLon) const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Terrain")
	float GetCraterDataAtLocation(FVector WorldPos) const;
};
