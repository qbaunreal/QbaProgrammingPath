// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "QbaCharacter.generated.h"

class USkeletalMeshComponent;
class UQbaInputConfig;
class UAbilitySystemComponent;
class UGameplayAbility;
class UInputMappingContext;
class UQbaAttributeSet_Basic;
class USpringArmComponent;
class UCameraComponent;
struct FOnAttributeChangeData;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FPlayerAbilities
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystem{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> JumpAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> SprintAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> DashAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> BallThrowAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> CrouchAbility;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaUpdated, float, StaminaValue, float, MaxStaminaValue);
DECLARE_LOG_CATEGORY_EXTERN(QbaAbilities, Log, All);

UCLASS()
class QBAPROGRAMMINGPATH_API AQbaCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AQbaCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	void OnStaminaUpdated(const FOnAttributeChangeData& Data);

private:
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnStaminaUpdated StaminaUpdated;

	float StaminaValue{0.f};
	float MaxStaminaValue{0.f};
	const float StartingStamina{ 200.f }; 

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Dash(const FInputActionValue& InputActionValue);
	void Input_Sprint(const FInputActionValue& InputActionValue);
	void Input_BallThrow(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

	void GrantDefaultAbilities();



	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext{ nullptr };
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UQbaInputConfig> InputConfig{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	FPlayerAbilities Abilities{};

	TObjectPtr<UQbaAttributeSet_Basic> BasicAttributes{ nullptr };
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm{ nullptr };

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera{ nullptr };
};

