// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "SCharacter.generated.h" 

class UCameraComponent;
class USpringArmComponent;
class USInteractComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;
	
	FTimerHandle TimerHandle_PrimaryAttack;

	
public:
	// Sets default values for this character's properties
	ASCharacter();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected: // for input protected
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_PrimaryInteract; 
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractComponent* InteractionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle move input
	void Move(const FInputActionValue& Value);
 
	// Handle look input
	void Look(const FInputActionValue& Value);

	// Handle Attach input
	void PrimaryAttack(const FInputActionValue& Value);

	void JumpAction();

	void PrimaryInteract();

	void PrimaryAttack_TimeElapsed();
};
