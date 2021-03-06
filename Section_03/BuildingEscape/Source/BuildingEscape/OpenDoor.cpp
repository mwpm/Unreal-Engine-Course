// Copyright Michael Molina 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	// Debug info
	//FQuat rotation = Owner->GetTransform().GetRotation();
	//FString ObjectName = Owner->GetName();
	//FString ObjectPosition = rotation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s has rotation %s"), *ObjectName, *ObjectPosition);

	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	// Debug info
	//FQuat rotation = Owner->GetTransform().GetRotation();
	//FString ObjectName = Owner->GetName();
	//FString ObjectPosition = rotation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s has rotation %s"), *ObjectName, *ObjectPosition);

	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
		// Then OpenDoor

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

