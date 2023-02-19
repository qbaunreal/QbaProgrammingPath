// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QbaCameraSystemComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

struct FCameraArmParams
{
public:
	float ArmLenght = 100.f;
	bool bInheritControlRotation = true;
	
	FORCEINLINE static const FCameraArmParams& Get() { return ArmParams; }
private:
	static FCameraArmParams ArmParams;
};

struct FCameraParams
{
public:
	float FOV = 60.f;
	
	FORCEINLINE static const FCameraParams& Get() { return CameraParams; }

private:
	static FCameraParams CameraParams;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QBAPROGRAMMINGPATH_API UQbaCameraSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UQbaCameraSystemComponent();
	void Construct(AActor* Owner);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PassInitParmsToArm();
	void PassInitParamsToCamera();


	UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> CharacterCamera = nullptr;
	UPROPERTY(VisibleAnywhere) TObjectPtr<USpringArmComponent> CameraSpringArm = nullptr;
	
};
