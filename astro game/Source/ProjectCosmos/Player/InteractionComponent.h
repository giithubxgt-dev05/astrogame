#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/Interface.h"
#include "InteractionComponent.generated.h"

class ACosmosCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UCosmosInteractable : public UInterface
{
    GENERATED_BODY()
};

class PROJECTCOSMOS_API ICosmosInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void Interact(ACosmosCharacter* Character);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    FText GetInteractPrompt();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    bool CanInteract(ACosmosCharacter* Character);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFoundSignature, AActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableLostSignature);

UCLASS(ClassGroup=(ProjectCosmos), meta=(BlueprintSpawnableComponent))
class PROJECTCOSMOS_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Interaction")
    float InteractionRange = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Interaction")
    TEnumAsByte<ECollisionChannel> InteractionChannel;

    UPROPERTY(BlueprintReadOnly, Category = "Cosmos|Interaction")
    TWeakObjectPtr<AActor> CurrentInteractable;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Interaction")
    void PerformInteractionTrace();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Interaction")
    void TryInteract();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Interaction")
    AActor* GetCurrentInteractable() const;

    UFUNCTION(BlueprintPure, Category = "Cosmos|Interaction")
    FText GetInteractionPrompt() const;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Interaction")
    FOnInteractableFoundSignature OnInteractableFound;

    UPROPERTY(BlueprintAssignable, Category = "Cosmos|Interaction")
    FOnInteractableLostSignature OnInteractableLost;
};
