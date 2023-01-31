
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "QbaGameplayTags.h"
#include "QbaAssetManager.generated.h"


UCLASS()
class QBAPROGRAMMINGPATH_API UQbaAssetManager : public UAssetManager
{
	GENERATED_BODY()

	static UQbaAssetManager& Get();

public:
	FORCEINLINE const FQbaInputTags& GetInputTags() { return InputTags; };
	FORCEINLINE const FQbaAbilityTags& GetAbilityTags() { return AbilityTags; };

protected:

	virtual void StartInitialLoading() override;

public:
	FQbaInputTags InputTags;
	FQbaAbilityTags AbilityTags;
};
