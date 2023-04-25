// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "QbaMovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class QBAPROGRAMMINGPATH_API AQbaMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AQbaMovingPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetAllowMoving(bool bNewValue) { bAllowMoving = bNewValue; SetActorTickEnabled(bNewValue); };
	FORCEINLINE const bool GetAllowMoving() const { return bAllowMoving; };

private:
	void MoveForward(float DeltaTime);

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	float MovementSpeed{ 100.f };

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation {FVector::ZeroVector};

	FVector WorldStartLocation{ FVector::ZeroVector };

	FVector WorldTargetLocation{ FVector::ZeroVector };

	bool bAllowMoving{ false };

};
