#pragma once

#include "CoreMinimal.h"
#include "MonocroSettings.generated.h"

USTRUCT(BlueprintType)
struct FMonocroSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
	float Weight;

	MONOCRO_API FMonocroSettings();
};
