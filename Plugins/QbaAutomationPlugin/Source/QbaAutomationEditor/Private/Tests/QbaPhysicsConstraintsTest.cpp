// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaPhysicsConstraintsTest.h"


IMPLEMENT_QBA_LATENT_TEST(FQbaPhysicsConstraintsTest, "Qba.Editor.ConstraintsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::EngineFilter);


bool FQbaPhysicsConstraintsTest::PrepareTest()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("PrepareTest assets"));
	return true;
}

bool FQbaPhysicsConstraintsTest::RunTestLogic()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Runlogic assets"));

	ADD_LATENT_AUTOMATION_COMMAND(FQbaTestCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));
	ADD_LATENT_AUTOMATION_COMMAND(FQbaTestCommand2(this));
	ADD_LATENT_AUTOMATION_COMMAND(FQbaTestCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FQbaTestCommand_FinishTest(this));
	return true;
}

bool FQbaPhysicsConstraintsTest::FinishTest()
{
	
	UE_LOG(LogQbaAutomation, Error, TEXT("Finishing test"));
	return true;
}

bool FQbaTestCommand::Update()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("is this running?"));
	return true;
}

bool FQbaTestCommand2::Update()
{
	static int TestInt = 0.f;
	TestInt++;

	UE_LOG(LogQbaAutomation, Warning, TEXT("or this? %i"), TestInt);

	if (TestInt > 5)
	{
		return true;
	}
	return false;
}

