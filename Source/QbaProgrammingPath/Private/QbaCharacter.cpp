// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "QbaAttributeSet_Basic.h"

#include "QbaInputConfig.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AQbaCharacter::AQbaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System"));

	AbilitySystem->SetIsReplicated(true);
	BasicAttributes = CreateDefaultSubobject<UQbaAttributeSet_Basic>(TEXT("Basic Attributes"));
}

void AQbaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystem)
	{
		if (JumpAbility)
		{
			FGameplayAbilitySpec JumpAbilitySpecs(JumpAbility, 1, INDEX_NONE, this);
			AbilitySystem->GiveAbility(JumpAbilitySpecs);
		}
		if (BasicAttributes)
		{
			BasicAttributes->SetMaxStamina(StartingStamina);
			BasicAttributes->SetStamina(StartingStamina);

			AbilitySystem->GetGameplayAttributeValueChangeDelegate(BasicAttributes->GetStaminaAttribute()).AddUObject(this, &AQbaCharacter::OnStaminaUpdated);
		}
	}
}

void AQbaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
		AbilitySystem->RefreshAbilityActorInfo();
	}
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
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	EnhancedSubsystem->ClearAllMappings();
	EnhancedSubsystem->AddMappingContext(DefaultMappingContext, 0);
	TObjectPtr<UEnhancedInputComponent> EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInput && InputConfig)
	{
		EnhancedInput->BindAction(InputConfig->InputMove, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Move);
		EnhancedInput->BindAction(InputConfig->InputLook, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Look);
		EnhancedInput->BindAction(InputConfig->InputFire, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Fire);
		EnhancedInput->BindAction(InputConfig->InputAim, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Aim);
		EnhancedInput->BindAction(InputConfig->InputJump, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Jump);
	}
}

void AQbaCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller)
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		if (MoveValue.Y != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(Direction, MoveValue.Y);
		}

		if (MoveValue.X != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void AQbaCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	if (Controller)
	{
		const FVector2D LookValue = InputActionValue.Get<FVector2D>();

		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}

void AQbaCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (JumpAbility && AbilitySystem)
	{
		const bool bInputAllowJump = InputActionValue.Get<FInputActionValue::Button>();
		const bool bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

		if (bInputAllowJump && bIsGrounded)
		{
			AbilitySystem->TryActivateAbilityByClass(JumpAbility);
			UE_LOG(LogTemp, Warning, TEXT("GetStamina good: %f"), AbilitySystem->GetNumericAttribute(BasicAttributes->GetStaminaAttribute()));
			UE_LOG(LogTemp, Warning, TEXT("max good: %f"), AbilitySystem->GetNumericAttribute(BasicAttributes->GetMaxStaminaAttribute()));
		}
	}
}

void AQbaCharacter::Input_Fire(const FInputActionValue& InputActionValue)
{
	const float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
}

void AQbaCharacter::Input_Aim(const FInputActionValue& InputActionValue)
{
	const float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
}


void AQbaCharacter::OnStaminaUpdated(const FOnAttributeChangeData& Data)
{
	StaminaUpdated.Broadcast(Data.NewValue, GetMaxStamina());
}

float AQbaCharacter::GetMaxStamina() const
{ 
	if (AbilitySystem) 
	{ 
		return  AbilitySystem->GetNumericAttribute(BasicAttributes->GetMaxStaminaAttribute());
	}
	return 0.f;
}

float AQbaCharacter::GetStamina() const
{
	if (AbilitySystem)
	{
		return  AbilitySystem->GetNumericAttribute(BasicAttributes->GetStaminaAttribute());
	}
	return 0.f;
}