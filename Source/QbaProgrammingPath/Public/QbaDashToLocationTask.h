// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"
#include "QbaDashToLocationTask.generated.h"


class UGameplayTasksComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashToLocationDelegate);


/**
 *
 */

UCLASS()
class QBAPROGRAMMINGPATH_API UQbaDashToLocationTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	UQbaDashToLocationTask(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FDashToLocationDelegate	OnTargetLocationReached;

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	static UQbaDashToLocationTask* DashToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:

	bool bIsFinished;

	UPROPERTY(Replicated)
	FVector StartLocation;


	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	float DurationOfMovement;

	float TimeMoveStarted;

	float TimeMoveWillEnd;
};
