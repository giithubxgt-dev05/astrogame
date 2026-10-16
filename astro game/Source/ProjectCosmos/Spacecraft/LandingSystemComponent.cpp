#include "LandingSystemComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ULandingSystemComponent::ULandingSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    LandingGearState = ELandingGearState::Retracted;
    bAssistedLandingActive = false;
    RadarAltitudeM = 0.0f;
    LandingGearDeployAltitudeM = 500.0f;
    DescentRateMs = 0.0f;
    bLandingTargetSet = false;
    LandingTarget = FVector::ZeroVector;
    bLandingComplete = false;
}

void ULandingSystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void ULandingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateRadarAltitude();

    if (bAssistedLandingActive && !bLandingComplete)
    {
        PIDAssistedDescent(DeltaTime);
        if (RadarAltitudeM <= LandingGearDeployAltitudeM && LandingGearState == ELandingGearState::Retracted)
        {
            DeployLandingGear();
        }
    }
}

void ULandingSystemComponent::DeployLandingGear()
{
    LandingGearState = ELandingGearState::Deploying;
    // Timer or anim hook to set Deployed
    LandingGearState = ELandingGearState::Deployed;
}

void ULandingSystemComponent::RetractLandingGear()
{
    LandingGearState = ELandingGearState::Retracting;
    // Timer or anim hook to set Retracted
    LandingGearState = ELandingGearState::Retracted;
}

void ULandingSystemComponent::SetLandingTarget(FVector Target)
{
    LandingTarget = Target;
    bLandingTargetSet = true;
}

void ULandingSystemComponent::ActivateAssistedLanding()
{
    bAssistedLandingActive = true;
    bLandingComplete = false;
}

void ULandingSystemComponent::DeactivateAssistedLanding()
{
    bAssistedLandingActive = false;
}

void ULandingSystemComponent::UpdateRadarAltitude()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start - FVector(0, 0, 1000000.0f); // 10km max range

    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        RadarAltitudeM = Hit.Distance / 100.0f; // cm to m
        if (RadarAltitudeM < 5.0f && bAssistedLandingActive && !bLandingComplete) // Touching ground
        {
            OnLandingComplete();
        }
    }
    else
    {
        RadarAltitudeM = 10000.0f;
    }
}

ELandingGearState ULandingSystemComponent::GetLandingGearState() const
{
    return LandingGearState;
}

bool ULandingSystemComponent::IsLandingSafe() const
{
    AActor* Owner = GetOwner();
    if (!Owner) return false;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start - FVector(0, 0, 10000.0f);

    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        float Slope = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector)));
        if (Slope < 15.0f) // Max safe slope 15 degrees
        {
            return true;
        }
    }
    return false;
}

void ULandingSystemComponent::OnLandingComplete()
{
    bLandingComplete = true;
    bAssistedLandingActive = false;
    // Event dispatch
}

void ULandingSystemComponent::PIDAssistedDescent(float DeltaTime)
{
    UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if (!PrimComp) return;

    float TargetDescentRateMs = -5.0f; // 5 m/s descent
    float CurrentZVelocityMs = PrimComp->GetComponentVelocity().Z / 100.0f;

    float Error = TargetDescentRateMs - CurrentZVelocityMs;
    
    // Simple PID
    float P = 2.0f;
    float I = 0.1f;
    float D = 0.5f;

    float ThrustCorrection = (P * Error); // Simplified, ignoring I and D for raw implementation

    PrimComp->AddForce(FVector(0, 0, ThrustCorrection * PrimComp->GetMass() * 100.0f));

    // Damping rotation
    FVector AngularVel = PrimComp->GetPhysicsAngularVelocityInRadians();
    PrimComp->AddTorqueInRadians(-AngularVel * PrimComp->GetMass() * 5.0f, NAME_None, true);
}
