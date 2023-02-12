// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
	ProjectileSpeed = 5000.0f;
	LifeTime = 0.2f;
	Damage = 0.0f;
	ProjectileType = "TPProjectile";
	SphereComp->SetCollisionProfileName(ProjectileType);
	
	MovementComp->InitialSpeed = ProjectileSpeed;
	AttackRange = LifeTime*ProjectileSpeed;
	InitialLifeSpan = LifeTime;
}

void ASTeleportProjectile::Destroyed()
{
	Super::Destroyed();

	UE_LOG(LogTemp, Warning, TEXT("TP_Destroyed"));
	if(IsValid(GetInstigator()))
	{
		GetInstigator()->GetMovementComponent()->StopMovementImmediately();
	
		GetInstigator()->TeleportTo(GetActorLocation(), Owner->GetActorRotation());
	}
}
