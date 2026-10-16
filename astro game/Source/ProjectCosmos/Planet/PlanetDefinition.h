#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Engine/Scene.h"
#include "PlanetDefinition.generated.h"

UENUM(BlueprintType)
enum class EPlanetID : uint8
{
	Earth,
	Moon,
	Mars,
	Venus,
	Mercury,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	Io,
	Europa,
	Ganymede,
	Callisto,
	Titan,
	Enceladus,
	Unknown
};

UENUM(BlueprintType)
enum class ETerrainStyle : uint8
{
	EarthLike,
	CrateredHighlands,
	DesertPlains,
	VolcanicWasteland,
	GasGiant,
	IcyMoon,
	RockyBarren
};

UENUM(BlueprintType)
enum class EHazardType : uint8
{
	None,
	Vacuum,
	Radiation,
	ExtremeCold,
	ExtremeHeat,
	HighPressure,
	LowPressure,
	DustStorm,
	CorosiveAtmosphere,
	LowGravity
};

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	None,
	Rain,
	Snow,
	Thunderstorm,
	DustStorm,
	Fog,
	BlizzardWinds,
	AcidRain
};

USTRUCT(BlueprintType)
struct PROJECTCOSMOS_API FPlanetHazard
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|PlanetHazard")
	EHazardType HazardType = EHazardType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|PlanetHazard")
	float BaseIntensity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|PlanetHazard")
	bool CanBeProtectedAgainst = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|PlanetHazard")
	FGameplayTag RequiredEquipmentTag;
};

USTRUCT(BlueprintType)
struct PROJECTCOSMOS_API FResourceDeposit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|ResourceDeposit")
	FName ResourceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|ResourceDeposit")
	float AbundanceFactor = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|ResourceDeposit")
	float MinAltitude = -1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|ResourceDeposit")
	float MaxAltitude = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|ResourceDeposit")
	float SlopeMax = 45.0f;
};

UCLASS(BlueprintType)
class PROJECTCOSMOS_API UPlanetDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPlanetDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|PlanetInfo")
	EPlanetID PlanetID = EPlanetID::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|PlanetInfo")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|PlanetInfo")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Physics")
	float GravityMS2 = 9.81f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Atmosphere")
	float AtmosphereDensityKgM3 = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Atmosphere")
	float SurfacePressureKPa = 101.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Temperature")
	float AvgTemperatureCelsius = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Temperature")
	float MinTemperatureCelsius = -20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Temperature")
	float MaxTemperatureCelsius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Orbital")
	float DayLengthHours = 24.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Geometry")
	float RadiusKm = 6371.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Properties")
	bool bHasAtmosphere = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Properties")
	bool bIsGasGiant = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Properties")
	bool bHasWeather = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Properties")
	TArray<EWeatherType> ActiveWeatherTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Hazards")
	TArray<FPlanetHazard> Hazards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Resources")
	TArray<FResourceDeposit> ResourceDeposits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Terrain")
	ETerrainStyle TerrainStyle = ETerrainStyle::EarthLike;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Terrain")
	int32 TerrainSeedOffset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	TSoftObjectPtr<UMaterialInterface> SkyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	TSoftObjectPtr<UTexture2D> ColorGradeLUT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	float AmbientLightIntensity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	FLinearColor SkyLightColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	float SunAngularSize = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	bool bHasRings = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Visuals")
	FPostProcessSettings PostProcessSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Audio")
	TSoftObjectPtr<USoundBase> MusicAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Audio")
	TSoftObjectPtr<USoundBase> AmbientSoundAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Orbital")
	TArray<TSoftObjectPtr<UPlanetDefinition>> ConnectedMoons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cosmos|Level")
	FSoftObjectPath LevelMapPath;

	UFUNCTION(BlueprintPure, Category="Cosmos|PlanetFunctions")
	float GetGravityScale() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|PlanetFunctions")
	bool IsHazardous(EHazardType Hazard) const;
};
