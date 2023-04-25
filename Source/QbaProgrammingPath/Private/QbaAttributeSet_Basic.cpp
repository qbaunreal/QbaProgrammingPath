// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaAttributeSet_Basic.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


void UQbaAttributeSet_Basic::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data); 
}

void UQbaAttributeSet_Basic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add replication for Health.
	DOREPLIFETIME_CONDITION_NOTIFY(UQbaAttributeSet_Basic, Health, COND_None, REPNOTIFY_Always);
}

/**  Health Getters */
float UQbaAttributeSet_Basic::GetHealth() const
{
	return FMath::Max(Health.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetHealth(float NewHealthValue)
{
	NewHealthValue = FMath::Clamp(NewHealthValue, 0.f, GetMaxHealth());

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	
	if (AbilityComponent)
	{
		AbilityComponent->SetNumericAttributeBase(GetHealthAttribute(), NewHealthValue);
	}	
}

void UQbaAttributeSet_Basic::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UQbaAttributeSet_Basic, Health, OldHealth);
}

/** Max Health Getters */
float UQbaAttributeSet_Basic::GetMaxHealth() const
{
	return FMath::Max(MaxHealth.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetMaxHealth(float NewMaxHealthValue)
{
	NewMaxHealthValue = FMath::Max(NewMaxHealthValue, 0.f);

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (AbilityComponent) 
	{
		AbilityComponent->SetNumericAttributeBase(GetMaxHealthAttribute(), NewMaxHealthValue);
	}
}

/** Stamina Getters */
float UQbaAttributeSet_Basic::GetStamina() const
{
	return FMath::Max(Stamina.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetStamina(float NewStaminaValue)
{
	NewStaminaValue = FMath::Clamp(NewStaminaValue, 0.f, GetMaxStamina());

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (AbilityComponent) 
	{ 
		AbilityComponent->SetNumericAttributeBase(GetStaminaAttribute(), NewStaminaValue);;

	}
}

/** Max Stamina Getters */
float UQbaAttributeSet_Basic::GetMaxStamina() const
{
	return FMath::Max(MaxStamina.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetMaxStamina(float NewMaxStaminaValue)
{
	NewMaxStaminaValue = FMath::Max(NewMaxStaminaValue, 0.f);

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (AbilityComponent)
	{
		AbilityComponent->SetNumericAttributeBase(GetMaxStaminaAttribute(), NewMaxStaminaValue);;
	}
}
