using System.IO;
using UnrealBuildTool;

public class EpicSdk : ModuleRules
{
    public EpicSdk( ReadOnlyTargetRules Target ) : base( Target )
    {
        Type = ModuleType.External;		
		
        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "EOSSDK-Win32-Shipping.lib"));
            
            PublicDelayLoadDLLs.Add("EOSSDK-Win32-Shipping.dll");
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Bin", "EOSSDK-Win32-Shipping.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "EOSSDK-Win64-Shipping.lib"));
            
            PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Bin", "EOSSDK-Win64-Shipping.dll"));
        }
        else
        {
            System.Console.WriteLine("Epic Online Services SDK not supported for this platform");
        }
		
        PublicIncludePaths.Add(Path.Combine( ModuleDirectory, "Include" ));
    }
}