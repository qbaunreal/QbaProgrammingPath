// Copyright Jakub Urbanek. All Rights Reserved.

using UnrealBuildTool;

public class QbaAutomationEditor : ModuleRules
{
	public QbaAutomationEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "QbaAutomationPlugin",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "QbaAutomationPlugin",
                "EditorSubsystem",
				"UnrealEd",
                "BlueprintGraph",
				"Kismet"
            }
			);
    }
}
