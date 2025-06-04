// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class The_Shooter : ModuleRules
{
	public The_Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
