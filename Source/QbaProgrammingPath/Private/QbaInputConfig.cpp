
#include "QbaInputConfig.h"
#include "InputAction.h"


const UInputAction* UQbaInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTaggedInputAction& TaggedAction : TaggedInputActions)
	{
		if (TaggedAction.InputAction && TaggedAction.InputTag == InputTag)
		{
			return TaggedAction.InputAction;
		}
	}

	return nullptr;
}