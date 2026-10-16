#include "InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CosmosCharacter.h"
#include "Camera/CameraComponent.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    InteractionChannel = ECC_Visibility;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Only perform trace locally if it's the player
    if (GetOwner() && GetOwner()->GetLocalRole() == ROLE_AutonomousProxy || (GetOwner()->GetLocalRole() == ROLE_Authority && Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController()) != nullptr && Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController())->IsLocalController()))
    {
        PerformInteractionTrace();
    }
}

void UInteractionComponent::PerformInteractionTrace()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UCameraComponent* CameraComp = Owner->FindComponentByClass<UCameraComponent>();
    if (!CameraComp) return;

    FVector Start = CameraComp->GetComponentLocation();
    FVector End = Start + (CameraComp->GetForwardVector() * InteractionRange);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, InteractionChannel, QueryParams);

    if (bHit && HitResult.GetActor())
    {
        AActor* HitActor = HitResult.GetActor();
        
        if (HitActor->Implements<UCosmosInteractable>())
        {
            if (CurrentInteractable.Get() != HitActor)
            {
                CurrentInteractable = HitActor;
                OnInteractableFound.Broadcast(HitActor);
            }
            return;
        }
    }

    if (CurrentInteractable.IsValid())
    {
        CurrentInteractable.Reset();
        OnInteractableLost.Broadcast();
    }
}

void UInteractionComponent::TryInteract()
{
    if (CurrentInteractable.IsValid())
    {
        AActor* InteractActor = CurrentInteractable.Get();
        
        if (InteractActor->Implements<UCosmosInteractable>())
        {
            ACosmosCharacter* Character = Cast<ACosmosCharacter>(GetOwner());
            
            if (ICosmosInteractable::Execute_CanInteract(InteractActor, Character))
            {
                ICosmosInteractable::Execute_Interact(InteractActor, Character);
            }
        }
    }
}

AActor* UInteractionComponent::GetCurrentInteractable() const
{
    return CurrentInteractable.Get();
}

FText UInteractionComponent::GetInteractionPrompt() const
{
    if (CurrentInteractable.IsValid())
    {
        if (CurrentInteractable->Implements<UCosmosInteractable>())
        {
            return ICosmosInteractable::Execute_GetInteractPrompt(CurrentInteractable.Get());
        }
    }
    return FText::GetEmpty();
}
