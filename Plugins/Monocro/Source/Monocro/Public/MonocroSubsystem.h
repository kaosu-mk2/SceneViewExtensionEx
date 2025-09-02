#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MonocroSettings.h"
#include "MonocroSubsystem.generated.h"

UCLASS()
class MONOCRO_API UMonocroSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static UMonocroSubsystem* Get(UWorld* World);

	const FMonocroSettings& GetMonocroSettings() const;

	void SetMonocroSettings(const FMonocroSettings& NewValue);

private:
	TSharedPtr<class FMonocroViewExtension, ESPMode::ThreadSafe> ViewExtension;

	FMonocroSettings MonocroSettings;
};
