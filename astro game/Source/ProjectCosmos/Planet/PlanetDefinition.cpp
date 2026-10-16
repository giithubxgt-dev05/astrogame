#include "PlanetDefinition.h"

UPlanetDefinition::UPlanetDefinition()
{
}

float UPlanetDefinition::GetGravityScale() const
{
	return GravityMS2 / 9.81f;
}

bool UPlanetDefinition::IsHazardous(EHazardType Hazard) const
{
	for (const FPlanetHazard& PH : Hazards)
	{
		if (PH.HazardType == Hazard)
		{
			return true;
		}
	}
	return false;
}
