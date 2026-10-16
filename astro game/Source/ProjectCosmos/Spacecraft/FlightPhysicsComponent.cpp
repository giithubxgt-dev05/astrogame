#include "FlightPhysicsComponent.h"
#include "Components/PrimitiveComponent.h"

UFlightPhysicsComponent::UFlightPhysicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    FlightMode = EFlightMode::Vacuum;
    ThrustVector = FVector::UpVector;
    CurrentThrust = 0.0f;
    MaxThrust = 1500000.0f; // 1500 kN
    AtmosphericDragCoeff = 0.3f;
    ReferenceArea = 20.0f;
    bAtmosphericFlightEnabled = true;
    CurrentHeatLevel = 0.0f;
    bAutostabilize = true;
}

void UFlightPhysicsComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFlightPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if (!PrimComp || !PrimComp->IsSimulatingPhysics()) return;

    // Apply main thrust
    if (CurrentThrust > 0.0f)
    {
        FVector ThrustForce = ThrustVector * CurrentThrust * MaxThrust;
        PrimComp->AddForce(ThrustForce);
    }

    // Determine flight mode logic (simplified representation)
    float Altitude = GetOwner()->GetActorLocation().Z / 100000.0f;
    float AtmoDensity = FMath::Clamp(1.0f - (Altitude / 100.0f), 0.0f, 1.0f); // Example formula
    FVector Velocity = PrimComp->GetComponentVelocity();

    if (AtmoDensity > 0.1f)
    {
        if (Velocity.Size() > 20000.0f) // Very fast in atmosphere -> Reentry
        {
            FlightMode = EFlightMode::Reentry;
        }
        else
        {
            FlightMode = EFlightMode::Atmospheric;
        }
    }
    else
    {
        FlightMode = EFlightMode::Vacuum;
    }

    if (FlightMode == EFlightMode::Atmospheric)
    {
        ApplyAtmosphericDrag(PrimComp, AtmoDensity, Velocity);
    }
    else if (FlightMode == EFlightMode::Vacuum)
    {
        if (bAutostabilize)
        {
            Autostabilize(PrimComp);
        }
    }
    else if (FlightMode == EFlightMode::Reentry)
    {
        ApplyAtmosphericDrag(PrimComp, AtmoDensity, Velocity);
        ApplyReentryHeat(Velocity.Size() / 100000.0f, AtmoDensity); // km/s
    }
}

void UFlightPhysicsComponent::SetThrust(float NormalizedThrust)
{
    CurrentThrust = FMath::Clamp(NormalizedThrust, 0.0f, 1.0f);
}

void UFlightPhysicsComponent::SetThrustVector(FVector NewThrustVector)
{
    ThrustVector = NewThrustVector.GetSafeNormal();
}

void UFlightPhysicsComponent::ApplyAtmosphericDrag(UPrimitiveComponent* TargetComp, float AtmoDensity, FVector Velocity)
{
    if (TargetComp)
    {
        FVector DragForce = GetDragForce(Velocity, AtmoDensity);
        TargetComp->AddForce(DragForce);
    }
}

void UFlightPhysicsComponent::ApplyReentryHeat(float VelocityKms, float AtmoDensity)
{
    float HeatAccumulation = VelocityKms * AtmoDensity;
    CurrentHeatLevel += HeatAccumulation * GetWorld()->GetDeltaSeconds();
    
    if (CurrentHeatLevel > 1.0f)
    {
        // Apply damage to hull
    }
}

void UFlightPhysicsComponent::SetFlightMode(EFlightMode NewMode)
{
    FlightMode = NewMode;
}

FVector UFlightPhysicsComponent::GetDragForce(FVector Velocity, float AtmoDensity) const
{
    float SpeedSq = Velocity.SizeSquared();
    if (SpeedSq < SMALL_NUMBER) return FVector::ZeroVector;
    
    float DragMagnitude = 0.5f * AtmoDensity * SpeedSq * AtmosphericDragCoeff * ReferenceArea;
    return -Velocity.GetSafeNormal() * DragMagnitude;
}

void UFlightPhysicsComponent::ApplyGravity(float GravityScale)
{
    UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if (PrimComp)
    {
        PrimComp->AddForce(FVector(0, 0, -980.0f * GravityScale) * PrimComp->GetMass());
    }
}

float UFlightPhysicsComponent::GetCurrentHeatLevel() const
{
    return CurrentHeatLevel;
}

void UFlightPhysicsComponent::Autostabilize(UPrimitiveComponent* TargetComp)
{
    if (TargetComp)
    {
        FVector AngularVel = TargetComp->GetPhysicsAngularVelocityInRadians();
        TargetComp->AddTorqueInRadians(-AngularVel * TargetComp->GetMass() * 10.0f, NAME_None, true);
    }
}
