#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivalComponent.generated.h"

UENUM(BlueprintType)
enum class ESurvivalStat : uint8
{
    Oxygen,
    Temperature,
    Radiation,
    SuitIntegrity,
    Energy
};

USTRUCT(BlueprintType)
struct FSurvivalStatConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESurvivalStat StatID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxValue = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentValue = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalThreshold = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DepleteRatePerSec = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RegenRatePerSec = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive = true;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatCriticalSignature, ESurvivalStat, Stat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatDepletedSignature, ESurvivalStat, Stat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedSignature, ESurvivalStat, Stat, float, NewValue);

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API USurvivalComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USurvivalComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Survival")
    TMap<ESurvivalStat, FSurvivalStatConfig> Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Survival")
    bool bSurvivalEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Survival")
    float TickInterval = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Survival")
    float OxygenDepletionRate = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Survival")
    float RadiationDamageThreshold = 50.0f;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Survival")
    void InitializeStats();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Survival")
    void SetStatValue(ESurvivalStat StatID, float Value);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Survival")
    float GetStatValue(ESurvivalStat StatID) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Survival")
    float GetStatPercent(ESurvivalStat StatID) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Survival")
    void AddOxygen(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Survival")
    void AddEnergy(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Survival")
    void RepairSuit(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Survival")
    void ApplyEnvironmentalEffects();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Survival")
    bool IsStatCritical(ESurvivalStat StatID) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Survival")
    TMap<ESurvivalStat, FSurvivalStatConfig> GetAllStats() const;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Survival")
    FOnStatCriticalSignature OnStatCritical;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Survival")
    FOnStatDepletedSignature OnStatDepleted;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Survival")
    FOnStatChangedSignature OnStatChanged;

private:
    float TimeSinceLastTick;
};
