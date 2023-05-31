// Copyright Jakub Urbanek. All Rights Reserved.

#include "QbaPhysicsConstraintsTest.h"

#if WITH_EDITOR
#include "ActorFactories/ActorFactory.h"
#include "BlueprintEditor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Editor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraphSchema_K2.h"
#include "QbaBlueprintHelpers.h"


IMPLEMENT_QBA_LATENT_TEST(FQbaPhysicsConstraintsTest, "Qba.Editor.ConstraintsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriority | EAutomationTestFlags::SmokeFilter);

//TODO: move helper functions to to more proper place


/* TEST OUTLINE:
* 
* 1. +Create blueprint asset
* 2. +Open its graph
* 3. Add proper nodes to the graph (2x static mesh, 1 constraint). 
* 4. Set proper params of the items
* 5. +Compile and Save the asset
* 6. Place it on the level 
* 7. PIE
* 8. Check if constraint is functional
* 9. End PIE
* 10. Delete assets before and after runtestlogic
*/

bool FQbaPhysicsConstraintsTest::RunTestLogic()
{
	FQbaTestRunnerBase::RunTestLogic();

	ADD_LATENT_AUTOMATION_COMMAND(SpawnConstraintActorBlueprint(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
	ADD_LATENT_AUTOMATION_COMMAND(OpenBlueprintGraph(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
	ADD_LATENT_AUTOMATION_COMMAND(AddNodesToGraph(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(10.f));
	ADD_LATENT_AUTOMATION_COMMAND(SaveSpawnedBlueprints(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
	
	return true;
}


PRAGMA_DISABLE_OPTIMIZATION
bool SpawnConstraintActorBlueprint::Update()
{
	FAutomationTestBase& TestRunner = Test->GetTestRunner();

	UE_LOG(LogQbaAutomation, Log, TEXT("Creating ConstraintActor asset"));
	//3. Set it's params

	FQbaTestHelpers::FAssetCreationData AssetCreationData;
	AssetCreationData.AssetClass = AActor::StaticClass();
	AssetCreationData.AssetName = FString::Printf(TEXT("TestAsset"));
	AssetCreationData.AssetPath = Test->GetAssetSaveLocation();

	UPackage* AssetPackage{ nullptr };

	Test->ConstraintActorBlueprint = FQbaTestHelpers::CreateBlueprint(AssetCreationData, AssetPackage);

	TestRunner.TestNotNull(FString(TEXT("ConstraintActor")), Test->ConstraintActorBlueprint);

	
	if (Test->ConstraintActorBlueprint)
	{
		Test->AddAssetToTestAssets(Test->ConstraintActorBlueprint);
	}
	
	return true;
}

bool OpenBlueprintGraph::Update()
{
	if (Test->ConstraintActorBlueprint)
	{
		FAutomationTestBase& TestRunner = Test->GetTestRunner();

		// Find valid Blueprint editor and open graph
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Test->ConstraintActorBlueprint);
		IAssetEditorInstance* AssetEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(Test->ConstraintActorBlueprint, true);
		FBlueprintEditor* BlueprintEditor = static_cast<FBlueprintEditor*>(AssetEditor);

		if (!BlueprintEditor)
		{
			return true;
		}
		TestRunner.TestNotNull(FString(TEXT("BlueprintEditor")), BlueprintEditor);

		Test->EventGraph = FBlueprintEditorUtils::FindEventGraph(Test->ConstraintActorBlueprint);
		TestRunner.TestNotNull(FString(TEXT("EventGraph pointer")), Test->EventGraph);

		if (!Test->EventGraph)
		{
			return true;
		}

		TSharedPtr<SGraphEditor> Graph = BlueprintEditor->OpenGraphAndBringToFront(Test->EventGraph);
		TestRunner.TestNotNull(FString(TEXT("Graph pointer")), Graph.Get());

		const bool bIsGraphVisible = Graph.Get()->GetVisibility().IsVisible();
		TestRunner.TestTrue(FString(TEXT("Is Graph visible")), bIsGraphVisible);
	}
	return true;
}


bool AddNodesToGraph::Update()
{
	FAutomationTestBase& TestRunner = Test->GetTestRunner();

	if (!Test->ConstraintActorBlueprint || !Test->EventGraph)
	{
		TestRunner.TestNotNull(FString(TEXT("ConstraintActorBlueprint")), Test->ConstraintActorBlueprint);
		TestRunner.TestNotNull(FString(TEXT("EventGraph")), Test->EventGraph);
		return true;
	}


	//Find beginplay node, add debug print string node
	UEdGraphNode* BeginPlayNode = QbaBPTestHelpers::FindGraphNode(Test->EventGraph, TEXT("BeginPlay"));
	TestRunner.TestNotNull(FString(TEXT("BeginPlay node pointer")), BeginPlayNode);
	if (!BeginPlayNode)
	{
		return true;
	}

	UEdGraphPin* const BeginPlayExecPin = BeginPlayNode->FindPin(TEXT("then"));
	TestRunner.TestNotNull(FString(TEXT("BeginPlay exec pin")), BeginPlayExecPin);
	if (!BeginPlayExecPin)
	{
		return true;
	}

	// Spawn PrintString node for debugging purpouse
	const float NodeXOffset{ 200.f };
	const float NodeYOffset{ 10.f };
	const FVector2D PrintNodeLocation = FVector2D(BeginPlayNode->NodePosX + NodeXOffset, BeginPlayNode->NodePosY + NodeYOffset);
	UEdGraphNode* PrintStringNode = QbaBPTestHelpers::AddPrintStringNode(Test->ConstraintActorBlueprint, Test->EventGraph, PrintNodeLocation,BeginPlayExecPin);
	TestRunner.TestNotNull(FString(TEXT("PrintStringNode")), PrintStringNode);
	if (!BeginPlayExecPin)
	{
		return true;
	}

	return true;
}


bool SaveSpawnedBlueprints::Update()
{
	if (Test->ConstraintActorBlueprint)
	{
		FBlueprintEditorUtils::RefreshAllNodes(Test->ConstraintActorBlueprint);
		FKismetEditorUtilities::CompileBlueprint(Test->ConstraintActorBlueprint, EBlueprintCompileOptions::SkipGarbageCollection | EBlueprintCompileOptions::SkipSave);

		const bool bCorrectlyCompiledBlueprint = Test->ConstraintActorBlueprint->Status == EBlueprintStatus::BS_UpToDate;
		Test->GetTestRunner().TestTrue(FString(TEXT("Correctly compiled blueprint")), bCorrectlyCompiledBlueprint);

		const bool bCorrectlySavedAsset = FQbaTestHelpers::SaveAsset(Test->ConstraintActorBlueprint);
		Test->GetTestRunner().TestTrue(FString(TEXT("Saving constraint actor blueprint")), bCorrectlySavedAsset);
	}
	return true;
}
PRAGMA_ENABLE_OPTIMIZATION

#endif // WITH_EDITOR

