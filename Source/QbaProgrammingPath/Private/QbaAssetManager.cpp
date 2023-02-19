// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaAssetManager.h"

UQbaAssetManager& UQbaAssetManager::Get()
{
	check(GEngine);

	TObjectPtr<UQbaAssetManager> MyAssetManager = Cast<UQbaAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}

void UQbaAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	InputTags = FQbaInputTags::Get();
	InputTags.AddInputTags();

	AbilityTags = FQbaAbilityTags::Get();
	AbilityTags.AddAbilityTags();
}

