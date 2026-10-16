#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CosmosPlayerController.generated.h"

class ACosmosHUD;

UCLASS()
class PROJECTCOSMOS_API ACosmosPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACosmosPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void OnPossess(APawn* aPawn) override;
    virtual void OnUnPossess() override;

public:
    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|UI")
    ACosmosHUD* CosmosHUD;

    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|State")
    bool bInPhotoMode;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ToggleInventory();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ToggleMap();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void TogglePauseMenu();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|State")
    void TogglePhotoMode();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|UI")
    void ShowNotification(FText Message, float Duration);

    UFUNCTION(Client, Reliable, Category = "Cosmos|UI")
    void Client_ShowNotification(const FText& Message);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Input")
    void SetInputMode_GameOnly_Custom();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Input")
    void SetInputMode_UIOnly_Custom();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Input")
    void SetInputMode_GameAndUI_Custom();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Vehicle")
    void HandleShipBoarded();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Vehicle")
    void HandleShipExited();
};
