


#include "QbaAttributeSet_Basic.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


void UQbaAttributeSet_Basic::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data); 
}

//TODO: Learn and understand how this replication works.
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
	
	if (!AbilityComponent) return;
		
	AbilityComponent->SetNumericAttributeBase(GetHealthAttribute(), NewHealthValue);
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

	if (!AbilityComponent) return;

	AbilityComponent->SetNumericAttributeBase(GetMaxHealthAttribute(), NewMaxHealthValue);
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

	if (!AbilityComponent) return;

	AbilityComponent->SetNumericAttributeBase(GetStaminaAttribute(), NewStaminaValue);
}

/** Max Stamina Getters */
float UQbaAttributeSet_Basic::GetMaxStamina() const
{
	return FMath::Max(MaxStamina.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetMaxStamina(float NewMaxStaminaValue)
{
	/*NewMaxStaminaValue = FMath::Clamp(NewMaxStaminaValue, 0.f, GetMaxHealth());*/

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (!AbilityComponent) return;

	AbilityComponent->SetNumericAttributeBase(GetMaxStaminaAttribute(), NewMaxStaminaValue);
}

/** Mana Getters */
float UQbaAttributeSet_Basic::GetMana() const
{
	return FMath::Max(Mana.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetMana(float NewManaValue)
{
	NewManaValue = FMath::Clamp(NewManaValue, 0.f, GetMaxStamina());

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (!AbilityComponent) return;

	AbilityComponent->SetNumericAttributeBase(GetManaAttribute(), NewManaValue);
}

/** Max Mana Getters*/
float UQbaAttributeSet_Basic::GetMaxMana() const
{
	return FMath::Max(MaxMana.GetCurrentValue(), 0.0f);
}

void UQbaAttributeSet_Basic::SetMaxMana(float NewMaxManaValue)
{
	NewMaxManaValue = FMath::Max(NewMaxManaValue, 0.f);

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (!AbilityComponent) return;

	AbilityComponent->SetNumericAttributeBase(GetMaxManaAttribute(), NewMaxManaValue);
}

//** 
// 1. Telekinesis: Have a force calulation, based on the expression calulation. Takes mass of the object. Stat is kept as transient in Basic stats (for a player)
// 2. have a mana consumption calculation, the higher the mass, the bigger cost /sec
// 3. In gameplay ability component create a binding to enhanced input
// 4. Input will be like in gemini, and get rid of the ability wheel. There will be max 3 abilities. Granted when entering some volume or other s
// 5. each ability should have cooldown
// 6. each ability should cancel each other
// 7 jumping should be separate ability
// 8. walking should be separate abiltiy
// 9. Ideally i would like to see it working in muliplayer

