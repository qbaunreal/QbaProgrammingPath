// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaGameplayAbility_Crouch.h"
#include "GameFramework/Character.h"

UQbaGameplayAbility_Crouch::UQbaGameplayAbility_Crouch(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UQbaGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return true; //TODO: check
	}
	return false;
}

void UQbaGameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
	{
		if (CommitAbility(Handle, OwnerInfo, ActivationInfo))
		{
			TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(OwnerInfo->AvatarActor.Get());
			//TODO: Resize player model/capsule
		}
	}
}

void UQbaGameplayAbility_Crouch::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UQbaGameplayAbility_Crouch::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UQbaGameplayAbility_Crouch::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	// TODO: resize the capsule back
}
