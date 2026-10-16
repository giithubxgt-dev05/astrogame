#include "EquipmentComponent.h"
#include "GameFramework/Actor.h"
// #include "HazardComponent.h"

// Dummy declaration for UHazardComponent
class UHazardComponent : public UActorComponent {
public:
    void UpdateHazardProtection(EHazardType Type, float Value) {}
};

UEquipmentComponent::UEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
    NotifyHazardComponent();
}

void UEquipmentComponent::EquipItem(FName ItemID, EEquipmentSlot Slot)
{
    FEquipmentItem NewItem;
    NewItem.ItemID = ItemID;
    NewItem.EquipmentSlot = Slot;
    NewItem.bIsEquipped = true;
    
    // Mock item properties based on ItemID lookup
    if (Slot == EEquipmentSlot::Spacesuit)
    {
        NewItem.PressureProtection = 0.8f;
        NewItem.ThermalProtection = 0.5f;
    }
    else if (Slot == EEquipmentSlot::Helmet)
    {
        NewItem.OxygenCapacity = 100.0f;
    }

    EquippedItems.Add(Slot, NewItem);
    
    NotifyHazardComponent();
    
    // Broadcast changes, update mesh visuals, etc.
}

void UEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
    if (EquippedItems.Contains(Slot))
    {
        EquippedItems.Remove(Slot);
        NotifyHazardComponent();
        
        // Broadcast changes, remove mesh visuals, etc.
    }
}

FEquipmentItem UEquipmentComponent::GetEquippedItem(EEquipmentSlot Slot) const
{
    if (const FEquipmentItem* Item = EquippedItems.Find(Slot))
    {
        return *Item;
    }
    return FEquipmentItem();
}

bool UEquipmentComponent::IsSlotOccupied(EEquipmentSlot Slot) const
{
    return EquippedItems.Contains(Slot) && EquippedItems[Slot].bIsEquipped;
}

float UEquipmentComponent::GetTotalOxygenCapacity() const
{
    float Total = 0.0f;
    for (const auto& Pair : EquippedItems)
    {
        Total += Pair.Value.OxygenCapacity;
    }
    return Total;
}

float UEquipmentComponent::GetTotalThermalProtection() const
{
    float Total = 0.0f;
    for (const auto& Pair : EquippedItems)
    {
        Total += Pair.Value.ThermalProtection;
    }
    return FMath::Clamp(Total, 0.0f, 1.0f);
}

float UEquipmentComponent::GetTotalRadiationProtection() const
{
    float Total = 0.0f;
    for (const auto& Pair : EquippedItems)
    {
        Total += Pair.Value.RadiationProtection;
    }
    return FMath::Clamp(Total, 0.0f, 1.0f);
}

float UEquipmentComponent::GetTotalPressureProtection() const
{
    float Total = 0.0f;
    for (const auto& Pair : EquippedItems)
    {
        Total += Pair.Value.PressureProtection;
    }
    return FMath::Clamp(Total, 0.0f, 1.0f);
}

float UEquipmentComponent::GetTotalWeight() const
{
    float Total = 0.0f;
    for (const auto& Pair : EquippedItems)
    {
        Total += Pair.Value.Weight;
    }
    return Total;
}

void UEquipmentComponent::NotifyHazardComponent()
{
    if (AActor* Owner = GetOwner())
    {
        if (UHazardComponent* HazComp = Owner->FindComponentByClass<UHazardComponent>())
        {
            HazComp->UpdateHazardProtection(EHazardType::Vacuum, GetTotalPressureProtection());
            HazComp->UpdateHazardProtection(EHazardType::Radiation, GetTotalRadiationProtection());
            
            float Thermal = GetTotalThermalProtection();
            HazComp->UpdateHazardProtection(EHazardType::ExtremeHeat, Thermal);
            HazComp->UpdateHazardProtection(EHazardType::ExtremeCold, Thermal);
        }
    }
}

float UEquipmentComponent::GetHazardProtectionFactor(EHazardType HazardType) const
{
    switch (HazardType)
    {
    case EHazardType::Vacuum:
        return GetTotalPressureProtection();
    case EHazardType::Radiation:
        return GetTotalRadiationProtection();
    case EHazardType::ExtremeHeat:
    case EHazardType::ExtremeCold:
        return GetTotalThermalProtection();
    default:
        return 0.0f;
    }
}
