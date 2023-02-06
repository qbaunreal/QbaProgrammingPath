

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPCourseTestActor.generated.h"

/**
 * Actor used for the CPP course 
 */
UCLASS()
class QBAPROGRAMMINGPATH_API ACPPCourseTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPPCourseTestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
