
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
	
	GameplayTags = FQbaGameplayTags::Get();
	GameplayTags.AddTags();
}

