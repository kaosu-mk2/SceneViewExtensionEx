#pragma once

#include "CoreMinimal.h"
#include "MonocroSettings.h"
#include "GameFramework/Actor.h"
#include "MonocroControlActor.generated.h"

UCLASS()
class MONOCRO_API AMonocroControlActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonocroControlActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void ApplySettings();

	UPROPERTY(EditAnywhere, Category = "Monocro Settings")
	FMonocroSettings MonocroSettings;
};
