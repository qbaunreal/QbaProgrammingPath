// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaCameraSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

FCameraArmParams FCameraArmParams::ArmParams;
FCameraParams FCameraParams::CameraParams;


UQbaCameraSystemComponent::UQbaCameraSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("Camera Arm");
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
}

void UQbaCameraSystemComponent::Construct(AActor* Owner)
{
	PassInitParmsToArm();
	PassInitParamsToCamera();

	if (!Owner)return;
	if (!CameraSpringArm || !CharacterCamera) return;

	CameraSpringArm->SetupAttachment(Owner->GetRootComponent());
	CharacterCamera->SetupAttachment(CameraSpringArm);
}

void UQbaCameraSystemComponent::PassInitParmsToArm()
{
	CameraSpringArm->TargetArmLength = FCameraArmParams::Get().ArmLenght;
	CameraSpringArm->bUsePawnControlRotation = FCameraArmParams::Get().bInheritControlRotation;
}

void UQbaCameraSystemComponent::PassInitParamsToCamera()
{
	CharacterCamera->FieldOfView = FCameraParams::Get().FOV;
}

void UQbaCameraSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQbaCameraSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

