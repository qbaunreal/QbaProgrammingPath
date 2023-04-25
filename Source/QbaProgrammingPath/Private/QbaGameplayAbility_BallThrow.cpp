// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaGameplayAbility_BallThrow.h"
#include "GameFramework/Character.h"

UQbaGameplayAbility_BallThrow::UQbaGameplayAbility_BallThrow(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UQbaGameplayAbility_BallThrow::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		const TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
		return (Character && Character->CanJump()); //TODO: is there any restriction for this ability?
	}
	return false;
}

void UQbaGameplayAbility_BallThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
	{
		if (CommitAbility(Handle, OwnerInfo, ActivationInfo))
		{
			TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(OwnerInfo->AvatarActor.Get());
			Character->Jump();
		}
	}
}

void UQbaGameplayAbility_BallThrow::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UQbaGameplayAbility_BallThrow::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UQbaGameplayAbility_BallThrow::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();;
}
