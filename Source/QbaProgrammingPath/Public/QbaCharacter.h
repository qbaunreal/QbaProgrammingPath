
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "QbaCharacter.generated.h"

class USkeletalMeshComponent;
class UQbaCameraSystemComponent;
class UQbaInputConfig;
class UAbilitySystemComponent;
class UQbaAbilitySystemComponent;
struct FInputActionValue;

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

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UQbaInputConfig> InputConfig;

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

};

