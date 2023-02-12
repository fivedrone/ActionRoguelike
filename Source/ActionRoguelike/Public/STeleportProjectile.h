// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectile.h"
#include "STeleportProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASMagicProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTeleportProjectile();

	virtual void Destroyed() override;


};
