

#include "QbaCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

UQbaCameraManager::UQbaCameraManager()
{

	PrimaryComponentTick.bCanEverTick = false;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
}


void UQbaCameraManager::BeginPlay()
{
	Super::BeginPlay();

	
}


void UQbaCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

