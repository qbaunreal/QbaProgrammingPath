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

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "QbaGameplayAbility_Sprint.h"
#include "QbaGameplayAbility_Dash.h"
#include "QbaGameplayAbility_BallThrow.h"
#include "QbaGameplayAbility_Crouch.h"

DEFINE_LOG_CATEGORY(QbaAbilities);

AQbaCharacter::AQbaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Abilities.AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System"));

	Abilities.AbilitySystem->SetIsReplicated(true);
	BasicAttributes = CreateDefaultSubobject<UQbaAttributeSet_Basic>(TEXT("Basic Attributes"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	if (SpringArm && RootComponent)
	{
		const FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative, 
			EAttachmentRule::KeepRelative, 
			false);
		
		SpringArm->AttachToComponent(RootComponent, AttachmentRules);
		SpringArm->bUsePawnControlRotation = true;

		SpringArm->TargetArmLength = 500.f;
		if (Camera)
		{
			Camera->AttachToComponent(SpringArm, AttachmentRules);
		}
	}
}

void AQbaCharacter::BeginPlay()
{
	Super::BeginPlay();

	GrantDefaultAbilities();

	if (BasicAttributes)
	{
		BasicAttributes->SetMaxStamina(StartingStamina);
		BasicAttributes->SetStamina(StartingStamina);

		Abilities.AbilitySystem->GetGameplayAttributeValueChangeDelegate(BasicAttributes->GetStaminaAttribute()).AddUObject(this, &AQbaCharacter::OnStaminaUpdated);
	}
}

void AQbaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (Abilities.AbilitySystem)
	{
		Abilities.AbilitySystem->InitAbilityActorInfo(this, this);
		Abilities.AbilitySystem->RefreshAbilityActorInfo();
	}
}

UAbilitySystemComponent* AQbaCharacter::GetAbilitySystemComponent() const
{
	return Abilities.AbilitySystem;
}

void AQbaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQbaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	EnhancedSubsystem->ClearAllMappings();
	EnhancedSubsystem->AddMappingContext(DefaultMappingContext, 0);
	TObjectPtr<UEnhancedInputComponent> EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInput && InputConfig)
	{
		EnhancedInput->BindAction(InputConfig->InputMove, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Move);
		EnhancedInput->BindAction(InputConfig->InputLook, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Look);
		EnhancedInput->BindAction(InputConfig->InputJump, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Jump);
		EnhancedInput->BindAction(InputConfig->InputDash, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Dash);
		EnhancedInput->BindAction(InputConfig->InputSprint, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Sprint);
		EnhancedInput->BindAction(InputConfig->InputBallThrow, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_BallThrow);
		EnhancedInput->BindAction(InputConfig->InputCrouch, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Crouch);
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
	if (Abilities.JumpAbility && Abilities.AbilitySystem)
	{
		const bool bInputAllowJump = InputActionValue.Get<FInputActionValue::Button>();
		const bool bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

		if (bInputAllowJump && bIsGrounded)
		{
			Abilities.AbilitySystem->TryActivateAbilityByClass(Abilities.JumpAbility);
		}
	}
}

void AQbaCharacter::Input_Dash(const FInputActionValue& InputActionValue)
{
	if (Abilities.DashAbility && Abilities.AbilitySystem)
	{
		const float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
		UE_LOG(QbaAbilities, Log, TEXT("Dash value is: %f"), AxisValue);
		Abilities.AbilitySystem->TryActivateAbilityByClass(Abilities.DashAbility);

	}
}

void AQbaCharacter::Input_Sprint(const FInputActionValue& InputActionValue)
{
	if (Abilities.SprintAbility && Abilities.AbilitySystem)
	{
		const float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
		UE_LOG(QbaAbilities, Log, TEXT("SprintAbility value is: %f"), AxisValue);
		Abilities.AbilitySystem->TryActivateAbilityByClass(Abilities.SprintAbility);
	}
}

void AQbaCharacter::Input_BallThrow(const FInputActionValue& InputActionValue)
{
	if (Abilities.BallThrowAbility && Abilities.AbilitySystem)
	{
		const float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
		UE_LOG(QbaAbilities, Log, TEXT("BallThrowAbility value is: %f"), AxisValue);
		Abilities.AbilitySystem->TryActivateAbilityByClass(Abilities.BallThrowAbility);
	}
}

void AQbaCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (Abilities.CrouchAbility && Abilities.AbilitySystem)
	{
		const float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();
		UE_LOG(QbaAbilities, Log, TEXT("CrouchAbility value is: %f"), AxisValue);
		Abilities.AbilitySystem->TryActivateAbilityByClass(Abilities.CrouchAbility);
	}
}

void AQbaCharacter::GrantDefaultAbilities()
{
	if (Abilities.AbilitySystem)
	{
		if (Abilities.JumpAbility)
		{
			const FGameplayAbilitySpec JumpAbilitySpecs(Abilities.JumpAbility, 1, INDEX_NONE, this);
			Abilities.AbilitySystem->GiveAbility(JumpAbilitySpecs);
		}
		if (Abilities.DashAbility)
		{
			const FGameplayAbilitySpec DashAbilitySpecs(Abilities.DashAbility, 1, INDEX_NONE, this);
			Abilities.AbilitySystem->GiveAbility(DashAbilitySpecs);
		}
		if (Abilities.SprintAbility)
		{
			const FGameplayAbilitySpec SprintAbilitySpecs(Abilities.SprintAbility, 1, INDEX_NONE, this);
			Abilities.AbilitySystem->GiveAbility(Abilities.SprintAbility);
		}
		if (Abilities.BallThrowAbility)
		{
			const FGameplayAbilitySpec BallThrowSpecs(Abilities.BallThrowAbility, 1, INDEX_NONE, this);
			Abilities.AbilitySystem->GiveAbility(Abilities.BallThrowAbility);
		}
		if (Abilities.CrouchAbility)
		{
			const FGameplayAbilitySpec CrouchAbilitySpecs(Abilities.CrouchAbility, 1, INDEX_NONE, this);
			Abilities.AbilitySystem->GiveAbility(Abilities.CrouchAbility);
		}
	}
}


void AQbaCharacter::OnStaminaUpdated(const FOnAttributeChangeData& Data)
{
	StaminaUpdated.Broadcast(Data.NewValue, GetMaxStamina());
}

float AQbaCharacter::GetMaxStamina() const
{ 
	if (Abilities.AbilitySystem)
	{ 
		return  Abilities.AbilitySystem->GetNumericAttribute(BasicAttributes->GetMaxStaminaAttribute());
	}
	return 0.f;
}

float AQbaCharacter::GetStamina() const
{
	if (Abilities.AbilitySystem)
	{
		return  Abilities.AbilitySystem->GetNumericAttribute(BasicAttributes->GetStaminaAttribute());
	}
	return 0.f;
}