// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "QbaAttributeSet_Basic.generated.h"


UCLASS()
class QBAPROGRAMMINGPATH_API UQbaAttributeSet_Basic : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Character Current health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Health")
	FGameplayAttributeData Health;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UQbaAttributeSet_Basic, Health);
	float GetHealth() const;
	void SetHealth(float NewHealthValue);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);
	
	/** Called when a new Health value arrives over the network */
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	/**  Character Max Health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UQbaAttributeSet_Basic, MaxHealth);
	float GetMaxHealth() const;
	void SetMaxHealth(float NewMaxHealthValue);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth);

	/** Character Current Stamina */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData Stamina;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UQbaAttributeSet_Basic, Stamina);
	float GetStamina() const;
	void SetStamina(float NewStaminaValue);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Stamina);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStamina;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UQbaAttributeSet_Basic, MaxStamina);
	float GetMaxStamina() const;
	void SetMaxStamina(float NewMaxStaminaValue);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxStamina);

};
