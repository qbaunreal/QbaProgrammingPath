// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "QbaTestBase.h"

class FQbaPhysicsConstraintsTest : public FQbaTestRunnerBase
{
public:
	// Check if initialized properly, loading environment, required assets etc.
	virtual bool PrepareTest() override;

	// Run logic of your test here 
	virtual bool RunTestLogic() override;

	// Delete all necessary assets, finish test logic
	virtual bool FinishTest() override;

	bool bFinishTest{ false };
};

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FQbaTestCommand, FQbaPhysicsConstraintsTest*, Test);
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FQbaTestCommand2, FQbaPhysicsConstraintsTest*, Test);

