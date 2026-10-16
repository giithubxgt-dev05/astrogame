#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoordinateSystemLibrary.generated.h"

UENUM(BlueprintType)
enum class EPlanetID : uint8
{
    None,
    Mercury,
    Venus,
    Earth,
    Moon,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune
};

USTRUCT(BlueprintType)
struct FPlanetaryCoord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlanetID PlanetID = EPlanetID::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double LatitudeDeg = 0.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double LongitudeDeg = 0.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double AltitudeKm = 0.0;
};

USTRUCT(BlueprintType)
struct FSolarSystemCoord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector PositionAU = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlanetID ReferencePlanet = EPlanetID::None;
};

UCLASS()
class PROJECTCOSMOS_API UCoordinateSystemLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static FPlanetaryCoord WorldToLatLon(FVector WorldPos, EPlanetID Planet);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static FVector LatLonToWorld(FPlanetaryCoord Coord);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static float GetDistanceBetweenPlanetsAU(EPlanetID A, EPlanetID B);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static FVector GetPlanetPositionAU(EPlanetID Planet);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static float KmToUnrealUnits(float Km);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static float GetGameplayScaleFactor();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Universe")
    static FString FormatCoordinateString(FPlanetaryCoord Coord);
};
