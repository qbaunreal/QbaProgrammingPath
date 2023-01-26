

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QbaCameraManager.generated.h"

class UCameraComponent;
class USpringArmComponent;

struct FCameraArmParams 
{
	float ArmLenght = 100.f;
};

struct FCameraParams
{
	float FOV = 60.f;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QBAPROGRAMMINGPATH_API UQbaCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQbaCameraManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:

	TObjectPtr<UCameraComponent> CharacterCamera = nullptr;
	TObjectPtr<USpringArmComponent> CameraSpringArm = nullptr;

};
