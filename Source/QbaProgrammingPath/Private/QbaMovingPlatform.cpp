// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaMovingPlatform.h"

AQbaMovingPlatform::AQbaMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AQbaMovingPlatform::BeginPlay()
{
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		SetActorTickEnabled(false);

		WorldStartLocation = GetActorLocation();
		WorldTargetLocation = GetTransform().TransformPosition(TargetLocation);
	}
}

void AQbaMovingPlatform::Tick(float DeltaTime)
{
	MoveForward(DeltaTime);
}

void AQbaMovingPlatform::MoveForward(float DeltaTime)
{
	if (bAllowMoving)
	{
		if (HasAuthority())
		{
			FVector CurrentLocation = GetActorLocation();

			const float PathLenght = (WorldTargetLocation - WorldStartLocation).Size();
			const float DistanceTravelled = (CurrentLocation - WorldStartLocation).Size();

			if (DistanceTravelled >= PathLenght)
			{
				const FVector OldLocation = WorldStartLocation;
				WorldStartLocation = WorldTargetLocation;
				WorldTargetLocation = OldLocation;
			}

			const FVector Direction = (WorldTargetLocation - CurrentLocation).GetSafeNormal();

			CurrentLocation += (Direction * MovementSpeed * DeltaTime);
			SetActorLocation(CurrentLocation);
			UE_LOG(LogTemp, Warning, TEXT("Server"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Client"));
		}
	}
}
