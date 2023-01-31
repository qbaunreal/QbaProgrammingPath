
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "QbaInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "InputTag"))
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag::EmptyTag;
};


UCLASS(BlueprintType)
class QBAPROGRAMMINGPATH_API UQbaInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaggedInputAction> TaggedInputActions;	
};
