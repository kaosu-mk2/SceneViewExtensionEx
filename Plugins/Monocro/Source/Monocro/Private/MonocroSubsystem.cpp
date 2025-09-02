#include "MonocroSubsystem.h"
#include "MonocroViewExtension.h"

void UMonocroSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ViewExtension = FSceneViewExtensions::NewExtension<FMonocroViewExtension>();
}

void UMonocroSubsystem::Deinitialize()
{
	Super::Deinitialize();

	ViewExtension.Reset();
}

UMonocroSubsystem* UMonocroSubsystem::Get(UWorld* World)
{
	if (World)
	{
		return World->GetSubsystem<UMonocroSubsystem>();
	}

	return nullptr;
}

const FMonocroSettings& UMonocroSubsystem::GetMonocroSettings() const
{
	return MonocroSettings;
}

void UMonocroSubsystem::SetMonocroSettings(const FMonocroSettings& NewValue)
{
	FMonocroSettings& Value = MonocroSettings;

	Value.Enabled = NewValue.Enabled;
	Value.Weight = NewValue.Weight;
}
