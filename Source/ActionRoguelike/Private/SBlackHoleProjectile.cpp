// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASBlackHoleProjectile::ASBlackHoleProjectile()

{
	ProjectileSpeed = 1000.0f;
	LifeTime = 4.8f;
	Damage = 0.0f;
	ProjectileType = "BHProjectile";
	SphereComp->SetCollisionProfileName(ProjectileType);
	
	BlackHoleForce = CreateDefaultSubobject<URadialForceComponent>("BlackHoleForce");
	BlackHoleForce->SetupAttachment(SphereComp);
	BlackHoleForce->Radius = 1500.0f;
	BlackHoleForce->bImpulseVelChange = true;
	BlackHoleForce->bIgnoreOwningActor = true;
	BlackHoleForce->ForceStrength = -1500000.0f;
	AttackRange = LifeTime * ProjectileSpeed;
}


void ASBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASBlackHoleProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	OtherActor->Destroy();
}

void ASBlackHoleProjectile::Destroyed()
{
	if(IsValid(Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("BH_Destroyed"));
		Super::Destroyed();

		BlackHoleForce->FireImpulse();
	}
}
