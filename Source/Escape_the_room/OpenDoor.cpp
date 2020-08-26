// Fill out your copyright notice in the Description page of Project Settings.

#define OUT
#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UOpenDoor::BeginPlay()
{
    Super::BeginPlay();
    Owner = GetOwner();
    
    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("Pressure plate is missing"));
    }
}

void UOpenDoor::OpenDoor()
{
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
    Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass= 0.f;
    
    // Find overlapping actors
    TArray<AActor*> OverlappingActors;
    if (!PressurePlate) { return TotalMass; }
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    // Iterate through them adding their masss
    for (const auto* Actor : OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s is in the spot!"), *Actor->GetName());
    }
    
    return TotalMass;
    
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
    if (GetTotalMassOfActorsOnPlate() > 30.f)
    {
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
    {
        CloseDoor();
    }
    GetTotalMassOfActorsOnPlate();
}


