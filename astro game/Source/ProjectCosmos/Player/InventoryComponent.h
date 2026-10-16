#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Resource,
    Equipment,
    ShipPart,
    Consumable,
    QuestItem,
    Blueprint
};

USTRUCT(BlueprintType)
struct FItemDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStackSize = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanDrop = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SlotIndex = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEmpty = true;
};

USTRUCT(BlueprintType)
struct FInventoryItemSave
{
    GENERATED_BODY()
    
    UPROPERTY()
    int32 SlotIndex;
    UPROPERTY()
    FName ItemID;
    UPROPERTY()
    int32 Quantity;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedSignature);

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_Slots, EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Inventory")
    TArray<FInventorySlot> Slots;

    UFUNCTION()
    void OnRep_Slots();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Inventory")
    int32 MaxSlots = 40;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Inventory")
    float MaxWeightKg = 50.0f;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    bool AddItem(FName ItemID, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    bool RemoveItem(FName ItemID, int32 Quantity);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Inventory")
    bool HasItem(FName ItemID, int32 Quantity) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Inventory")
    int32 GetItemCount(FName ItemID) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Inventory")
    FInventorySlot GetSlot(int32 Index) const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Inventory")
    TArray<FInventorySlot> GetAllSlots() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Inventory")
    int32 FindItemSlot(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void SwapSlots(int32 A, int32 B);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void DropItem(int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Cosmos|Inventory")
    float GetCurrentWeightKg() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void ClearInventory();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    TArray<FInventoryItemSave> GetSaveData() const;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Inventory")
    void LoadFromSaveData(const TArray<FInventoryItemSave>& SaveData);

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Inventory")
    FOnInventoryChangedSignature OnInventoryChanged;
};
