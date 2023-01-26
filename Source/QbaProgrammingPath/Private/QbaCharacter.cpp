
#include "QbaCharacter.h"
#include "QbaEnhancedInputComponent.h"
#include "QbaInputConfig.h"
#include "QbaAssetManager.h"
#include "GameplayTagsManager.h"
#include "InputActionValue.h"



AQbaCharacter::AQbaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/*TObjectPtr<UQbaAssetManager> AssetManager = Cast<UQbaAssetManager>(GEngine->AssetManager);
	if (AssetManager)
	{
		LocalGameplayTags = AssetManager->GetTags();
	}*/
	
}

void AQbaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AQbaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (LocalGameplayTags.InputTag_Move.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Is valid"));
	}*/
}

void AQbaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UQbaEnhancedInputComponent* EnhancedInputComponent = Cast<UQbaEnhancedInputComponent>(PlayerInputComponent);

	if(!EnhancedInputComponent) return;
	UE_LOG(LogTemp, Error, TEXT("aaaa"));

	if(!InputConfig) return;
	UE_LOG(LogTemp, Error, TEXT("BBB"));

	auto AssetManager = Cast<UQbaAssetManager>(GEngine->AssetManager);
	if (!AssetManager) return;
	FQbaGameplayTags GameplayTags = AssetManager->GetTags();

	UE_LOG(LogTemp, Error, TEXT("CharacterTag: %s"), *GameplayTags.InputTag_Move.GetTagName().ToString());
	if (GameplayTags.InputTag_Move.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Is valid"));
	}

	

	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Move);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Look);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Look_Gamepad, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Look_Gamepad);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Fire, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Fire);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Jump);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_Aim);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_AbilityWheel, ETriggerEvent::Triggered, this, &AQbaCharacter::Input_AbilityWheel);
}

void AQbaCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	FVector2D AxisValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	UE_LOG(LogTemp, Warning, TEXT("Input_Move: %s"), *AxisValue.ToString());
}

void AQbaCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	FVector2D AxisValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	UE_LOG(LogTemp, Warning, TEXT("Input_Look: %s"), *AxisValue.ToString());
}


void AQbaCharacter::Input_Look_Gamepad(const FInputActionValue& InputActionValue)
{
	FVector2D AxisValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	UE_LOG(LogTemp, Warning, TEXT("Input_Look_Gamepad: %s"), *AxisValue.ToString());
}

void AQbaCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	bool BoolValue = InputActionValue.Get<FInputActionValue::Button>();

	UE_LOG(LogTemp, Warning, TEXT("Input_Jump is %s"), (BoolValue ? TEXT("true") : TEXT("false")));
}

void AQbaCharacter::Input_Fire(const FInputActionValue& InputActionValue)
{
	float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();

	UE_LOG(LogTemp, Warning, TEXT("Input_Fire: %f"), AxisValue);
}

void AQbaCharacter::Input_Aim(const FInputActionValue& InputActionValue)
{
	float AxisValue = InputActionValue.Get<FInputActionValue::Axis1D>();

	UE_LOG(LogTemp, Warning, TEXT("Input_Aim: %f"), AxisValue);
}

void AQbaCharacter::Input_AbilityWheel(const FInputActionValue& InputActionValue)
{
	bool BoolValue = InputActionValue.Get<FInputActionValue::Button>();

	UE_LOG(LogTemp, Warning, TEXT("Input_AbilityWheel: %s"), (BoolValue ? TEXT("true") : TEXT("false")));
}

