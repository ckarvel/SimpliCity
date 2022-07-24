using UnrealBuildTool; 

public class Road: ModuleRules
{
    public Road(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"}); 
    }
}