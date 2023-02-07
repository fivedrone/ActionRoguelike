// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplodeBarrel.h"

#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplodeBarrel::ASExplodeBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderMesh"));
	CylinderMesh->SetSimulatePhysics(true);
	RootComponent = CylinderMesh;

	RadialForceMesh = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceMesh"));
	RadialForceMesh->SetupAttachment(CylinderMesh);
	RadialForceMesh->SetAutoActivate(false);

	ExplodeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplodeParticle"));
	ExplodeParticle->SetupAttachment(CylinderMesh);
	ExplodeParticle->SetAutoActivate(false);

	RadialForceMesh->Radius = 750.0f;
	RadialForceMesh->ImpulseStrength = 2500.0f;
	RadialForceMesh->bImpulseVelChange = true;

	RadialForceMesh->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
}

void ASExplodeBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CylinderMesh->OnComponentHit.AddDynamic(this, &ASExplodeBarrel::OnActorHit);
}

void ASExplodeBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceMesh->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location : %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

	ExplodeParticle->Activate();
	
	SetLifeSpan(0.4f);
}

