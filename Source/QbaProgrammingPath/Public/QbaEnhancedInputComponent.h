// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "QbaInputConfig.h"
#include "GameplayTagContainer.h"
#include "QbaEnhancedInputComponent.generated.h"


UCLASS()
class QBAPROGRAMMINGPATH_API UQbaEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, typename FuncType>
	void BindActionByTag(const UQbaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);
};

template<class UserClass, typename FuncType>
void UQbaEnhancedInputComponent::BindActionByTag(const UQbaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag);

	if (!IA) 
	{ 
		return; 
	}
	
	BindAction(IA, TriggerEvent, Object, Func);
}