
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QbaGameplayTags.h"
#include "QbaCharacter.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UQbaInputConfig;
struct FInputActionValue;

UCLASS()
class QBAPROGRAMMINGPATH_API AQbaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AQbaCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UQbaInputConfig> InputConfig;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Look_Gamepad(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Fire(const FInputActionValue& InputActionValue);
	void Input_Aim(const FInputActionValue& InputActionValue);
	void Input_AbilityWheel(const FInputActionValue& InputActionValue);

private:

	TObjectPtr<USkeletalMeshComponent> PlayerSkeletalMesh;
};

