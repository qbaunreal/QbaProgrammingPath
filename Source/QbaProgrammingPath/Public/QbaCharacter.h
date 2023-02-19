// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "QbaGameplayTags.h"
#include "QbaAttributeSet_Basic.h"
#include "QbaCharacter.generated.h"

class USkeletalMeshComponent;
class UQbaCameraSystemComponent;
class UQbaInputConfig;
class UAbilitySystemComponent;
class UQbaAbilitySystemComponent;
class UQbaGameplayAbility;
class UGameplayAbility;
struct FInputActionValue;
struct FOnAttributeChangeData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChange, float, StaminaValue, float, MaxStaminaValue);

UCLASS()
class QBAPROGRAMMINGPATH_API AQbaCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AQbaCharacter();


protected:
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	void OnStaminaUpdated(const FOnAttributeChangeData& Data);
	
private:

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Look_Gamepad(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Fire(const FInputActionValue& InputActionValue);
	void Input_Aim(const FInputActionValue& InputActionValue);
	void Input_AbilityWheel(const FInputActionValue& InputActionValue);

	void TurnAtRate(float Value, float Rate);
	void LookUpAtRate(float Value, float Rate);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UQbaInputConfig> InputConfig;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnStaminaChange OnStaminaChange;

private:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UQbaCameraSystemComponent> CameraSystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Abilities")
	TObjectPtr<UQbaAbilitySystemComponent> AbilitySystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	float MouseLookRate = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GamepadLookRate = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> JumpAbility;

	FQbaAbilityTags AbilityTags;
	FQbaInputTags InputTags;

	TObjectPtr<UQbaAttributeSet_Basic> BasicAttributes;
	float StaminaValue;
	float MaxStaminaValue;
};

