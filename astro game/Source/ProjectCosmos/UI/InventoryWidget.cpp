#include "InventoryWidget.h"
#include "Components/PanelWidget.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SelectedSlotIndex = -1;
}

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* InInventoryComp)
{
    OwnerInventory = InInventoryComp;
    RefreshDisplay();
}

void UInventoryWidget::RefreshDisplay()
{
    // Normally we'd clear SlotWidgets and rebuild based on OwnerInventory
    // For now we assume blueprint handles the actual binding or we'd iterate UI slots
}

void UInventoryWidget::OnSlotClicked(int32 SlotIndex)
{
    SelectedSlotIndex = SlotIndex;
    RefreshDisplay();
}

void UInventoryWidget::OnSlotHovered(int32 SlotIndex)
{
    // Update tooltip or highlighted slot visual
}

void UInventoryWidget::UseSelectedItem()
{
    if (OwnerInventory && SelectedSlotIndex >= 0)
    {
        // OwnerInventory->UseItemAtIndex(SelectedSlotIndex);
        RefreshDisplay();
    }
}

void UInventoryWidget::DropSelectedItem()
{
    if (OwnerInventory && SelectedSlotIndex >= 0)
    {
        // OwnerInventory->DropItemAtIndex(SelectedSlotIndex);
        RefreshDisplay();
    }
}

FInventorySlot UInventoryWidget::GetSelectedSlot() const
{
    FInventorySlot Result;
    // if (OwnerInventory && SelectedSlotIndex >= 0)
    // {
    //     Result = OwnerInventory->GetSlotAtIndex(SelectedSlotIndex);
    // }
    return Result;
}
