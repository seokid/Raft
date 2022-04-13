// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RaftDedicated : ModuleRules
{
	public RaftDedicated(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG"
																	, "SlateCore"
                                                                    , "CableComponent"
                                                                    , "Water"
                                                                    , "AIModule", "GameplayTasks", "NavigationSystem"     //AIModule, GameplayTasks, NavigationSystem AI
                                                                    , "ApexDestruction"										//Desturctible Component
																	});
		
	}
}