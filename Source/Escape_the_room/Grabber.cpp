// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
    if (!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("The %s is missing the physics handle component"), *GetOwner()->GetName());
    }
}

void UGrabber::SetUpInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
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
    // LINE TRACE and see if we reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    // if we hit something than attach a physics handle
    if (ActorHit) {
        PhysicsHandle->GrabComponentAtLocationWithRotation (
                           ComponentToGrab,
                           NAME_None,
                           ComponentToGrab->GetOwner()->GetActorLocation(),
                           FRotator(0.f, 0.f, 0.f)
                           );
    }
}

void UGrabber::Drop()
{
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    FVector LineTraceEnd = GetReachLineEnd();

    //  Setup query parameters
    
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

    // Line-trace (aka ray-cast) out to reach distance

    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                    OUT Hit,
                    GetReachLineStart(),
                    LineTraceEnd,
                    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                    TraceParameters
                    );
        
    return Hit;
}

FVector UGrabber::GetReachLineStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
         OUT PlayerViewPointLocation,
         OUT PlayerViewPointRotation
    );
        
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
         OUT PlayerViewPointLocation,
         OUT PlayerViewPointRotation
    );
        
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    

    
    //  if the physics handle is attached
           // move the object that we're holding
    
    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }

    
}

