// Copyright Jakub Urbanek. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class QbaProgrammingPathEditorTarget : TargetRules
{
	public QbaProgrammingPathEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("QbaProgrammingPath");
	}
}
