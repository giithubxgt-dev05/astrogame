#include "EnvironmentalSystem.h"
#include "PlanetManager.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UEnvironmentalSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentWeather = EWeatherType::None;
	WeatherIntensity = 0.0f;
	TimeOfDay = 12.0f;
}

void UEnvironmentalSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (!World) return;

	UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>();
	if (PlanetManager)
	{
		UPlanetDefinition* Planet = PlanetManager->GetCurrentPlanet();
		if (Planet && Planet->DayLengthHours > 0)
		{
			// Advance time based on day length. 
			// Assuming 1 real second = some amount of game time. 
			// Let's say 24 real minutes = 24 game hours. 1 real minute = 1 game hour.
			// 60 real seconds = 1 game hour.
			float GameHoursPerRealSecond = 1.0f / 60.0f;
			float DayLengthModifier = 24.0f / Planet->DayLengthHours;
			
			TimeOfDay += (DeltaTime * GameHoursPerRealSecond * DayLengthModifier);
			if (TimeOfDay >= 24.0f)
			{
				TimeOfDay -= 24.0f;
			}
			bIsDaytime = (TimeOfDay >= 6.0f && TimeOfDay <= 18.0f);
		}
	}

	// Lerp weather intensity
	if (!FMath::IsNearlyEqual(WeatherIntensity, TargetWeatherIntensity))
	{
		WeatherIntensity = FMath::FInterpTo(WeatherIntensity, TargetWeatherIntensity, DeltaTime, 0.5f);
		OnWeatherChanged.Broadcast(CurrentWeather, WeatherIntensity);
	}
}

TStatId UEnvironmentalSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEnvironmentalSystem, STATGROUP_Tickables);
}

void UEnvironmentalSystem::SetWeather(EWeatherType InWeather, float Intensity)
{
	CurrentWeather = InWeather;
	TargetWeatherIntensity = Intensity;
	if (Intensity > 0.0f)
	{
		WeatherIntensity = 0.01f; // Kickstart
	}
	OnWeatherChanged.Broadcast(CurrentWeather, WeatherIntensity);
}

float UEnvironmentalSystem::GetWeatherIntensity() const
{
	return WeatherIntensity;
}

float UEnvironmentalSystem::GetTimeOfDay() const
{
	return TimeOfDay;
}

FVector UEnvironmentalSystem::GetSunDirection() const
{
	// Simple sun direction based on TimeOfDay
	// 6 AM = East (Y axis), 12 PM = Up (Z axis), 6 PM = West (-Y axis)
	float NormalizedTime = (TimeOfDay - 6.0f) / 12.0f; // 0 at 6AM, 1 at 6PM
	NormalizedTime = FMath::Clamp(NormalizedTime, 0.0f, 1.0f);
	
	float AngleRad = NormalizedTime * PI;
	return FVector(0.0f, FMath::Cos(AngleRad), FMath::Sin(AngleRad));
}

float UEnvironmentalSystem::GetAmbientTemperatureModifier() const
{
	return bIsDaytime ? 5.0f : -5.0f;
}

void UEnvironmentalSystem::TriggerDustStorm(float Duration, float MaxIntensity)
{
	SetWeather(EWeatherType::DustStorm, MaxIntensity);
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DustStormTimerHandle, this, &UEnvironmentalSystem::OnDustStormFinished, Duration, false);
	}
}

void UEnvironmentalSystem::ClearWeather()
{
	TargetWeatherIntensity = 0.0f;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DustStormTimerHandle);
	}
}

void UEnvironmentalSystem::OnDustStormFinished()
{
	ClearWeather();
}
