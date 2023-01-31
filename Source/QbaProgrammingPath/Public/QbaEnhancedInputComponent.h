
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "QbaInputConfig.h"
#include "GameplayTagContainer.h"
#include "QbaEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class QBAPROGRAMMINGPATH_API UQbaEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	//Q: I kinda know how the templates work, but not 100% sure. I feel like know to read them but not sure what are the benefits of using them and when to use them efficiently. 
	//I would like to have a mini-training about those

	template<class UserClass, typename FuncType>
	void BindActionByTag(const UQbaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);
};

//Q: apperantly they are only allowed in .h, cant be moved to cpp ,got a compilation error. Is that the case with template functions? or is it other way to define them in cpp
template<class UserClass, typename FuncType>
void UQbaEnhancedInputComponent::BindActionByTag(const UQbaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag);

	if (!IA) return;
	
	//Q: I would like to learn more about function binding. How do the delegates work etc. Basically how to create a binding from scratch
	BindAction(IA, TriggerEvent, Object, Func);
}