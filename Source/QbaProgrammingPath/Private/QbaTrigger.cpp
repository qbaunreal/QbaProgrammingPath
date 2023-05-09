// Copyright Jakub Urbanek. All Rights Reserved.


#include "QbaTrigger.h"
#include "Components/BoxComponent.h"
#include "QbaMovingPlatform.h"

AQbaTrigger::AQbaTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger"), false);

	if (TriggerVolume)
	{
		RootComponent = TriggerVolume;
	}
}

void AQbaTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (TriggerVolume)
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AQbaTrigger::OnOverlapBegin);
		TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AQbaTrigger::OnOverlapEnd);
	}
}

void AQbaTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQbaTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap true"));
	if (MovingPlatform)
	{
		if (!MovingPlatform->GetAllowMoving())
		{
			MovingPlatform->SetAllowMoving(true);
			UE_LOG(LogTemp, Warning, TEXT("Moving true"));
		}
	}
}

void AQbaTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MovingPlatform)
	{
		if (MovingPlatform->GetAllowMoving())
		{
			MovingPlatform->SetAllowMoving(false);
		}
	}
}

