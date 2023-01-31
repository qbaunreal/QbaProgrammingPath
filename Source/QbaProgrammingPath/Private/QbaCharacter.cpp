
#include "QbaCharacter.h"
#include "QbaEnhancedInputComponent.h"
#include "QbaCameraSystemComponent.h"
#include "QbaAssetManager.h"
#include "QbaAbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "InputActionValue.h"



AQbaCharacter::AQbaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSystem = CreateDefaultSubobject<UQbaCameraSystemComponent>(TEXT("Camera System"));
	AbilitySystem = CreateDefaultSubobject<UQbaAbilitySystemComponent>(TEXT("Ability System"));

	check(CameraSystem);
	CameraSystem->Construct(this);
}

void AQbaCharacter::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* AQbaCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void AQbaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQbaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	TObjectPtr<UQbaEnhancedInputComponent> EnhancedInputComponent = Cast<UQbaEnhancedInputComponent>(PlayerInputComponent);
	TObjectPtr<UQbaAssetManager> AssetManager = Cast<UQbaAssetManager>(GEngine->AssetManager);

	check(EnhancedInputComponent);
	check(InputConfig);
	check (AssetManager);

	FQbaInputTags InputTags = AssetManager->GetInputTags();

	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_Move, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Move);
	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Look);
	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_Look_Gamepad, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Look_Gamepad);
	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_Fire, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Fire);
	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_Jump, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Jump);
	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_Aim, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Aim);
	EnhancedInputComponent->BindActionByTag(InputConfig, InputTags.InputTag_AbilityWheel, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_AbilityWheel);
}

void AQbaCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;
	const FVector2D AxisValue = InputActionValue.Get<FInputActionValue::Axis2D>();
	const FRotator ControllerRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (AxisValue.X != 0.0f)
	{
		const FVector MovementDirection = ControllerRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, AxisValue.X);
	}

	if (AxisValue.Y != 0.0f)
	{
		const FVector MovementDirection = ControllerRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, AxisValue.Y);
	}
}

void AQbaCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;

	FVector2D AxisValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	if (AxisValue.X != 0.0f)
	{
		TurnAtRate(AxisValue.X, MouseLookRate);
	}

	if (AxisValue.Y != 0.0f)
	{
		LookUpAtRate(AxisValue.Y, MouseLookRate);
	}
}

void AQbaCharacter::Input_Look_Gamepad(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;

	FVector2D AxisValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	if (AxisValue.X != 0.0f)
	{
		TurnAtRate(AxisValue.X, GamepadLookRate);
	}

	if (AxisValue.Y != 0.0f)
	{
		LookUpAtRate(AxisValue.Y, GamepadLookRate);
	}
}

void AQbaCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	//NOTE: In order to use Button, pls uncomment a line in FInputActionValue
	bool BoolValue = InputActionValue.Get<FInputActionValue::Button>();

	if (BoolValue) Jump(); //TODO: replace it with GameplayAbility_CharacterJump
	
}

void AQbaCharacter::Input_Fire(const FInputActionValue& InputActionValue)
{
	float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
}

void AQbaCharacter::Input_Aim(const FInputActionValue& InputActionValue)
{
	float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
}

void AQbaCharacter::Input_AbilityWheel(const FInputActionValue& InputActionValue)
{
	bool BoolValue = InputActionValue.Get<FInputActionValue::Button>();
}

void AQbaCharacter::TurnAtRate(float Value, float Rate)
{
	AddControllerYawInput(Value * Rate * GetWorld()->GetDeltaSeconds());
}

void AQbaCharacter::LookUpAtRate(float Value, float Rate)
{
	AddControllerPitchInput(Value * Rate * GetWorld()->GetDeltaSeconds());
}