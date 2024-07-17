// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SmartClock : ModuleRules
{
	public SmartClock(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.AddRange(new string[] {
                "Bthprops.lib",
                "Ws2_32.lib"
            });

            PublicIncludePaths.AddRange(new string[] {
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um"
            });
        }

        // Ensure that the Java code is included in the Android build
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch", "AndroidPermission" }); // Required for Android
                                                                                                   //PublicDependencyModuleNames.AddRange(new string[] { "ApplicationCore", "AndroidRuntimeSettings" });
            //PrivateIncludePaths.Add("SmartClock/Build/Android/src/main/java");
        }

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
