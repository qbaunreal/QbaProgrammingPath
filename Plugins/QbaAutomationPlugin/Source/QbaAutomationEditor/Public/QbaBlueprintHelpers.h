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
		/*UEdGraph* TempOuter = NewObject<UEdGraph>(static_cast<UObject*>(InBlueprint));*/
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
}

#endif // WITH_EDITOR