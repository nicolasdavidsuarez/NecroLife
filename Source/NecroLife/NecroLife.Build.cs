// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NecroLife : ModuleRules
{
	public NecroLife(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"NecroLife",
			"NecroLife/Variant_Platforming",
			"NecroLife/Variant_Platforming/Animation",
			"NecroLife/Variant_Combat",
			"NecroLife/Variant_Combat/AI",
			"NecroLife/Variant_Combat/Animation",
			"NecroLife/Variant_Combat/Gameplay",
			"NecroLife/Variant_Combat/Interfaces",
			"NecroLife/Variant_Combat/UI",
			"NecroLife/Variant_SideScrolling",
			"NecroLife/Variant_SideScrolling/AI",
			"NecroLife/Variant_SideScrolling/Gameplay",
			"NecroLife/Variant_SideScrolling/Interfaces",
			"NecroLife/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
