// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
    UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    FindPhysicsHandleComponent();
    SetUpInputComponent();
    

}

void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (PhysicsHandle)
    {
        // PhysicsHandle is found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The %s is missing the handle"), *GetOwner()->GetName());
    }
}

void UGrabber::SetUpInputComponent()
{
    if (InputComponent)
    {
        // Input Component Found
        UE_LOG(LogTemp, Warning, TEXT("ALL IS GOOD"));
        // Bind input action
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The %s is missing the input component"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("GRAB GRAB GRAB!!!"));
    
    // LINE TRACE and see if we reach any actors with physics body collision channel set
    GetFirstPhysicsBodyInReach();
    // if we hit something than attach a physics handle
}

void UGrabber::Drop()
{
    UE_LOG(LogTemp, Warning, TEXT("DROPEDDY DROP!!!"));
    
    // release physics handle
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Get player viewpoint this tick
        FVector PlayerViewPointLocation;
        FRotator PlayerViewPointRotation;
        GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
             OUT PlayerViewPointLocation,
             OUT PlayerViewPointRotation
    );
        
        FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

        //  Setup query parameters
        
        FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
        // Line-trace (aka ray-cast) out to reach distance
        
        FHitResult Hit;
        GetWorld()->LineTraceSingleByObjectType(
                        OUT Hit,
                        PlayerViewPointLocation,
                        LineTraceEnd,
                        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                        TraceParameters
                        );
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("you hit %s!"), *HitActor->GetName());
        }
        
        
        // See what we hit
    
    return Hit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    //  if the physics handle is attached
        // move the object that we're holding

    
}

