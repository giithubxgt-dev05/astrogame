#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlanetDefinition.h"
#include "EnvironmentalSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeatherChanged, EWeatherType, WeatherType, float, Intensity);

UCLASS()
class PROJECTCOSMOS_API UEnvironmentalSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	UPROPERTY(BlueprintAssignable, Category="Cosmos|Environment")
	FOnWeatherChanged OnWeatherChanged;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Environment")
	void SetWeather(EWeatherType InWeather, float Intensity);

	UFUNCTION(BlueprintPure, Category="Cosmos|Environment")
	float GetWeatherIntensity() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Environment")
	float GetTimeOfDay() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Environment")
	FVector GetSunDirection() const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Environment")
	float GetAmbientTemperatureModifier() const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Environment")
	void TriggerDustStorm(float Duration, float MaxIntensity);

	UFUNCTION(BlueprintCallable, Category="Cosmos|Environment")
	void ClearWeather();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Environment")
	EWeatherType CurrentWeather = EWeatherType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Environment")
	float WeatherIntensity = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Environment")
	float TimeOfDay = 12.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Environment")
	bool bIsDaytime = true;

private:
	FTimerHandle DustStormTimerHandle;
	float TargetWeatherIntensity = 0.0f;
	void OnDustStormFinished();
};
