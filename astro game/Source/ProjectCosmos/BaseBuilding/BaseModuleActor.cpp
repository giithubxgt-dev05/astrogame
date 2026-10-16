#include "BaseModuleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"

ABaseModuleActor::ABaseModuleActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    PowerIndicatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PowerIndicatorLight"));
    PowerIndicatorLight->SetupAttachment(RootComponent);
    PowerIndicatorLight->SetIntensity(0.0f);
    PowerIndicatorLight->SetLightColor(FLinearColor::Red);

    bIsPowered = false;
}

void ABaseModuleActor::BeginPlay()
{
    Super::BeginPlay();
    
    // Attempt to register with base manager if initialized manually via blueprint
}

void ABaseModuleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseModuleActor::InitializeModule(FBaseModule Data, FName InBaseID)
{
    ModuleData = Data;
    BaseID = InBaseID;
    
    SetActorLocationAndRotation(Data.WorldLocation, Data.WorldRotation);
    SetPowered(Data.bIsPowered);
}

void ABaseModuleActor::SetPowered(bool bPowered)
{
    bIsPowered = bPowered;
    ModuleData.bIsPowered = bPowered;

    if (bIsPowered)
    {
        PowerIndicatorLight->SetIntensity(5000.0f);
        PowerIndicatorLight->SetLightColor(FLinearColor::Green);
    }
    else
    {
        PowerIndicatorLight->SetIntensity(1000.0f);
        PowerIndicatorLight->SetLightColor(FLinearColor::Red);
    }
}

bool ABaseModuleActor::IsPowered() const
{
    return bIsPowered;
}

FText ABaseModuleActor::GetInteractPrompt()
{
    FString Prompt = FString::Printf(TEXT("Access %d Module"), (int32)ModuleData.ModuleType);
    return FText::FromString(Prompt);
}

void ABaseModuleActor::Interact(ACosmosCharacter* Character)
{
    // Open UI or interact based on module type
    UE_LOG(LogTemp, Warning, TEXT("Interacted with Base Module: %s"), *ModuleData.ModuleID.ToString());
}

bool ABaseModuleActor::CanInteract(ACosmosCharacter* Character)
{
    // Usually modules can be interacted with to manage them, regardless of power state
    return true;
}
