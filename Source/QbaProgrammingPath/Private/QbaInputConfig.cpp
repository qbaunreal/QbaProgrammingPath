
#include "QbaInputConfig.h"
#include "InputAction.h"


const UInputAction* UQbaInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTaggedInputAction& TaggedAction : TaggedInputActions)
	{
		//Q: This for some reason does not work after compiling the engine. It's fine after the restart but somehow looses reference to the InputAction when compiling.
		// Looks OK in the data asset itsef :/ 
		// Pls help

		if (!TaggedAction.InputAction) { UE_LOG(LogTemp, Error, TEXT("Input Action is not valid")); }
		/*else
		{
			UE_LOG(LogTemp, Error, TEXT("Input Action is valid"));
		}*/
		/*UE_LOG(LogTemp, Warning, TEXT("TaggedInputAction.InputAction: %s"), *ActionToCheck.InputAction->GetName());*/
		/*UE_LOG(LogTemp, Warning, TEXT("TaggedInputAction.InputTag: %s"), *TaggedAction.InputTag.ToString());
		UE_LOG(LogTemp, Warning, TEXT("InputTag: %s"), *InputTag.ToString());*/


		if (TaggedAction.InputAction && TaggedAction.InputTag == InputTag)
		{
			return TaggedAction.InputAction;
		}
	}

	return nullptr;
}