
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
//#include "QbaGameplayTags.generated.h"

struct FQbaGameplayTags
{

public:
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Look_Gamepad;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Fire;
	FGameplayTag InputTag_Aim;
	FGameplayTag InputTag_AbilityWheel;

	FORCEINLINE static const FQbaGameplayTags& Get() { return GameplayTags; }
	void AddTags();


private:
	//Q: I need to do a little more reasearch. It's first time I'm using it and not sure 100% how it works
	static FQbaGameplayTags GameplayTags;
};

//USTRUCT()
//struct FQbaGameplayTags
//{
//	GENERATED_USTRUCT_BODY()
//
//public:
//	UPROPERTY(EditAnywhere)
//	FGameplayTag InputTag_Move;
//	FGameplayTag InputTag_Look_Mouse;
//	FGameplayTag InputTag_Look_Gamepad;
//	FGameplayTag InputTag_Jump;
//	FGameplayTag InputTag_Fire;
//	FGameplayTag InputTag_Aim;
//	FGameplayTag InputTag_AbilityWheel;
//
//	void AddTags();
//
//};

