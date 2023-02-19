// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaInputConfig.h"

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