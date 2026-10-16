#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CosmosHUD.generated.h"

class UCosmosHUDWidget;
class UUserWidget;
class USurvivalComponent;
class UPlanetManager;

UCLASS()
class PROJECTCOSMOS_API ACosmosHUD : public AHUD
{
    GENERATED_BODY()

public:
    ACosmosHUD();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ShowHUD();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void HideHUD();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ToggleInventory();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    bool IsInventoryOpen() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ToggleMap();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    bool IsMapOpen() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ShowPauseMenu();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void HidePauseMenu();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ShowNotification(FText Message, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ShowInteractionPrompt(FText Prompt);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void HideInteractionPrompt();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void UpdateHUDData(USurvivalComponent* SurvivalComp, UPlanetManager* PlanetMgr);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void SetShipHUDMode(bool bShipMode);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Cosmos|UI")
    TSubclassOf<UCosmosHUDWidget> HUDWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|UI")
    UCosmosHUDWidget* HUDWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Cosmos|UI")
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|UI")
    UUserWidget* InventoryWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Cosmos|UI")
    TSubclassOf<UUserWidget> MapWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|UI")
    UUserWidget* MapWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Cosmos|UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|UI")
    UUserWidget* PauseMenu;

    UPROPERTY(EditDefaultsOnly, Category = "Cosmos|UI")
    TSubclassOf<UUserWidget> NotificationWidgetClass;
};
