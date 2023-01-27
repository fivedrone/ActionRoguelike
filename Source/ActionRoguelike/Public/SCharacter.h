// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "SCharacter.generated.h" 

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_Look;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_Jump;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle move input
	void Move(const FInputActionValue& Value);
 
	// Handle look input
	void Look(const FInputActionValue& Value);

	// Handle Attach input
	void PrimaryAttack(const FInputActionValue& Value);

	void JumpAction();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
