// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaGameplayAbility_Dash.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "QbaDashToLocationTask.h"

#include "GameFramework/Character.h"


UQbaGameplayAbility_Dash::UQbaGameplayAbility_Dash(const FObjectInitializer& ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UQbaGameplayAbility_Dash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		const TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	
		return (Character && !Character->GetLastMovementInputVector().IsNearlyZero());
	}
	return false;
}

void UQbaGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
	{
		if (CommitAbility(Handle, OwnerInfo, ActivationInfo))
		{
			TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(OwnerInfo->AvatarActor.Get());
			
			TempData.Handle = Handle;
			TempData.OwnerInfo = OwnerInfo;
			TempData.ActivationInfo = ActivationInfo;

			const FVector PlayerLocation = Character->GetActorLocation();
			const FVector MovementDirection = Character->GetLastMovementInputVector().GetSafeNormal();
			const FVector TargetDestination = PlayerLocation + MovementDirection * DashLenght;
			
			TObjectPtr<UQbaDashToLocationTask> DashTask = UQbaDashToLocationTask::DashToLocation(this, NAME_None, TargetDestination, 60.f/DashSpeed);
			//TODO: Have dash value from gameplayeffect

			DashTask->OnTargetLocationReached.AddDynamic(this, &UQbaGameplayAbility_Dash::OnArrivedToTargetLocation);

			DashTask->ReadyForActivation();
		}
		else
		{
			EndAbility(Handle, OwnerInfo, ActivationInfo, true, true);
		}
	}
}

void UQbaGameplayAbility_Dash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UQbaGameplayAbility_Dash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UQbaGameplayAbility_Dash::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UQbaGameplayAbility_Dash::OnArrivedToTargetLocation()
{
	EndAbility(TempData.Handle, TempData.OwnerInfo, TempData.ActivationInfo,true,false);
}

