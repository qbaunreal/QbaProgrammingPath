// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "EdGraphSchema_K2_Actions.h"
#include "BlueprintEditor.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EditorLevelUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealEdGlobals.h"
#include "LevelEditor.h"
#include "Editor/UnrealEdEngine.h"
#include "UObject/UObjectGlobals.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Editor.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_VariableGet.h"
#include "K2Node_VariableSet.h"
#include "ComponentAssetBroker.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"


namespace QbaBPTestHelpers
{
	static TObjectPtr<UEdGraphNode> const FindGraphNode(const UEdGraph* Graph, const FString& NodeName)
	{
		if (!Graph || Graph->Nodes.Num() == 0)
		{
			return nullptr;
		}

		for (const TObjectPtr<UEdGraphNode>& Node : Graph->Nodes)
		{
			if (Node.Get()->GetFindReferenceSearchString() == NodeName)
			{
				return Node;
			}
		}
		return nullptr;
	}
	static UK2Node* CreateKismetTemplateFunction(UObject* NodeOuter, const FName& FunctionName)
	{
		// Make a call function template
		UK2Node_CallFunction* CallFuncNode = NewObject<UK2Node_CallFunction>(NodeOuter);

		UFunction* Function = FindFieldChecked<UFunction>(UKismetSystemLibrary::StaticClass(), FunctionName);
		CallFuncNode->FunctionReference.SetFromField<UFunction>(Function, false);
		return CallFuncNode;
	}

	static UEdGraphNode* CreateNewNodeFromTemplate(UK2Node* NodeTemplate, UEdGraph* InGraph, const FVector2D& GraphLocation, UEdGraphPin* ConnectPin = NULL)
	{
		TSharedPtr<FEdGraphSchemaAction_K2NewNode> Action = TSharedPtr<FEdGraphSchemaAction_K2NewNode>(new FEdGraphSchemaAction_K2NewNode(FText::GetEmpty(), FText::GetEmpty(), FText::GetEmpty(), 0));
		Action->NodeTemplate = NodeTemplate;

		return Action->PerformAction(InGraph, ConnectPin, GraphLocation, false);
	}

	static UEdGraphNode* AddPrintStringNode(UBlueprint* InBlueprint, UEdGraph* InGraph, const FVector2D& InGraphLocation, UEdGraphPin* ConnectPin = NULL)
	{
		InGraph->Modify();
		UEdGraph* TempOuter = NewObject<UEdGraph>(InBlueprint);
		TempOuter->SetFlags(RF_Transient);

		// Make a call function template
		const FName PrintStringFunctionName(TEXT("PrintString"));
		UK2Node* CallFuncNode = CreateKismetTemplateFunction(TempOuter, PrintStringFunctionName);

		return CreateNewNodeFromTemplate(CallFuncNode, InGraph, InGraphLocation, ConnectPin);
	}


	static void StartPIE(bool bSimulateInEditor)
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::Get().GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		TSharedPtr<class IAssetViewport> ActiveLevelViewport = LevelEditorModule.GetFirstActiveViewport();

		FRequestPlaySessionParams SessionParams;
		SessionParams.DestinationSlateViewport = ActiveLevelViewport;
		if (bSimulateInEditor)
		{
			SessionParams.WorldType = EPlaySessionWorldType::SimulateInEditor;
		}

		GUnrealEd->RequestPlaySession(SessionParams);
	}

	static bool AddVariable(const FString& Name, const FString& Value, UBlueprint* InBlueprint, UEdGraph* InGraph, const FName Type = UEdGraphSchema_K2::PC_String)
	{
		const FName VariableName = FName(*Name);
		const FEdGraphPinType PinType(Type, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
		return FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VariableName, PinType, Value);
	}

	static bool AddArray(const FString& Name, const FString& Value, UBlueprint* InBlueprint, UEdGraph* InGraph, const FName Type = UEdGraphSchema_K2::PC_String)
	{
		const FName VariableName = FName(*Name);
		const FEdGraphPinType PinType(Type, NAME_None, nullptr, EPinContainerType::Array, false, FEdGraphTerminalType());
		return FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VariableName, PinType, Value);
	}

	static UEdGraphNode* AddGetSetNode(UBlueprint* InBlueprint, UEdGraph* InGraph, const FString& VarName, bool bGet, const FVector2D& Location)
	{
		InGraph->Modify();

		FEdGraphSchemaAction_K2NewNode NodeInfo;
		UK2Node_Variable* TemplateNode = NULL;
		if (bGet)
		{
			TemplateNode = NewObject<UK2Node_VariableGet>(InBlueprint);
		}
		else
		{
			TemplateNode = NewObject<UK2Node_VariableSet>(InBlueprint);
		}

		TemplateNode->VariableReference.SetSelfMember(FName(*VarName));
		NodeInfo.NodeTemplate = TemplateNode;

		return NodeInfo.PerformAction(InGraph, NULL, Location, true);
	}
	

	static void SetPinValue(UEdGraphNode* Node, const FName& PinName, const FString& PinValue)
	{
		UEdGraphPin* Pin = Node->FindPin(PinName);
		Pin->DefaultValue = PinValue;
	}


	static USCS_Node* ConstructBPComponent(UBlueprint* InBlueprint, UObject* InAsset, const FString& NodeName)
	{
		IAssetEditorInstance* OpenedEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(InBlueprint, true);
		FBlueprintEditor* BlueprintEditor = static_cast<FBlueprintEditor*>(OpenedEditor);

		USCS_Node* NewNode = InBlueprint->SimpleConstructionScript->CreateNode(InAsset->GetClass(), FName(NodeName));
		FComponentAssetBrokerage::AssignAssetToComponent(NewNode->ComponentTemplate, InAsset);

		TArray<USCS_Node*> AllNodes = InBlueprint->SimpleConstructionScript->GetAllNodes();
		USCS_Node* RootNode = AllNodes.Num() > 0 ? AllNodes[0] : NULL;

		if (!RootNode || (RootNode == InBlueprint->SimpleConstructionScript->GetDefaultSceneRootNode()))
		{
			InBlueprint->SimpleConstructionScript->AddNode(NewNode);
		}
		else
		{
			RootNode->AddChildNode(NewNode);
		}

		FKismetEditorUtilities::GenerateBlueprintSkeleton(InBlueprint, true);
		BlueprintEditor->UpdateSubobjectPreview(true);

		return NewNode;
	}

}

#endif // WITH_EDITOR