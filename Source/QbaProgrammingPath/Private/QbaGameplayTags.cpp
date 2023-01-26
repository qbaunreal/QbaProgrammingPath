
#include "QbaGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"


// Q: Why do I construct it like this? What is the purpouse of constructing struct like this in cpp file?
FQbaGameplayTags FQbaGameplayTags::GameplayTags;

void FQbaGameplayTags::AddTags()
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
