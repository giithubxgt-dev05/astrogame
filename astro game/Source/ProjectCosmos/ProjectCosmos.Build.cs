using UnrealBuildTool;

public class ProjectCosmos : ModuleRules
{
	public ProjectCosmos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput", 
            "UMG", 
            "Slate", 
            "SlateCore", 
            "GameplayAbilities", 
            "GameplayTasks", 
            "GameplayTags", 
            "AIModule", 
            "NavigationSystem", 
            "PhysicsCore", 
            "ChaosVehicles", 
            "NetCore", 
            "OnlineSubsystem", 
            "OnlineSubsystemUtils", 
            "Json", 
            "JsonUtilities" 
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
