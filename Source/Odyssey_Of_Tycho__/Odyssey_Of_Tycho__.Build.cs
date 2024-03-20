// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Odyssey_Of_Tycho__ : ModuleRules
{
	public Odyssey_Of_Tycho__(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "NavigationSystem", "Navmesh" });
		
		PrivateDependencyModuleNames.AddRange(new string[] {"ProceduralMeshComponent"});
	}
}
