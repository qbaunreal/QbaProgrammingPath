// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "QbaTestBase.h"

#if WITH_EDITOR

class UEdGraph;

class FQbaPhysicsConstraintsTest : public FQbaTestRunnerBase
{
public:
	virtual bool RunTestLogic() override;

	
	UEdGraph* EventGraph{ nullptr };
	UBlueprint* ConstraintActorBlueprint{ nullptr };
};

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(SpawnConstraintActorBlueprint, FQbaPhysicsConstraintsTest*, Test);
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(OpenBlueprintGraph, FQbaPhysicsConstraintsTest*, Test);
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(AddNodesToGraph, FQbaPhysicsConstraintsTest*, Test);
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(SaveSpawnedBlueprints, FQbaPhysicsConstraintsTest*, Test);

#endif // WITH_EDITOR