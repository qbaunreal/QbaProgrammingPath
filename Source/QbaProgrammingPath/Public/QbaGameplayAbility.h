

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QbaGameplayAbility.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class QBAPROGRAMMINGPATH_API UQbaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UInputAction* InputAction;
};

// TODOS:
// 1. Add InputAction for the Ability so they are bound to the enhanced input actions such as "Ablity 1 / Ablity2 etc."