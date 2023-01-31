
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
//#include "QbaGameplayTags.generated.h"


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
	//Q: I need to do a little more reasearch. I have never declared a struct like that and im' not 100% sure how it works. I was only using the UStruct(as below)
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

	// etc. add tags once I know what they'll do, general scope of ability

	FORCEINLINE static const FQbaAbilityTags& Get() { return AbilityTags; }
	void AddAbilityTags();

private:
	static FQbaAbilityTags AbilityTags;
};


//Q: And here is the other method, UStruct base. Not sure which one is better

//USTRUCT()
//struct FQbaInputTags
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

