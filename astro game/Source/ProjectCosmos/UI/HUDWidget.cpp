#include "HUDWidget.h"
#include "Kismet/KismetMathLibrary.h"

void UCosmosHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    OxygenPercent = 1.0f;
    TemperatureCelsius = 20.0f;
    RadiationLevel = 0.0f;
    SuitIntegrityPercent = 1.0f;
    EnergyPercent = 1.0f;

    CurrentGravityMS2 = 9.81f;
    CurrentPlanetName = FText::FromString("Earth");
    
    CompassHeading = 0.0f;
    CurrentLocationString = TEXT("Unknown Sector");

    bIsShipMode = false;
    bShowInteractionPrompt = false;
    bShowNotification = false;
}

void UCosmosHUDWidget::UpdateFromSurvivalComponent(USurvivalComponent* SurvivalComp)
{
    // Normally we'd cast and read actual values, assuming component functions exist.
    // For now, we simulate fetching data if functions are available.
    // OxygenPercent = SurvivalComp->GetOxygenLevel() / SurvivalComp->GetMaxOxygen();
}

void UCosmosHUDWidget::UpdateFromShip(ACosmosShipBase* Ship)
{
    // CurrentAltitudeKm = Ship->GetAltitudeKm();
    // ShipVelocityKmh = Ship->GetVelocityKmh();
    // ShipFuelPercent = Ship->GetFuelPercent();
}

void UCosmosHUDWidget::UpdateFromPlanetManager(UPlanetManager* PlanetManager)
{
    // CurrentGravityMS2 = PlanetManager->GetCurrentGravity();
    // CurrentPlanetName = PlanetManager->GetCurrentPlanetName();
}

void UCosmosHUDWidget::ShowNotification(FText Message, float Duration)
{
    NotificationText = Message;
    bShowNotification = true;

    // A real implementation would use a timer to hide it, or trigger a blueprint animation.
}

void UCosmosHUDWidget::SetShipMode(bool bEnabled)
{
    bIsShipMode = bEnabled;
}

FLinearColor UCosmosHUDWidget::GetOxygenColor() const
{
    if (OxygenPercent > 0.5f)
    {
        return FLinearColor::Green;
    }
    else if (OxygenPercent > 0.25f)
    {
        float Alpha = (OxygenPercent - 0.25f) / 0.25f;
        return FMath::Lerp(FLinearColor::Yellow, FLinearColor::Green, Alpha);
    }
    else
    {
        float Alpha = OxygenPercent / 0.25f;
        return FMath::Lerp(FLinearColor::Red, FLinearColor::Yellow, Alpha);
    }
}

FLinearColor UCosmosHUDWidget::GetTemperatureColor() const
{
    if (TemperatureCelsius > 40.0f)
    {
        return FLinearColor::Red;
    }
    else if (TemperatureCelsius < -10.0f)
    {
        return FLinearColor::Blue;
    }
    
    return FLinearColor::White;
}
