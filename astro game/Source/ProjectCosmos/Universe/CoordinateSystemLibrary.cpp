#include "CoordinateSystemLibrary.h"

FPlanetaryCoord UCoordinateSystemLibrary::WorldToLatLon(FVector WorldPos, EPlanetID Planet)
{
    FPlanetaryCoord Coord;
    Coord.PlanetID = Planet;
    
    // Simplified conversion assuming planet center is at 0,0,0 relative to its own coordinate space
    float Radius = WorldPos.Size();
    Coord.AltitudeKm = (Radius / 100000.0f) - 6371.0f; // Using earth radius as baseline for example

    WorldPos.Normalize();
    Coord.LatitudeDeg = FMath::RadiansToDegrees(FMath::Asin(WorldPos.Z));
    Coord.LongitudeDeg = FMath::RadiansToDegrees(FMath::Atan2(WorldPos.Y, WorldPos.X));
    
    return Coord;
}

FVector UCoordinateSystemLibrary::LatLonToWorld(FPlanetaryCoord Coord)
{
    float Radius = (6371.0f + Coord.AltitudeKm) * 100000.0f; // Base radius + altitude

    float LatRad = FMath::DegreesToRadians(Coord.LatitudeDeg);
    float LonRad = FMath::DegreesToRadians(Coord.LongitudeDeg);

    FVector WorldPos;
    WorldPos.X = Radius * FMath::Cos(LatRad) * FMath::Cos(LonRad);
    WorldPos.Y = Radius * FMath::Cos(LatRad) * FMath::Sin(LonRad);
    WorldPos.Z = Radius * FMath::Sin(LatRad);

    return WorldPos;
}

float UCoordinateSystemLibrary::GetDistanceBetweenPlanetsAU(EPlanetID A, EPlanetID B)
{
    FVector PosA = GetPlanetPositionAU(A);
    FVector PosB = GetPlanetPositionAU(B);
    return FVector::Distance(PosA, PosB);
}

FVector UCoordinateSystemLibrary::GetPlanetPositionAU(EPlanetID Planet)
{
    // Simplified circular orbital data table (semi-major axes in AU)
    float SemiMajorAxisAU = 1.0f;
    float OrbitalAngleRad = 0.0f; // Current angle would depend on game time

    switch (Planet)
    {
        case EPlanetID::Mercury: SemiMajorAxisAU = 0.387f; break;
        case EPlanetID::Venus:   SemiMajorAxisAU = 0.723f; break;
        case EPlanetID::Earth:   SemiMajorAxisAU = 1.0f; break;
        case EPlanetID::Mars:    SemiMajorAxisAU = 1.524f; break;
        case EPlanetID::Jupiter: SemiMajorAxisAU = 5.203f; break;
        case EPlanetID::Saturn:  SemiMajorAxisAU = 9.537f; break;
        case EPlanetID::Uranus:  SemiMajorAxisAU = 19.19f; break;
        case EPlanetID::Neptune: SemiMajorAxisAU = 30.07f; break;
        case EPlanetID::Moon:    return GetPlanetPositionAU(EPlanetID::Earth) + FVector(0.00257f, 0, 0);
        default: return FVector::ZeroVector;
    }

    return FVector(SemiMajorAxisAU * FMath::Cos(OrbitalAngleRad), SemiMajorAxisAU * FMath::Sin(OrbitalAngleRad), 0);
}

float UCoordinateSystemLibrary::KmToUnrealUnits(float Km)
{
    return Km * 100000.0f * GetGameplayScaleFactor();
}

float UCoordinateSystemLibrary::GetGameplayScaleFactor()
{
    return 0.0001f; // 1:10000 scale relative to real distances
}

FString UCoordinateSystemLibrary::FormatCoordinateString(FPlanetaryCoord Coord)
{
    FString LatStr = FString::Printf(TEXT("%.1f°%s"), FMath::Abs(Coord.LatitudeDeg), Coord.LatitudeDeg >= 0 ? TEXT("N") : TEXT("S"));
    FString LonStr = FString::Printf(TEXT("%.1f°%s"), FMath::Abs(Coord.LongitudeDeg), Coord.LongitudeDeg >= 0 ? TEXT("E") : TEXT("W"));
    FString AltStr = FString::Printf(TEXT("%+.1fkm"), Coord.AltitudeKm);

    return FString::Printf(TEXT("%s %s %s"), *LatStr, *LonStr, *AltStr);
}
