using System.IO;
using UnrealBuildTool;
namespace UnrealBuildTool.Rules
{
    public class OSSModule : ModuleRules
    {
        public OSSModule(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivateIncludePaths.AddRange(
            new string[] {
                "OSSModule/Private",
                "OSSModule/Private/SDK"
            }
            );

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "GCore"
                //"libcurl"
            }
            );

            LoadThirdParty(Target);
        }

        public void LoadThirdParty(ReadOnlyTargetRules Target)
        {
            string prefixPath = Path.Combine(ModuleDirectory, "..", "..", "Thirdparty", "OSS");
            if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                PrivateIncludePaths.AddRange(new string[] {

                    Path.Combine(prefixPath,"include","Mac"),
                    Path.Combine(prefixPath,"include","Mac","apr-1"),
                    Path.Combine(prefixPath,"include","Mac","aprutil-1"),
                    Path.Combine(prefixPath,"include","Mac","curl")
                });

                string LibPath = Path.Combine(prefixPath, "lib", "Mac");
                PublicLibraryPaths.Add(LibPath);

                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libAliyunOSS.a"));
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libcurl.a"));
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libmxml.a"));
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libapr-1.a"));
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libiconv.2.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libaprutil-1.a"));
            }
            else if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
            {
            	PublicDependencyModuleNames.AddRange(
            	new string[]{
                	"libcurl"
            	});
                PrivateIncludePaths.AddRange(new string[] {
                    Path.Combine(ModuleDirectory,"Private","SDK"),
                    Path.Combine(prefixPath, "include","Win","apr"),
                    Path.Combine(prefixPath, "include","Win","apr-util"),
                    Path.Combine(prefixPath, "include","Win","mxml"),

                    Path.Combine(prefixPath, "include","Win","iconv"),
                    Path.Combine(prefixPath, "include","Win","expat"),
                });

                string libPath = Path.Combine(prefixPath, "lib", "Win");
                PublicLibraryPaths.Add(libPath);
                PublicAdditionalLibraries.Add(Path.Combine(libPath, "libexpat.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(libPath, "libapr-1.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(libPath, "libapriconv-1.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(libPath, "libaprutil-1.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(libPath, "mxml1.lib"));
            }
            else if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PrivateDependencyModuleNames.Add("Launch");
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "OSSMoudle_APL.xml")));
            }
            else if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicLibraryPaths.Add("/usr/lib");
                
                PublicAdditionalLibraries.Add("/usr/lib/libresolv.dylib");
                PublicAdditionalLibraries.Add("/usr/lib/libz.1.dylib");
                PublicAdditionalLibraries.Add("/usr/lib/libxml2.2.dylib");
                
                PublicIncludePaths.AddRange(
                new string[] {
                    ModuleDirectory+"/Frameworks/AliyunOSSiOS.framework/Headers/",
                });
                
                PublicAdditionalFrameworks.Add(
                new UEBuildFramework(
                "AliyunOSSiOS",                                                        // Framework name
                "Frameworks/AliyunOSSiOS.embeddedframework.zip"
                //"Resources/TapjoyResources.bundle"                                // Resources we need copied and staged
                )
                );
                

                PublicFrameworks.AddRange(
                new string[]
                {
                    "SystemConfiguration",
                    "CoreTelephony",
                    "CFNetwork",
                    "CoreFoundation",
                    "MobileCoreServices",
                }
                );
            }
        }
    }
}
