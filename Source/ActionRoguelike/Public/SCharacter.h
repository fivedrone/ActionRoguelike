// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "SCharacter.generated.h" 

class UCameraComponent;
class USpringArmComponent;
class USInteractComponent;
class USAttributeComponent;
class ASMagicProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	USInteractComponent* InteractionComp;

public:
	// Sets default values for this character's properties
	ASCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TObjectPtr<UCameraComponent> GetCameraComp() const
	{
		return CameraComp;
	}
	
protected: // for input protected
	
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASMagicProjectile> PrimaryAttackSlot;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASMagicProjectile> UltimateSlot;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASMagicProjectile> TeleportSlot;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category="1")
	FName FireLocation = "Muzzle_01";
	
	FTimerHandle TimerHandle_PrimaryAttack;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_BlackHole;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputAction* IA_Teleport;

public:

	// Handle move input
	void Move(const FInputActionValue& Value);
 
	// Handle look input
	void Look(const FInputActionValue& Value);

	void Fire(TSubclassOf<ASMagicProjectile> PJClass);

	// Handle Jump input
	void JumpAction();

	// Handle Interact input
	void PrimaryInteract();

	// Handle Attack input
	void PrimaryAttack(const FInputActionValue& Value);
	void PrimaryAttack_TimeElapsed();

	// Handle Ultimate input
	void BlackHole(const FInputActionValue& Value);
	void BlackHole_TimeElapsed();

	// Handle Teleport input
	void Teleport(const FInputActionValue& Value);
	void Teleport_TimeElapsed();
};
