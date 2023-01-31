
#include "QbaGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"


// Q: Why do I construct it like this? What is the purpouse of constructing struct like this in cpp file?
FQbaInputTags FQbaInputTags::InputTags;

void FQbaInputTags::AddInputTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	InputTag_Move = Manager.AddNativeGameplayTag(TEXT("InputTag.Move"));
	InputTag_Look_Mouse = Manager.AddNativeGameplayTag(TEXT("InputTag.Look.Mouse"));
	InputTag_Look_Gamepad = Manager.AddNativeGameplayTag(TEXT("InputTag.Look.Gamepad"));
	InputTag_Jump = Manager.AddNativeGameplayTag(TEXT("InputTag.Jump"));
	InputTag_Fire = Manager.AddNativeGameplayTag(TEXT("InputTag.Fire"));
	InputTag_Aim = Manager.AddNativeGameplayTag(TEXT("InputTag.Aim"));
	InputTag_AbilityWheel = Manager.AddNativeGameplayTag(TEXT("InputTag.AbilityWheel"));
}

FQbaAbilityTags FQbaAbilityTags::AbilityTags;
void FQbaAbilityTags::AddAbilityTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	AbilityTag_Movement = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Movement.Walk"));
	AbilityTag_CameraMouse = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Movement.Camera.Mouse"));
	AbilityTag_CameraGamepad = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Movement.Camera.Gamepad"));
	AbilityTag_Jump = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Movement.Jump"));
	AbilityTag_Sprint = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Movement.Sprint"));
	AbilityTag_Dash = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Movement.Dash"));
	AbilityTag_Telekinesis = Manager.AddNativeGameplayTag(TEXT("AbilityTag.Skills.Telelkinesis"));
	//NOTE: Tags names are not final.
}
