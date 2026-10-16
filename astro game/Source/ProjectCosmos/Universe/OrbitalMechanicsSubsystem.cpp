#include "OrbitalMechanicsSubsystem.h"

void UOrbitalMechanicsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    GameTimeSeconds = 0.0;

    // Base: Earth year = 3600 seconds
    float EarthYear = 3600.0f;
    OrbitalPeriods.Add(EPlanetID::Mercury, EarthYear * 0.24f);
    OrbitalPeriods.Add(EPlanetID::Venus, EarthYear * 0.615f);
    OrbitalPeriods.Add(EPlanetID::Earth, EarthYear);
    OrbitalPeriods.Add(EPlanetID::Mars, EarthYear * 1.88f);
    OrbitalPeriods.Add(EPlanetID::Jupiter, EarthYear * 11.86f);
    OrbitalPeriods.Add(EPlanetID::Saturn, EarthYear * 29.46f);
    OrbitalPeriods.Add(EPlanetID::Uranus, EarthYear * 84.01f);
    OrbitalPeriods.Add(EPlanetID::Neptune, EarthYear * 164.79f);
    OrbitalPeriods.Add(EPlanetID::Moon, EarthYear * 0.074f); // relative to Earth
}

bool UOrbitalMechanicsSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UOrbitalMechanicsSubsystem::Tick(float DeltaTime)
{
    AdvanceTime(DeltaTime);
}

void UOrbitalMechanicsSubsystem::AdvanceTime(float DeltaSeconds)
{
    GameTimeSeconds += DeltaSeconds;
}

float UOrbitalMechanicsSubsystem::GetPlanetAngleDeg(EPlanetID Planet) const
{
    if (const float* Period = OrbitalPeriods.Find(Planet))
    {
        if (*Period > 0.0f)
        {
            float Fraction = FMath::Fmod(GameTimeSeconds, *Period) / *Period;
            return Fraction * 360.0f;
        }
    }
    return 0.0f;
}

FVector UOrbitalMechanicsSubsystem::GetPlanetPosition(EPlanetID Planet) const
{
    float AngleDeg = GetPlanetAngleDeg(Planet);
    float AngleRad = FMath::DegreesToRadians(AngleDeg);

    float SemiMajorAxisAU = 1.0f;
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
        case EPlanetID::Moon:    
            // Simplification: Moon orbits Earth
            return GetPlanetPosition(EPlanetID::Earth) + FVector(0.00257f * FMath::Cos(AngleRad), 0.00257f * FMath::Sin(AngleRad), 0);
        default: return FVector::ZeroVector;
    }

    return FVector(SemiMajorAxisAU * FMath::Cos(AngleRad), SemiMajorAxisAU * FMath::Sin(AngleRad), 0);
}

float UOrbitalMechanicsSubsystem::GetRelativeAngleBetween(EPlanetID A, EPlanetID B) const
{
    float AngleA = GetPlanetAngleDeg(A);
    float AngleB = GetPlanetAngleDeg(B);
    
    float Diff = FMath::Fmod(FMath::Abs(AngleA - AngleB), 360.0f);
    if (Diff > 180.0f)
    {
        Diff = 360.0f - Diff;
    }
    return Diff;
}

FVector UOrbitalMechanicsSubsystem::GetSunDirectionFromPlanet(EPlanetID Planet, double InGameTime) const
{
    // Simplified since sun is at 0,0,0
    FVector PlanetPos = GetPlanetPosition(Planet);
    return -PlanetPos.GetSafeNormal();
}
