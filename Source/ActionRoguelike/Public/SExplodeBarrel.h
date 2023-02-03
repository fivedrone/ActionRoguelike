// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplodeBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplodeBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASExplodeBarrel();

protected:
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;

	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForceMesh;

	virtual void PostInitializeComponents() override;

	
	
	UFUNCTION(BlueprintCallable)
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
