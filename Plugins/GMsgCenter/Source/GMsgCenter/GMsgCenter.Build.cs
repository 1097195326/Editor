// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GMsgCenter : ModuleRules
{
	public GMsgCenter(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "GMsgCenter/Public/System/",
                "GMsgCenter/Public/System/MsgCenter/Channels",
                "GMsgCenter/Public/System/MsgCenter/MsgType",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"GMsgCenter/Private",
                "GMsgCenter/Private/System/",
                "GMsgCenter/Private/MsgCenter/Channels",
                "GMsgCenter/Private/MsgCenter/MsgType",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core", "CoreUObject", "Engine", "InputCore","GCore","JsonUtilities","Json"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
