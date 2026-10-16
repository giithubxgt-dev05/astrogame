#include "CosmosCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SurvivalComponent.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.h"
#include "InteractionComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

// Assuming these headers exist based on the prompt
// #include "PlanetManager.h" 
// #include "HazardComponent.h"
// #include "CosmosShipBase.h"

// Dummy declarations for compilation
class UPlanetManager : public UWorldSubsystem {
public:
    float GetCurrentGravityScale() const { return 1.0f; }
};

class UHazardComponent : public UActorComponent {};

class ACosmosShipBase : public AActor {
public:
    FTransform GetCockpitTransform() const { return FTransform::Identity; }
    FTransform GetExitTransform() const { return FTransform::Identity; }
};

ACosmosCharacter::ACosmosCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    SurvivalComponent = CreateDefaultSubobject<USurvivalComponent>(TEXT("SurvivalComponent"));
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
    HazardComponent = CreateDefaultSubobject<UHazardComponent>(TEXT("HazardComponent"));

    CurrentPlayerState = EPlayerState::OnFoot_Walk;
    bIsInShip = false;
    CurrentPilotedShip = nullptr;
}

void ACosmosCharacter::BeginPlay()
{
    Super::BeginPlay();
    UpdateGravity();
}

void ACosmosCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateGravity();
}

void ACosmosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Bind Axis and Actions
    PlayerInputComponent->BindAxis("MoveForward", this, &ACosmosCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACosmosCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ACosmosCharacter::LookUp);
    PlayerInputComponent->BindAxis("LookRight", this, &ACosmosCharacter::LookRight);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACosmosCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACosmosCharacter::StopSprint);
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACosmosCharacter::ToggleCrouch);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACosmosCharacter::DoJump);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACosmosCharacter::Interact);
}

void ACosmosCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACosmosCharacter, CurrentPlayerState);
    DOREPLIFETIME(ACosmosCharacter, bIsInShip);
    DOREPLIFETIME(ACosmosCharacter, CurrentPilotedShip);
}

void ACosmosCharacter::OnRep_CurrentPlayerState()
{
    // Handle state change visuals or client-side logic
}

void ACosmosCharacter::MoveForward(float Value)
{
    if (Value != 0.0f && !bIsInShip)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void ACosmosCharacter::MoveRight(float Value)
{
    if (Value != 0.0f && !bIsInShip)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void ACosmosCharacter::LookUp(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerPitchInput(Value);
    }
}

void ACosmosCharacter::LookRight(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerYawInput(Value);
    }
}

void ACosmosCharacter::StartSprint()
{
    if (!bIsInShip && CurrentPlayerState != EPlayerState::OnFoot_Crouch)
    {
        SetPlayerState(EPlayerState::OnFoot_Run);
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
}

void ACosmosCharacter::StopSprint()
{
    if (!bIsInShip && CurrentPlayerState == EPlayerState::OnFoot_Run)
    {
        SetPlayerState(EPlayerState::OnFoot_Walk);
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void ACosmosCharacter::ToggleCrouch()
{
    if (bIsInShip) return;

    if (CurrentPlayerState == EPlayerState::OnFoot_Crouch)
    {
        SetPlayerState(EPlayerState::OnFoot_Walk);
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
        UnCrouch();
    }
    else
    {
        SetPlayerState(EPlayerState::OnFoot_Crouch);
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
        Crouch();
    }
}

void ACosmosCharacter::DoJump()
{
    if (bIsInShip) return;

    SetPlayerState(EPlayerState::OnFoot_Jump);
    Jump();
}

void ACosmosCharacter::Interact()
{
    if (InteractionComponent)
    {
        InteractionComponent->TryInteract();
    }
}

void ACosmosCharacter::BoardShip(ACosmosShipBase* Ship)
{
    if (Ship && HasAuthority())
    {
        bIsInShip = true;
        CurrentPilotedShip = Ship;
        SetPlayerState(EPlayerState::InVehicle_Ship);
        
        GetCharacterMovement()->DisableMovement();
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        // Assuming Cockpit Socket or transform
        SetActorTransform(Ship->GetCockpitTransform());
        AttachToActor(Ship, FAttachmentTransformRules::KeepWorldTransform);
    }
}

void ACosmosCharacter::ExitShip()
{
    if (bIsInShip && CurrentPilotedShip && HasAuthority())
    {
        bIsInShip = false;
        SetPlayerState(EPlayerState::OnFoot_Walk);
        
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        SetActorHiddenInGame(false);
        SetActorEnableCollision(true);

        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        SetActorTransform(CurrentPilotedShip->GetExitTransform());
        
        CurrentPilotedShip = nullptr;
    }
}

void ACosmosCharacter::SetPlayerState(EPlayerState NewState)
{
    if (HasAuthority())
    {
        CurrentPlayerState = NewState;
        OnRep_CurrentPlayerState();
    }
}

void ACosmosCharacter::UpdateGravity()
{
    if (UWorld* World = GetWorld())
    {
        if (UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>())
        {
            float Scale = PlanetManager->GetCurrentGravityScale();
            GetCharacterMovement()->GravityScale = Scale;
            
            float GravityMagnitude = 980.0f * Scale; // Standard UE gravity is 980 cm/s^2
            float DesiredJumpHeight = 200.0f; // 2 meters
            
            if (GravityMagnitude > 0.0f)
            {
                GetCharacterMovement()->JumpZVelocity = FMath::Sqrt(2.0f * GravityMagnitude * DesiredJumpHeight);
            }
        }
    }
}

float ACosmosCharacter::GetCurrentGravityScale() const
{
    if (UWorld* World = GetWorld())
    {
        if (UPlanetManager* PlanetManager = World->GetSubsystem<UPlanetManager>())
        {
            return PlanetManager->GetCurrentGravityScale();
        }
    }
    return 1.0f;
}
