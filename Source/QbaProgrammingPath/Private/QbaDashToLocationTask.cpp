// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaDashToLocationTask.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(QbaDashToLocationTask)

UQbaDashToLocationTask::UQbaDashToLocationTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;
}

UQbaDashToLocationTask* UQbaDashToLocationTask::DashToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration)
{
	TObjectPtr<UQbaDashToLocationTask> MyObj = NewAbilityTask<UQbaDashToLocationTask>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
	}

	MyObj->TargetLocation = Location;
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);
	MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;
	MyObj->bTickingTask = true;
	MyObj->bSimulatedTask = true;
	MyObj->bIsFinished = false;
	return MyObj;
}

void UQbaDashToLocationTask::Activate()
{

}

void UQbaDashToLocationTask::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

	TimeMoveStarted = GetWorld()->GetTimeSeconds();
	TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
}

void UQbaDashToLocationTask::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);
	TObjectPtr<AActor> MyActor = GetAvatarActor();
	if (MyActor)
	{
		TObjectPtr<ACharacter> MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			TObjectPtr<UCharacterMovementComponent> CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(MOVE_Custom, 0);
			}
		}


		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			MyActor->TeleportTo(TargetLocation, MyActor->GetActorRotation());
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnTargetLocationReached.Broadcast();
				}
				EndTask();
			}
		}
		else
		{
			const float MoveFraction = (CurrentTime - TimeMoveStarted) / DurationOfMovement;
			const FVector NewLocation = FMath::Lerp<FVector, float>(StartLocation, TargetLocation, MoveFraction);

			MyActor->SetActorLocation(NewLocation);
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UQbaDashToLocationTask::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(UQbaDashToLocationTask, StartLocation);
	DOREPLIFETIME(UQbaDashToLocationTask, TargetLocation);
	DOREPLIFETIME(UQbaDashToLocationTask, DurationOfMovement);
}

void UQbaDashToLocationTask::OnDestroy(bool AbilityIsEnding)
{
	TObjectPtr<AActor> MyActor = GetAvatarActor();
	if (MyActor)
	{
		TObjectPtr<ACharacter> MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			TObjectPtr<UCharacterMovementComponent> CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp && CharMoveComp->MovementMode == MOVE_Custom)
			{
				CharMoveComp->SetMovementMode(MOVE_Falling);
			}
		}
	}

	Super::OnDestroy(AbilityIsEnding);
}

