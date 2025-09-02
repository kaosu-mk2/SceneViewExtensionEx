#include "MonocroControlActor.h"
#include "MonocroSubsystem.h"

// Sets default values
AMonocroControlActor::AMonocroControlActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonocroControlActor::BeginPlay()
{
	Super::BeginPlay();
	ApplySettings();
}

void AMonocroControlActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplySettings();
}

// Called every frame
void AMonocroControlActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonocroControlActor::ApplySettings()
{
	if (UMonocroSubsystem* Subsystem = UMonocroSubsystem::Get(GetWorld()))
	{
		Subsystem->SetMonocroSettings(MonocroSettings);
	}
}

