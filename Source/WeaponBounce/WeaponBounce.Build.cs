// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WeaponBounce : ModuleRules
{
	public WeaponBounce(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Json", "JsonUtilities", "Niagara" });
	}
}
