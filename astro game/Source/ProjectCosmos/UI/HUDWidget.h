#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class USurvivalComponent;
class ACosmosShipBase;
class UPlanetManager;

UCLASS()
class PROJECTCOSMOS_API UCosmosHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void UpdateFromSurvivalComponent(USurvivalComponent* SurvivalComp);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void UpdateFromShip(ACosmosShipBase* Ship);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void UpdateFromPlanetManager(UPlanetManager* PlanetManager);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ShowNotification(FText Message, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void SetShipMode(bool bEnabled);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cosmos|UI")
    FLinearColor GetOxygenColor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cosmos|UI")
    FLinearColor GetTemperatureColor() const;

    // Survival Properties
    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Survival")
    float OxygenPercent;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Survival")
    float TemperatureCelsius;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Survival")
    float RadiationLevel;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Survival")
    float SuitIntegrityPercent;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Survival")
    float EnergyPercent;

    // Environment Properties
    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Environment")
    float CurrentGravityMS2;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Environment")
    FText CurrentPlanetName;

    // Navigation Properties
    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Navigation")
    float CompassHeading;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Navigation")
    FString CurrentLocationString;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Navigation")
    FText CurrentMissionText;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Navigation")
    FText CurrentObjectiveText;

    // Ship Properties
    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Ship")
    float CurrentAltitudeKm;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Ship")
    float ShipVelocityKmh;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Ship")
    float ShipFuelPercent;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Ship")
    bool bIsShipMode;

    // Interaction & Notification
    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Interaction")
    FText InteractionPromptText;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Interaction")
    bool bShowInteractionPrompt;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Notification")
    FText NotificationText;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|UI|Notification")
    bool bShowNotification;
};
