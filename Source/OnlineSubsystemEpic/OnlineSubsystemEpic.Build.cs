// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemEpic : ModuleRules
{
	public OnlineSubsystemEpic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"OnlineSubsystemUtils",
				"EpicSdk"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
				"NetCore",
				"OnlineSubsystem"
			}
		);
	}
}
