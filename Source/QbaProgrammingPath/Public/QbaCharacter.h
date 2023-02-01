
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

//Q: why Can't I forward declare these two structs but can FInputActionValue?
// Is that because FInputActionValue is in function params?
//struct FQbaAbilityTags;
//struct FQbaInputTags;

//Q: In Delegates, does the Parameters need to be defined variables? I mean I am not even using them (StaminaValue and MaxStaminaValue) 
// but needed to add them because of the compilation errors
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChange, float, StaminaValue, float, MaxStaminaValue);

UCLASS()
class QBAPROGRAMMINGPATH_API AQbaCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AQbaCharacter();


protected:
	virtual void BeginPlay() override;

	//Q: In in the interface there is const = 0, what does it mean?
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UQbaInputConfig> InputConfig;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	// The callback to be registered within AbilitySystem.
	void OnStaminaUpdated(const FOnAttributeChangeData& Data);
	
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnStaminaChange OnStaminaChange;

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

	UPROPERTY(VisibleAnywhere) TObjectPtr<UQbaCameraSystemComponent> CameraSystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Abilities")
	TObjectPtr<UQbaAbilitySystemComponent> AbilitySystem;

	// TODO: maybe pack them to struct
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	float MouseLookRate = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GamepadLookRate = 50.f;

	//Q: I don't want to store an pinpoint exact abilities. Is it a way to grant them by tag? I know that you can activate/find them if they are already present, but I'm not so sure about giving them.
	// at least there are no overloads of GiveAbility or FGameplayAbilitySpec that i am aware of
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> JumpAbility;

	FQbaAbilityTags AbilityTags;
	FQbaInputTags InputTags;

	TObjectPtr<UQbaAttributeSet_Basic> BasicAttributes;
	float StaminaValue;
	float MaxStaminaValue;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> BasicStatsHUD;*/

};

