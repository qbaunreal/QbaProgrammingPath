// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaPhysicsConstraintsTest.h"
#include "ActorFactories/ActorFactory.h"


IMPLEMENT_QBA_LATENT_TEST(FQbaPhysicsConstraintsTest, "Qba.Editor.ConstraintsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriority | EAutomationTestFlags::SmokeFilter);


bool FQbaPhysicsConstraintsTest::PrepareTest()
{
	//1. Check if there are assets already in the folder, if so. try to delete them
	//2. Create actor
	//3. Set it's params

	UE_LOG(LogQbaAutomation, Warning, TEXT("Prepare assets"));
	return true;
}

bool FQbaPhysicsConstraintsTest::RunTestLogic()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Runlogic assets"));
	ADD_LATENT_AUTOMATION_COMMAND(SpawnTestBlueprints(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));
	ADD_LATENT_AUTOMATION_COMMAND(SaveSpawnedBlueprints(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));
	
	return true;
}

bool FQbaPhysicsConstraintsTest::FinishTest()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Finishing test"));
	return FQbaTestRunnerBase::FinishTest();;
}

bool SpawnTestBlueprints::Update()
{
	UE_LOG(LogQbaAutomation, Warning, TEXT("Creating asset"));
	
	for (int32 i = 0; i < 10; i++) // for tests
	{
		FQbaTestHelpers::FAssetCreationData AssetCreationData;
		AssetCreationData.AssetClass = AActor::StaticClass();
		AssetCreationData.AssetName = FString::Printf(TEXT("TestAsset%i"), i);
		AssetCreationData.AssetPath = FQbaTestHelpers::GetPathForAsset(FString(TEXT("ConstraintTest")));

		UPackage* AssetPackage{ nullptr };

		TObjectPtr<UObject>CreatedObject = FQbaTestHelpers::CreateBlueprint(AssetCreationData, AssetPackage);
		if (CreatedObject)
		{
			Test->AddAssetToTestAssets(CreatedObject);
		}
	}
	return true;
}

bool SaveSpawnedBlueprints::Update()
{
	Test->SaveAllTestAssets();
	return true;
}

