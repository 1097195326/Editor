// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Editor : ModuleRules
{
	public Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "XmlParser", "GCore", "GMsgCenter", "GHttp", "GXmlPlugins","OSSModule","ImageMagick","HaoshaSDK", "EncryptAndDecrypt", "WebBrowserWidget" });
        PrivateDependencyModuleNames.AddRange(new string[] { "ImageWrapper", "AssetRegistry", "HTTP", "UMG", "JsonUtilities", "Json", "PakFile", "RHI", "WebBrowserWidget" });
        PublicDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent", "ClothingSystemRuntime", "RenderCore", "HTTP"});
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("Launch");
        }
        PrivateIncludePaths.AddRange(
            new string[] {
                "Private/User/Const",
                "Private/User/Datas",
                "Private/User/Director",
                "Private/User/Modules"
				// ... add other private include paths required here ...
			}
            );

        PublicIncludePaths.AddRange(
            new string[] {
                "Public",
                "Public/User/Const",
                "Public/User/Datas",
                "Public/User/State/State",
                "Public/User/State/StateMachine",
                "Public/User/UIController",
                "Public/User/Datas/Runtime",
                "Public/User/Datas/Runtime/ComponentStruct",
                "Public/User/Datas/Template",
                "Public/User/Datas/Controller",
                "Public/User/DataManager",
                "Public/User/DataManager/Template",
                "Public/User/DataManager/Runtime",
                "Public/User/Director",
                "Public/User/Modules",
                "Public/User/Tools",
                "Public/User/UI",
                "Public/User/PCUI",
                "Public/User/Scheduler",
                "Public/User/Render",
                "Public/User/Render/Components",
                "Public/User/Render/Actors",
                "Public/User/Render/Render",
                "Public/User/Settings"
                // ... add public include paths required here ...
            }
            );

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
