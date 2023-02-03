// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"

#include "SGameplayInterface.h"
#include "SCharacter.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
USInteractComponent::USInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner =  GetOwner();
	
	FVector CameraLocation;
	FRotator CameraRotation;

	ASCharacter* MyCharacter = Cast<ASCharacter>(MyOwner);
	
	CameraLocation = MyCharacter->GetCameraComp()->GetComponentLocation();
	CameraRotation = MyCharacter->GetCameraComp()->GetComponentRotation();

	FVector End = CameraLocation + (CameraRotation.Vector() * 1000);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = 30.f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, CameraLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor Linecolor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
			
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);

				break;
			}
		}

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, Linecolor, false, 2.0f);
	}

	
	DrawDebugLine(GetWorld(),CameraLocation, End, Linecolor, false, 2.0f, 0, 2.0f);

	
}
