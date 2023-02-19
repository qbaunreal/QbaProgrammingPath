// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QbaPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class QBAPROGRAMMINGPATH_API AQbaPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

private:
	void AddDefaultMappingContext(const TObjectPtr<UInputMappingContext> ContextToMap) const;

};
