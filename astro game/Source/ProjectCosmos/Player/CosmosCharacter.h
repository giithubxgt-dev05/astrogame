#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CosmosCharacter.generated.h"

class USurvivalComponent;
class UInventoryComponent;
class UEquipmentComponent;
class UInteractionComponent;
class UHazardComponent;
class UCameraComponent;
class USpringArmComponent;
class ACosmosShipBase;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    OnFoot_Walk,
    OnFoot_Run,
    OnFoot_Crouch,
    OnFoot_Jump,
    Swimming,
    Climbing,
    LowGravity,
    ZeroGravity_EVA,
    InVehicle_Rover,
    InVehicle_Ship,
    Dead
};

UCLASS(Config=Game)
class PROJECTCOSMOS_API ACosmosCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACosmosCharacter();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    USurvivalComponent* SurvivalComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UEquipmentComponent* EquipmentComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UInteractionComponent* InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmos|Components")
    UHazardComponent* HazardComponent;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerState, BlueprintReadOnly, Category = "Cosmos|State")
    EPlayerState CurrentPlayerState;

    UFUNCTION()
    void OnRep_CurrentPlayerState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Cosmos|State")
    bool bIsInShip;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Cosmos|State")
    ACosmosShipBase* CurrentPilotedShip;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Movement")
    float WalkSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Movement")
    float RunSpeed = 700.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmos|Movement")
    float CrouchSpeed = 200.0f;

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void MoveRight(float Value);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void LookUp(float Value);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void LookRight(float Value);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void StopSprint();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void ToggleCrouch();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Movement")
    void DoJump();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Interaction")
    void Interact();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Vehicle")
    void BoardShip(ACosmosShipBase* Ship);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Vehicle")
    void ExitShip();

    UFUNCTION(BlueprintCallable, Category = "Cosmos|State")
    void SetPlayerState(EPlayerState NewState);

    UFUNCTION(BlueprintCallable, Category = "Cosmos|Physics")
    void UpdateGravity();

    UFUNCTION(BlueprintPure, Category = "Cosmos|Physics")
    float GetCurrentGravityScale() const;
};
