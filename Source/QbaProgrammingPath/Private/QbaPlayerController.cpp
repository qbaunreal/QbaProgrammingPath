// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaPlayerController.h"
#include "EnhancedInputSubsystems.h"


void AQbaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultMappingContext)
	{
		AddDefaultMappingContext(DefaultMappingContext);
	}
}

void AQbaPlayerController::AddDefaultMappingContext(const TObjectPtr<UInputMappingContext> ContextToMap) const
{
	if (!Cast<UEnhancedPlayerInput>(this->PlayerInput)) return;
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	if (EnhancedInputSubsystem)
	{
		int32 MappingContextPriority = 0;
		FModifyContextOptions MappingOptions;

		EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, MappingContextPriority, MappingOptions);
	}
}
