// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QbaGameplayAbility.generated.h"

class UInputAction;


UCLASS()
class QBAPROGRAMMINGPATH_API UQbaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UInputAction* InputAction;
};
