#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner()->HasAuthority())
    {
        Slots.SetNum(MaxSlots);
        for (int32 i = 0; i < MaxSlots; ++i)
        {
            Slots[i].SlotIndex = i;
            Slots[i].bIsEmpty = true;
        }
    }
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Slots);
}

void UInventoryComponent::OnRep_Slots()
{
    OnInventoryChanged.Broadcast();
}

bool UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
    if (Quantity <= 0) return false;
    if (!GetOwner()->HasAuthority()) return false;

    // Weight check should go here if we had item definitions database lookup
    // Assuming an arbitrary database lookup function for ItemDef:
    // FItemDefinition ItemDef = GetItemDatabase()->GetItemDef(ItemID);

    int32 RemainingToAdd = Quantity;
    int32 MaxStack = 99; // Mock max stack size

    // Try stacking
    for (int32 i = 0; i < MaxSlots; ++i)
    {
        if (!Slots[i].bIsEmpty && Slots[i].ItemID == ItemID && Slots[i].Quantity < MaxStack)
        {
            int32 SpaceLeft = MaxStack - Slots[i].Quantity;
            int32 AddAmount = FMath::Min(RemainingToAdd, SpaceLeft);
            
            Slots[i].Quantity += AddAmount;
            RemainingToAdd -= AddAmount;
            
            if (RemainingToAdd <= 0)
            {
                OnInventoryChanged.Broadcast();
                return true;
            }
        }
    }

    // Try new slot
    for (int32 i = 0; i < MaxSlots; ++i)
    {
        if (Slots[i].bIsEmpty)
        {
            int32 AddAmount = FMath::Min(RemainingToAdd, MaxStack);
            
            Slots[i].bIsEmpty = false;
            Slots[i].ItemID = ItemID;
            Slots[i].Quantity = AddAmount;
            
            RemainingToAdd -= AddAmount;
            
            if (RemainingToAdd <= 0)
            {
                OnInventoryChanged.Broadcast();
                return true;
            }
        }
    }

    OnInventoryChanged.Broadcast();
    return RemainingToAdd < Quantity; // Returned true if we added at least some
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    if (Quantity <= 0) return false;
    if (!GetOwner()->HasAuthority()) return false;

    if (!HasItem(ItemID, Quantity)) return false;

    int32 RemainingToRemove = Quantity;

    for (int32 i = MaxSlots - 1; i >= 0; --i)
    {
        if (!Slots[i].bIsEmpty && Slots[i].ItemID == ItemID)
        {
            int32 RemoveAmount = FMath::Min(RemainingToRemove, Slots[i].Quantity);
            Slots[i].Quantity -= RemoveAmount;
            RemainingToRemove -= RemoveAmount;

            if (Slots[i].Quantity <= 0)
            {
                Slots[i].bIsEmpty = true;
                Slots[i].ItemID = NAME_None;
                Slots[i].Quantity = 0;
            }

            if (RemainingToRemove <= 0) break;
        }
    }

    OnInventoryChanged.Broadcast();
    return true;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
    return GetItemCount(ItemID) >= Quantity;
}

int32 UInventoryComponent::GetItemCount(FName ItemID) const
{
    int32 Count = 0;
    for (const FInventorySlot& Slot : Slots)
    {
        if (!Slot.bIsEmpty && Slot.ItemID == ItemID)
        {
            Count += Slot.Quantity;
        }
    }
    return Count;
}

FInventorySlot UInventoryComponent::GetSlot(int32 Index) const
{
    if (Slots.IsValidIndex(Index))
    {
        return Slots[Index];
    }
    return FInventorySlot();
}

TArray<FInventorySlot> UInventoryComponent::GetAllSlots() const
{
    return Slots;
}

int32 UInventoryComponent::FindItemSlot(FName ItemID) const
{
    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (!Slots[i].bIsEmpty && Slots[i].ItemID == ItemID)
        {
            return i;
        }
    }
    return -1;
}

void UInventoryComponent::SwapSlots(int32 A, int32 B)
{
    if (GetOwner()->HasAuthority() && Slots.IsValidIndex(A) && Slots.IsValidIndex(B))
    {
        FInventorySlot Temp = Slots[A];
        
        Slots[A].ItemID = Slots[B].ItemID;
        Slots[A].Quantity = Slots[B].Quantity;
        Slots[A].bIsEmpty = Slots[B].bIsEmpty;

        Slots[B].ItemID = Temp.ItemID;
        Slots[B].Quantity = Temp.Quantity;
        Slots[B].bIsEmpty = Temp.bIsEmpty;

        OnInventoryChanged.Broadcast();
    }
}

void UInventoryComponent::DropItem(int32 SlotIndex)
{
    if (GetOwner()->HasAuthority() && Slots.IsValidIndex(SlotIndex) && !Slots[SlotIndex].bIsEmpty)
    {
        // Mock spawning world pickup actor
        // GetWorld()->SpawnActor<AWorldPickup>(PickupClass, DropLocation, DropRotation);
        
        Slots[SlotIndex].bIsEmpty = true;
        Slots[SlotIndex].ItemID = NAME_None;
        Slots[SlotIndex].Quantity = 0;
        
        OnInventoryChanged.Broadcast();
    }
}

float UInventoryComponent::GetCurrentWeightKg() const
{
    float TotalWeight = 0.0f;
    for (const FInventorySlot& Slot : Slots)
    {
        if (!Slot.bIsEmpty)
        {
            // Weight lookup would go here
            float ItemWeight = 1.0f; 
            TotalWeight += ItemWeight * Slot.Quantity;
        }
    }
    return TotalWeight;
}

void UInventoryComponent::ClearInventory()
{
    if (GetOwner()->HasAuthority())
    {
        for (FInventorySlot& Slot : Slots)
        {
            Slot.bIsEmpty = true;
            Slot.ItemID = NAME_None;
            Slot.Quantity = 0;
        }
        OnInventoryChanged.Broadcast();
    }
}

TArray<FInventoryItemSave> UInventoryComponent::GetSaveData() const
{
    TArray<FInventoryItemSave> SaveData;
    for (const FInventorySlot& Slot : Slots)
    {
        if (!Slot.bIsEmpty)
        {
            FInventoryItemSave Data;
            Data.SlotIndex = Slot.SlotIndex;
            Data.ItemID = Slot.ItemID;
            Data.Quantity = Slot.Quantity;
            SaveData.Add(Data);
        }
    }
    return SaveData;
}

void UInventoryComponent::LoadFromSaveData(const TArray<FInventoryItemSave>& SaveData)
{
    if (GetOwner()->HasAuthority())
    {
        ClearInventory();
        for (const FInventoryItemSave& Data : SaveData)
        {
            if (Slots.IsValidIndex(Data.SlotIndex))
            {
                Slots[Data.SlotIndex].bIsEmpty = false;
                Slots[Data.SlotIndex].ItemID = Data.ItemID;
                Slots[Data.SlotIndex].Quantity = Data.Quantity;
            }
        }
        OnInventoryChanged.Broadcast();
    }
}
