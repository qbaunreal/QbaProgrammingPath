// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"


struct FQbaInputTags
{

public:
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Look_Gamepad;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Fire;
	FGameplayTag InputTag_Aim;
	FGameplayTag InputTag_AbilityWheel;

	FORCEINLINE static const FQbaInputTags& Get() { return InputTags; }
	void AddInputTags();

private:
	static FQbaInputTags InputTags;
};

struct FQbaAbilityTags
{

public:
	FGameplayTag AbilityTag_Movement;
	FGameplayTag AbilityTag_CameraMouse;
	FGameplayTag AbilityTag_CameraGamepad;
	FGameplayTag AbilityTag_Jump;
	FGameplayTag AbilityTag_Sprint;
	FGameplayTag AbilityTag_Dash;
	FGameplayTag AbilityTag_Telekinesis;

	FORCEINLINE static const FQbaAbilityTags& Get() { return AbilityTags; }
	void AddAbilityTags();

private:
	static FQbaAbilityTags AbilityTags;
};