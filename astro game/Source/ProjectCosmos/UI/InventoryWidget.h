#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(BlueprintReadWrite)
    int32 Quantity = 0;
};

UCLASS()
class PROJECTCOSMOS_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void SetInventoryComponent(UInventoryComponent* InInventoryComp);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void RefreshDisplay();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void OnSlotClicked(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void OnSlotHovered(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void UseSelectedItem();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void DropSelectedItem();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cosmos|Inventory")
    FInventorySlot GetSelectedSlot() const;

protected:
    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|Inventory")
    UInventoryComponent* OwnerInventory;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|Inventory")
    int32 SelectedSlotIndex = -1;

    UPROPERTY(BlueprintReadWrite, Category = "Cosmos|Inventory")
    TArray<UUserWidget*> SlotWidgets;
};
