// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PlatformerTemplate : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ExternalLibsPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../_externalLibs/")); }
    }

    public PlatformerTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        LoadTrianglePP(Target);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}

    // Based on: https://wiki.unrealengine.com/Linking_Static_Libraries_Using_The_Build_System
    public bool LoadTrianglePP(ReadOnlyTargetRules Target)
    {
        string LibrariesPath = Path.Combine(ExternalLibsPath, "TrianglePP", "Libraries");

        /*
        test your path with:
        using System; // Console.WriteLine("");
        Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
        */

        // TODO: (Benjamin) Support for release version
        // PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "TrianglePP" + ConfigurationString + ".lib")); 
        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "TrianglePP.lib")); // 

        // Include path
        PublicIncludePaths.Add(Path.Combine(ExternalLibsPath, "TrianglePP", "Includes"));

        Definitions.Add(string.Format("WITH_TRIANGLEPP_BINDING={0}", 1)); // TODO: (Benjamin) Usefull?

        return true;
    }

}
