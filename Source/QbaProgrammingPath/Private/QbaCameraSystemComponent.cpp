

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

	//ensure when doing AttachToComponent as its no longer part of this constructor
	/*CameraSpringArm->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("")));
	CharacterCamera->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("")));*/

	CameraSpringArm->SetupAttachment(Owner->GetRootComponent());
	CharacterCamera->SetupAttachment(CameraSpringArm);
}

void UQbaCameraSystemComponent::PassInitParmsToArm()
{
	CameraSpringArm->TargetArmLength = FCameraArmParams::Get().ArmLenght;
	CameraSpringArm->bUsePawnControlRotation = FCameraArmParams::Get().bInheritControlRotation;
	//etc.
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

