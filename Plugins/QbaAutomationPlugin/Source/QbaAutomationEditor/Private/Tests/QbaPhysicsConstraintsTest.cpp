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
#include "Engine/SCS_Node.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


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
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));
	ADD_LATENT_AUTOMATION_COMMAND(SaveSpawnedBlueprints(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(12.f));
	
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

	UEdGraphPin* const BeginPlayExecPin = BeginPlayNode->FindPin(UEdGraphSchema_K2::PN_Then);
	TestRunner.TestNotNull(FString(TEXT("BeginPlay exec pin")), BeginPlayExecPin);
	if (!BeginPlayExecPin)
	{
		return true;
	}

	// Spawn PrintString node for debugging purpose
	const float NodeXOffset{ 200.f };
	const float NodeYOffset{ 10.f };
	const FVector2D PrintNodeLocation = FVector2D(BeginPlayNode->NodePosX + NodeXOffset, BeginPlayNode->NodePosY + NodeYOffset);
	UEdGraphNode* PrintStringNode = QbaBPTestHelpers::AddPrintStringNode(Test->ConstraintActorBlueprint, Test->EventGraph, PrintNodeLocation,BeginPlayExecPin);
	TestRunner.TestNotNull(FString(TEXT("PrintStringNode")), PrintStringNode);
	if (!BeginPlayExecPin)
	{
		return true;
	}

	const FString StringVariableName = FString(TEXT("ExampleStringVariable"));
	const FString StringValue = FString(TEXT("Hello, I am your string variable"));
	QbaBPTestHelpers::AddVariable(StringVariableName, StringValue, Test->ConstraintActorBlueprint, Test->EventGraph);

	if (PrintStringNode)
	{
		UEdGraphPin* PrintInString = PrintStringNode->FindPin(TEXT("InString"));
		TestRunner.TestNotNull(FString(TEXT("PrintInString")), PrintInString);

		if (PrintInString)
		{
			const float StringGetXOffset{ 0.f};
			const float StringGetYOffset{ -20.f };
			const FVector2D TargetLocation = FVector2D(PrintStringNode->NodePosX + StringGetXOffset, PrintStringNode->NodePosY + StringGetYOffset);
			UEdGraphNode* const PrintNode =  QbaBPTestHelpers::AddGetSetNode(Test->ConstraintActorBlueprint, Test->EventGraph, StringVariableName, true, TargetLocation);
			TestRunner.TestNotNull(FString(TEXT("ExampleStringVariable get node")), PrintNode);

			if (PrintNode)
			{
				UEdGraphPin* const StringGetPin =  PrintNode->FindPin(StringVariableName);
				StringGetPin->MakeLinkTo(PrintInString);
			}
		}
	}
	else
	{
		return true;
	}

	UEdGraphPin* PrintStringThenPin = PrintStringNode->FindPin(UEdGraphSchema_K2::PN_Then);
	
	// Create constraint component
	Test->ContraintComponent = NewObject<UPhysicsConstraintComponent>(Test->ConstraintActorBlueprint);
	TestRunner.TestNotNull(TEXT("Added constraint component object "), Test->ContraintComponent);

	const FString ContraintNodeName = FString(TEXT("ExampleContraintComponent"));
	USCS_Node* ConstraintNode = QbaBPTestHelpers::ConstructBPComponent(Test->ConstraintActorBlueprint, Test->ContraintComponent, ContraintNodeName);
	TestRunner.TestNotNull(TEXT("Added constraint component variable"), ConstraintNode);

	if (!Test->ContraintComponent || !ConstraintNode || !PrintStringNode || !PrintStringThenPin)
	{
		return true;
	}

	//Create static meshes
	Test->MeshComponent1 = NewObject<UStaticMeshComponent>(Test->ConstraintActorBlueprint);
	TestRunner.TestNotNull(TEXT("Adding mesh component 1"), Test->MeshComponent1);
	const FString MeshComponent1NodeName = FString(TEXT("MeshComponent1"));
	USCS_Node* MeshComponent1Node = QbaBPTestHelpers::ConstructBPComponent(Test->ConstraintActorBlueprint, Test->MeshComponent1, MeshComponent1NodeName);
	TestRunner.TestNotNull(TEXT("Adding mesh component 1 node "), MeshComponent1Node);

	Test->MeshComponent2 = NewObject<UStaticMeshComponent>(Test->ConstraintActorBlueprint);
	const FString MeshComponent2NodeName = FString(TEXT("MeshComponent2"));
	USCS_Node* MeshComponent2Node = QbaBPTestHelpers::ConstructBPComponent(Test->ConstraintActorBlueprint, Test->MeshComponent2, MeshComponent2NodeName);
	TestRunner.TestNotNull(TEXT("Adding mesh component 2 node "), MeshComponent2Node);

	if (!Test->MeshComponent1 || !Test->MeshComponent2)
	{
		return true;
	}

	//TODO: assign valid meshes to them
	

	//Setup constraint 
	const float SetConstrainedXOffset{ 200.f };
	const float SetConstrainedYOffset{ -20.f };
	const FVector2D SetConstrainedNodeLocation = FVector2D(PrintStringNode->NodePosX + NodeXOffset, PrintStringNode->NodePosY + NodeYOffset);
	UEdGraphNode* SetConstrainedNode = QbaBPTestHelpers::AddNode<UPhysicsConstraintComponent>(Test->ConstraintActorBlueprint, Test->EventGraph, SetConstrainedNodeLocation, TEXT("SetConstrainedComponents"), PrintStringThenPin);
	TestRunner.TestNotNull(FString(TEXT("SetConstrainedNode")), SetConstrainedNode);

	
	const float ContraintGetXOffset{ 0.f };
	const float ConstraintGetYOffset{ -20.f };
	const FVector2D TargetConstraintNodeLocation = FVector2D(SetConstrainedNode->NodePosX + ContraintGetXOffset, SetConstrainedNode->NodePosY + ConstraintGetYOffset);
	UEdGraphNode* const ContraintGetNode = QbaBPTestHelpers::AddGetSetNode(Test->ConstraintActorBlueprint, Test->EventGraph, ContraintNodeName, true, TargetConstraintNodeLocation);
	TestRunner.TestNotNull(FString(TEXT("Contraint get node")), ContraintGetNode);

	const float Mesh1GetXOffset{ 0.f };
	const float Mesh1GetYOffset{ -60.f };
	const FVector2D Mesh1GetNodeLocation = FVector2D(SetConstrainedNode->NodePosX + Mesh1GetXOffset, SetConstrainedNode->NodePosY + Mesh1GetYOffset);
	UEdGraphNode* const Mesh1GetNode = QbaBPTestHelpers::AddGetSetNode(Test->ConstraintActorBlueprint, Test->EventGraph, MeshComponent1NodeName, true, Mesh1GetNodeLocation);
	TestRunner.TestNotNull(FString(TEXT("Mesh1GetNode")), Mesh1GetNode);

	const float Mesh2GetXOffset{ 0.f };
	const float Mesh2GetYOffset{ -100.f };
	const FVector2D Mesh2GetNodeLocation = FVector2D(SetConstrainedNode->NodePosX + Mesh2GetXOffset, SetConstrainedNode->NodePosY + Mesh2GetYOffset);
	UEdGraphNode* const Mesh2GetNode = QbaBPTestHelpers::AddGetSetNode(Test->ConstraintActorBlueprint, Test->EventGraph, MeshComponent2NodeName, true, Mesh2GetNodeLocation);
	TestRunner.TestNotNull(FString(TEXT("Mesh2GetNode")), Mesh2GetNode);


	if (!ContraintGetNode || !SetConstrainedNode)
	{
		return true;
	}

	UEdGraphPin* const GetConstraintPin = ContraintGetNode->FindPin(ContraintNodeName);
	UEdGraphPin* const GetMesh1Pin = Mesh1GetNode->FindPin(MeshComponent1NodeName);
	UEdGraphPin* const GetMesh2Pin = Mesh2GetNode->FindPin(MeshComponent2NodeName);
	UEdGraphPin* const InConstraintPin = SetConstrainedNode->FindPin(UEdGraphSchema_K2::PN_Self);
	UEdGraphPin* const InMesh1Pin = SetConstrainedNode->FindPin(TEXT("Component1"));
	UEdGraphPin* const InMesh2Pin = SetConstrainedNode->FindPin(TEXT("Component2"));

	const bool bHasAllValidPins = GetConstraintPin && GetMesh1Pin && GetMesh2Pin && InConstraintPin && InMesh1Pin && InMesh2Pin;
	TestRunner.TestTrue(FString(TEXT("bHasAllValidPins")), bHasAllValidPins);

	if (!bHasAllValidPins)
	{
		return true;
	}


	GetConstraintPin->MakeLinkTo(InConstraintPin);
	GetMesh1Pin->MakeLinkTo(InMesh1Pin);
	GetMesh2Pin->MakeLinkTo(InMesh2Pin);


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

