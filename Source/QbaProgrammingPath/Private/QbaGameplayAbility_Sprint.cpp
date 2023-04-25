// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaGameplayAbility_Sprint.h"
#include "GameFramework/Character.h"

UQbaGameplayAbility_Sprint::UQbaGameplayAbility_Sprint(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UQbaGameplayAbility_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return true; //TODO: Check if should have any restrictions
	}
	return false;
}

void UQbaGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
	{
		if (CommitAbility(Handle, OwnerInfo, ActivationInfo))
		{
			TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(OwnerInfo->AvatarActor.Get());
			// TODO: change speed value (and make it into AttributeSet
		}
	}
}

void UQbaGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UQbaGameplayAbility_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UQbaGameplayAbility_Sprint::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	TObjectPtr<ACharacter> Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	//TODO: Change speed back to default value
}
