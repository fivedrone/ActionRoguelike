// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystem* ExplodeSlot;

	UPROPERTY(EditAnywhere, Category="Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category="Projectile")
	float LifeTime;

	UPROPERTY(EditAnywhere, Category="Projectile")
	float Damage;

	// attackrange = lifeTime * initial speed
	UPROPERTY(VisibleDefaultsOnly, Category="Projectile")
	float AttackRange;
	
	AActor* Owner;

	FName ProjectileType = "MProjectile";
	
private:
	FVector2D ScreenPosition;
	FVector WorldPosition;
	FVector WorldDirection;

	bool IsTraceReady;

	
protected:
	
	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);
	// Sets default values for this actor's properties
	ASMagicProjectile();
	
public:

	virtual void BeginPlay() override;
	
	bool InitTrace();

	void SceneTrace();

	void ObjectTrace();

	virtual void Destroyed() override;
};
