#include "CosmosGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

ACosmosGameMode::ACosmosGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentPlanetID = NAME_None;
	SpaceportSpawnLocation = FVector::ZeroVector;
}

void ACosmosGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Initialize PlanetManager, set gravity from planet definition
	if (CurrentPlanetID != NAME_None)
	{
		// Simulate setting planet gravity. Real implementation would look up from UPlanetManager.
		// For now we set default gravity.
		if (GetWorld())
		{
			GetWorld()->GetWorldSettings()->GlobalGravityZ = -981.0f;
		}
	}
}

void ACosmosGameMode::HandlePlayerDeath(AController* PlayerController)
{
	if (PlayerController)
	{
		APawn* DeadPawn = PlayerController->GetPawn();
		if (DeadPawn)
		{
			DeadPawn->Destroy();
		}
		
		// Wait and respawn
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, FTimerDelegate::CreateUObject(this, &ACosmosGameMode::RespawnPlayer, PlayerController), 3.0f, false);
	}
}

void ACosmosGameMode::RespawnPlayer(AController* PlayerController)
{
	if (PlayerController)
	{
		if (DefaultPlayerCharacterClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPlayerCharacterClass, SpaceportSpawnLocation, FRotator::ZeroRotator, SpawnParams);
			if (NewPawn)
			{
				PlayerController->Possess(NewPawn);
			}
		}
	}
}

FName ACosmosGameMode::GetCurrentPlanetID() const
{
	return CurrentPlanetID;
}

void ACosmosGameMode::SetActivePlanet(FName NewPlanetID)
{
	CurrentPlanetID = NewPlanetID;
	
	// Update gravity based on planet
	if (GetWorld())
	{
		float NewGravity = -981.0f;
		if (NewPlanetID == FName("Earth")) NewGravity = -981.0f;
		else if (NewPlanetID == FName("Moon")) NewGravity = -162.0f;
		else if (NewPlanetID == FName("Mars")) NewGravity = -371.0f;
		
		GetWorld()->GetWorldSettings()->GlobalGravityZ = NewGravity;
	}
}
