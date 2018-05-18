// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GCore : ModuleRules
{
    public GCore(ReadOnlyTargetRules Target) : base(Target)
    {
        //PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.AddRange(
            new string[] {
                "GCore/Public",
                "GCore/Public/System/Manager",
                "GCore/Public/System/BaseData",
                "GCore/Public/System/Interface",
                "GCore/Public/System/Tools",
                "GCore/Public/System/Render",
                "GCore/Public/System/Scene",
                "GCore/Public/System/Scheduler",
                "GCore/Public/System/States",
                "GCore/Public/System/UI",
                "GCore/Public/System/Const",
                "GCore/Public/System/GameMode",
                "GCore/Public/AAReflection",
                "GCore/Public/System/Prototypes",
                "GCore/Public/System/Settings",
                "GCore/Public/System/FileManager"
                // ... add public include paths required here ...
            }
            );
                
        
        PrivateIncludePaths.AddRange(
            new string[] {
                "GCore/Private",
                "GCore/Private/System/Manager",
                "GCore/Private/System/BaseData",
                "GCore/Private/System/Interface",
                "GCore/Private/System/Tools",
                "GCore/Private/System/Render",
                "GCore/Private/System/Scene",
                "GCore/Private/System/Scheduler",
                "GCore/Private/System/Prototypes",
                "GCore/Private/System/UI",
                "GCore/Private/System/Const",
                "GCore/Private/AAReflection",
                "GCore/Private/System/Prototypes",
                "GCore/Private/System/Settings",
                "GCore/Private/System/GameMode",
                "GCore/Private/System/FileManager"
                // ... add other private include paths required here ...
            }
            );
            
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",  "CoreUObject", "Engine","Slate", "SlateCore","UMG", "InputCore","JsonUtilities","Json","GXmlPlugins","HTTP","Networking", "EncryptAndDecrypt"
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

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "ShaderCore",
                "RenderCore",
                "RHI",
            }
        );
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("Launch");
        }
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "ImageWrapper",
                 "TargetPlatform",
            }
        );

        if (Target.Type != TargetType.Server)
        {
            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "SlateRHIRenderer",
                }
            );

            DynamicallyLoadedModuleNames.AddRange(
                new string[] {
                    "ImageWrapper",
                }
            );
        };
    }
}
