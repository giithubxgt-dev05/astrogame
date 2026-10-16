#include "CosmosPlayerController.h"
#include "GameFramework/HUD.h"
#include "Engine/World.h"

// Mock HUD class
class ACosmosHUD : public AHUD {};

ACosmosPlayerController::ACosmosPlayerController()
{
    bInPhotoMode = false;
}

void ACosmosPlayerController::BeginPlay()
{
    Super::BeginPlay();
    CosmosHUD = Cast<ACosmosHUD>(GetHUD());
}

void ACosmosPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    if (InputComponent)
    {
        InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ACosmosPlayerController::ToggleInventory);
        InputComponent->BindAction("ToggleMap", IE_Pressed, this, &ACosmosPlayerController::ToggleMap);
        InputComponent->BindAction("TogglePauseMenu", IE_Pressed, this, &ACosmosPlayerController::TogglePauseMenu);
        InputComponent->BindAction("TogglePhotoMode", IE_Pressed, this, &ACosmosPlayerController::TogglePhotoMode);
    }
}

void ACosmosPlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);
}

void ACosmosPlayerController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ACosmosPlayerController::ToggleInventory()
{
    // Implementation for UI toggle
}

void ACosmosPlayerController::ToggleMap()
{
    // Implementation for map toggle
}

void ACosmosPlayerController::TogglePauseMenu()
{
    // Implementation for pause menu
}

void ACosmosPlayerController::TogglePhotoMode()
{
    bInPhotoMode = !bInPhotoMode;
    if (bInPhotoMode)
    {
        SetInputMode_UIOnly_Custom();
    }
    else
    {
        SetInputMode_GameOnly_Custom();
    }
}

void ACosmosPlayerController::ShowNotification(FText Message, float Duration)
{
    if (HasAuthority())
    {
        Client_ShowNotification(Message);
    }
    else
    {
        Client_ShowNotification(Message);
    }
}

void ACosmosPlayerController::Client_ShowNotification_Implementation(const FText& Message)
{
    // Update HUD with notification
}

void ACosmosPlayerController::SetInputMode_GameOnly_Custom()
{
    FInputModeGameOnly Mode;
    SetInputMode(Mode);
    bShowMouseCursor = false;
}

void ACosmosPlayerController::SetInputMode_UIOnly_Custom()
{
    FInputModeUIOnly Mode;
    SetInputMode(Mode);
    bShowMouseCursor = true;
}

void ACosmosPlayerController::SetInputMode_GameAndUI_Custom()
{
    FInputModeGameAndUI Mode;
    SetInputMode(Mode);
    bShowMouseCursor = true;
}

void ACosmosPlayerController::HandleShipBoarded()
{
    // Switch mappings, update UI
}

void ACosmosPlayerController::HandleShipExited()
{
    // Revert mappings, update UI
}
