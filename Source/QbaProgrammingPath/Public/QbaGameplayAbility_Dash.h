// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QbaGameplayAbility_Dash.generated.h"


/**
 * 
 */

struct FAbilityTempData
{
	FGameplayAbilitySpecHandle Handle;
	const FGameplayAbilityActorInfo* OwnerInfo {nullptr};
	FGameplayAbilityActivationInfo ActivationInfo;
};


UCLASS()
class QBAPROGRAMMINGPATH_API UQbaGameplayAbility_Dash : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UQbaGameplayAbility_Dash(const FObjectInitializer& ObjectInitializer);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UFUNCTION()
	void OnArrivedToTargetLocation();

	const float DashLenght{200.f};
	const float DashSpeed{ 200.f };

	FAbilityTempData TempData;
};
