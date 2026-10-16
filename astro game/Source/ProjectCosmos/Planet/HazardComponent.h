#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlanetDefinition.h"
#include "HazardComponent.generated.h"

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UHazardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHazardComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|Hazards")
	bool bHazardsEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cosmos|Hazards")
	float HazardCheckInterval = 1.0f;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Hazards")
	void UpdateHazardProtection(EHazardType HazardType, float ProtectionFactor);

	UFUNCTION(BlueprintPure, Category="Cosmos|Hazards")
	float GetCurrentHazardSeverity(EHazardType HazardType) const;

	UFUNCTION(BlueprintPure, Category="Cosmos|Hazards")
	bool IsFullyProtected(EHazardType HazardType) const;

	UFUNCTION(BlueprintCallable, Category="Cosmos|Hazards")
	TArray<FName> EvaluateHazards();

	UFUNCTION(BlueprintPure, Category="Cosmos|Hazards")
	TArray<EHazardType> GetUnprotectedHazards() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Hazards")
	float LastHazardCheckTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cosmos|Hazards")
	TMap<EHazardType, float> ActiveHazards;
};
