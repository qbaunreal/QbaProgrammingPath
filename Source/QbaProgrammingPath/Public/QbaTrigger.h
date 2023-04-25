// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QbaTrigger.generated.h"

class AQbaMovingPlatform;
class UBoxComponent;

UCLASS()
class QBAPROGRAMMINGPATH_API AQbaTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AQbaTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerVolume{ nullptr };

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AQbaMovingPlatform> MovingPlatform{ nullptr };

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
