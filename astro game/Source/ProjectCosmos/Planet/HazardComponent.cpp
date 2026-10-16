#include "HazardComponent.h"
#include "PlanetManager.h"
#include "Engine/World.h"

UHazardComponent::UHazardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bHazardsEnabled = true;
	HazardCheckInterval = 1.0f;
}

void UHazardComponent::BeginPlay()
{
	Super::BeginPlay();
	LastHazardCheckTime = GetWorld()->GetTimeSeconds();
}

void UHazardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bHazardsEnabled && (GetWorld()->GetTimeSeconds() - LastHazardCheckTime >= HazardCheckInterval))
	{
		EvaluateHazards();
		LastHazardCheckTime = GetWorld()->GetTimeSeconds();
	}
}

void UHazardComponent::UpdateHazardProtection(EHazardType HazardType, float ProtectionFactor)
{
	ActiveHazards.Add(HazardType, FMath::Clamp(ProtectionFactor, 0.0f, 1.0f));
}

float UHazardComponent::GetCurrentHazardSeverity(EHazardType HazardType) const
{
	if (UWorld* World = GetWorld())
	{
		if (UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>())
		{
			TArray<FPlanetHazard> Hazards = PlanetManager->GetCurrentHazards();
			for (const FPlanetHazard& Hazard : Hazards)
			{
				if (Hazard.HazardType == HazardType)
				{
					float Protection = ActiveHazards.Contains(HazardType) ? ActiveHazards[HazardType] : 0.0f;
					return FMath::Max(0.0f, Hazard.BaseIntensity - Protection);
				}
			}
		}
	}
	return 0.0f;
}

bool UHazardComponent::IsFullyProtected(EHazardType HazardType) const
{
	return GetCurrentHazardSeverity(HazardType) <= 0.0f;
}

TArray<FName> UHazardComponent::EvaluateHazards()
{
	TArray<FName> DamageEvents;
	if (UWorld* World = GetWorld())
	{
		if (UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>())
		{
			TArray<FPlanetHazard> Hazards = PlanetManager->GetCurrentHazards();
			for (const FPlanetHazard& Hazard : Hazards)
			{
				float Severity = GetCurrentHazardSeverity(Hazard.HazardType);
				if (Severity > 0.0f)
				{
					FName DamageEvent = FName(*FString::Printf(TEXT("Damage_Hazard_%d"), (int32)Hazard.HazardType));
					DamageEvents.Add(DamageEvent);
					// Apply damage to owner or trigger gameplay events
				}
			}
		}
	}
	return DamageEvents;
}

TArray<EHazardType> UHazardComponent::GetUnprotectedHazards() const
{
	TArray<EHazardType> Unprotected;
	if (UWorld* World = GetWorld())
	{
		if (UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>())
		{
			TArray<FPlanetHazard> Hazards = PlanetManager->GetCurrentHazards();
			for (const FPlanetHazard& Hazard : Hazards)
			{
				if (GetCurrentHazardSeverity(Hazard.HazardType) > 0.0f)
				{
					Unprotected.Add(Hazard.HazardType);
				}
			}
		}
	}
	return Unprotected;
}
