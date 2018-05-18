// Some copyright should be here...

using UnrealBuildTool;
using System.IO;

public class HaoshaSDK : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Thirdparty/")); }
    }
	public HaoshaSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicIncludePaths.AddRange(
			new string[] {
				"HaoshaSDK/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"HaoshaSDK/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
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

		if (Target.Platform == UnrealTargetPlatform.Android)
        {
            
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "HaoshaSDK_APL.xml")));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicLibraryPaths.Add("/usr/lib");
            
            PublicAdditionalLibraries.Add("/usr/lib/libc++.dylib");
            PublicAdditionalLibraries.Add("/usr/lib/libz.dylib");
            PublicAdditionalLibraries.Add("/usr/lib/libsqlite3.dylib");
            PublicAdditionalLibraries.Add("/usr/lib/libstdc++.dylib");
            PublicAdditionalLibraries.Add("/usr/lib/libicucore.dylib");

            PublicIncludePaths.AddRange(
            new string[] {
                //ModuleDirectory+"/../../Thirdparty/iOS/lib/AlipaySDK.framework/Headers/",
                ModuleDirectory+"/../../Thirdparty/iOS/include/",
                ModuleDirectory+"/../../Thirdparty/ShareSDK/include/",
            });
            
            PublicAdditionalLibraries.Add(ModuleDirectory+"/../../Thirdparty/iOS/lib/HMJSBridge.a");
            PublicAdditionalLibraries.Add(ModuleDirectory+"/../../Thirdparty/iOS/lib/HMPublicTools.a");
           // PublicAdditionalLibraries.Add(ModuleDirectory+"/../../Thirdparty/ShareSDK/lib/libWeiboSDK.a");
            PublicAdditionalLibraries.Add(ModuleDirectory+"/../../Thirdparty/ShareSDK/lib/libWeChatSDK.a");
           // PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath,"ShareSDK/lib/libWeiboSDK.a"));
            //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath,"ShareSDK/lib/libWeChatSDK.a"));

            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "AlipaySDK",                                                        // Framework name
            "../../Thirdparty/iOS/lib/AlipaySDK.embeddedframework.zip",
            "AlipaySDK.bundle"                                // Resources we need copied and staged
            )
            );

            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "ShareSDK",
            "../../Thirdparty/ShareSDK/lib/ShareSDK.embeddedframework.zip",
            "ShareSDK.bundle"
            )
            );
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "ShareSDKConnector",
            "../../Thirdparty/ShareSDK/lib/ShareSDKConnector.embeddedframework.zip"
            )
            );
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "ShareSDKExtension",
            "../../Thirdparty/ShareSDK/lib/ShareSDKExtension.embeddedframework.zip"
            )
            );
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "ShareSDKUI",
            "../../Thirdparty/ShareSDK/lib/ShareSDKUI.embeddedframework.zip",
            "ShareSDKUI.bundle"
            )
            );
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "MOBFoundation",
            "../../Thirdparty/ShareSDK/lib/MOBFoundation.embeddedframework.zip"
            )
            );
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "WechatConnector",
            "../../Thirdparty/ShareSDK/lib/WechatConnector.embeddedframework.zip"
            )
            );
            
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "GTSDK",
            "../../Thirdparty/iOS/lib/GTSDK.embeddedframework.zip"
            )
            );

            PublicFrameworks.AddRange(
            new string[]
            {
                "SystemConfiguration",
                "CoreMotion",
                "CoreGraphics",
                "Foundation",
                "UIKit",
                "QuartzCore",
                "CFNetwork",
                "CoreText",
                "CoreTelephony",
                "JavaScriptCore",
                "ImageIO",
                "Security",
                "MobileCoreServices",
                "CoreLocation",
                "AdSupport",
                "UserNotifications"
            }
            );
        }
	}
}
