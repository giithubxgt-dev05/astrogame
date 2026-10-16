#include "CosmosHUD.h"
#include "HUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

ACosmosHUD::ACosmosHUD()
{
}

void ACosmosHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UCosmosHUDWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
            HUDWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }

    if (InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport();
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (MapWidgetClass)
    {
        MapWidget = CreateWidget<UUserWidget>(GetWorld(), MapWidgetClass);
        if (MapWidget)
        {
            MapWidget->AddToViewport();
            MapWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (PauseMenuClass)
    {
        PauseMenu = CreateWidget<UUserWidget>(GetWorld(), PauseMenuClass);
        if (PauseMenu)
        {
            PauseMenu->AddToViewport(100); // Higher Z-order for pause menu
            PauseMenu->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void ACosmosHUD::ShowHUD()
{
    if (HUDWidget)
    {
        HUDWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void ACosmosHUD::HideHUD()
{
    if (HUDWidget)
    {
        HUDWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ACosmosHUD::ToggleInventory()
{
    if (InventoryWidget)
    {
        bool bIsVisible = InventoryWidget->GetVisibility() == ESlateVisibility::Visible;
        InventoryWidget->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}

bool ACosmosHUD::IsInventoryOpen() const
{
    return InventoryWidget && InventoryWidget->GetVisibility() == ESlateVisibility::Visible;
}

void ACosmosHUD::ToggleMap()
{
    if (MapWidget)
    {
        bool bIsVisible = MapWidget->GetVisibility() == ESlateVisibility::Visible;
        MapWidget->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}

bool ACosmosHUD::IsMapOpen() const
{
    return MapWidget && MapWidget->GetVisibility() == ESlateVisibility::Visible;
}

void ACosmosHUD::ShowPauseMenu()
{
    if (PauseMenu)
    {
        PauseMenu->SetVisibility(ESlateVisibility::Visible);
    }
}

void ACosmosHUD::HidePauseMenu()
{
    if (PauseMenu)
    {
        PauseMenu->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ACosmosHUD::ShowNotification(FText Message, float Duration)
{
    if (HUDWidget)
    {
        HUDWidget->ShowNotification(Message, Duration);
    }
}

void ACosmosHUD::ShowInteractionPrompt(FText Prompt)
{
    if (HUDWidget)
    {
        HUDWidget->InteractionPromptText = Prompt;
        HUDWidget->bShowInteractionPrompt = true;
    }
}

void ACosmosHUD::HideInteractionPrompt()
{
    if (HUDWidget)
    {
        HUDWidget->bShowInteractionPrompt = false;
    }
}

void ACosmosHUD::UpdateHUDData(USurvivalComponent* SurvivalComp, UPlanetManager* PlanetMgr)
{
    if (HUDWidget)
    {
        HUDWidget->UpdateFromSurvivalComponent(SurvivalComp);
        HUDWidget->UpdateFromPlanetManager(PlanetMgr);
    }
}

void ACosmosHUD::SetShipHUDMode(bool bShipMode)
{
    if (HUDWidget)
    {
        HUDWidget->SetShipMode(bShipMode);
    }
}
