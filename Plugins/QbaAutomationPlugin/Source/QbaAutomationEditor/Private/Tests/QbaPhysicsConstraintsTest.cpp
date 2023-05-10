// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaPhysicsConstraintsTest.h"


IMPLEMENT_QBA_LATENT_TEST(FQbaPhysicsConstraintsTest, "Qba.Editor.ConstraintsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::EngineFilter);


bool FQbaPhysicsConstraintsTest::PrepareTest()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Prepare assets"));
	return true;
}

bool FQbaPhysicsConstraintsTest::RunTestLogic()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Runlogic assets"));
	ADD_LATENT_AUTOMATION_COMMAND(FQbaTestCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));
	ADD_LATENT_AUTOMATION_COMMAND(FQbaTestCommand2(this));
	
	return true;
}

bool FQbaPhysicsConstraintsTest::FinishTest()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Finishing test"));
	return true;
}

bool FQbaTestCommand::Update()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Running Test command1"));
	return true;
}

bool FQbaTestCommand2::Update()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Running test command2"));
	return true;
}

