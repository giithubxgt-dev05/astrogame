#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseManagerSubsystem.h"
#include "BaseModuleActor.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class ACosmosCharacter;

// Forward declare the interface since it isn't generated here, but it's required by the prompt
UINTERFACE(MinimalAPI, Blueprintable)
class UCosmosInteractable : public UInterface
{
    GENERATED_BODY()
};

class ICosmosInteractable
{
    GENERATED_BODY()
public:
    virtual FText GetInteractPrompt() = 0;
    virtual void Interact(ACosmosCharacter* Character) = 0;
    virtual bool CanInteract(ACosmosCharacter* Character) = 0;
};

UCLASS()
class PROJECTCOSMOS_API ABaseModuleActor : public AActor, public ICosmosInteractable
{
    GENERATED_BODY()
    
public:    
    ABaseModuleActor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    void InitializeModule(FBaseModule Data, FName InBaseID);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|BaseBuilding")
    void SetPowered(bool bPowered);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cosmos|BaseBuilding")
    bool IsPowered() const;

    // ICosmosInteractable interface
    virtual FText GetInteractPrompt() override;
    virtual void Interact(ACosmosCharacter* Character) override;
    virtual bool CanInteract(ACosmosCharacter* Character) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPointLightComponent* PowerIndicatorLight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|BaseBuilding")
    FBaseModule ModuleData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
    FName BaseID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|BaseBuilding")
    bool bIsPowered;
};
