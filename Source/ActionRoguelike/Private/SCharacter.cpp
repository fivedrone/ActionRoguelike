 // Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SGameplayInterface.h"
 #include "SInteractComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

 // Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent.Get());

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp.Get());

	InteractionComp = CreateDefaultSubobject<USInteractComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		const FVector2d MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Forward/Backward direction
		if(MoveValue.X != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, MoveValue.X);
			
		}

		// Right/Lef direction
		if(MoveValue.Y != 0.f)
		{
			//Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.Y);
		}
	}
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();

		if(LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if(LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
		
	}
}

 void ASCharacter::PrimaryAttack(const FInputActionValue& Value)
 {
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
	
 }

void ASCharacter::JumpAction()
{
	Jump();
}

 void ASCharacter::PrimaryInteract()
 {
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
 }

 void ASCharacter::PrimaryAttack_TimeElapsed()
 {
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FVector CameraLocation;
	FRotator CameraRotation;

	CameraLocation = CameraComp->GetComponentLocation();
	CameraRotation = CameraComp->GetComponentRotation();

	FVector End = CameraLocation + (CameraRotation.Vector() * 5000);
	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, ObjectQueryParams);

	float Radius = 30.f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	FVector IP = Hit.bBlockingHit? Hit.ImpactPoint : Hit.TraceEnd;

	//타켓에서 핸드를 뺴면 백터가 나오고 rotation을 구함
	
	FTransform SpawnTM = FTransform((IP-HandLocation).Rotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
 }

 // Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Get the player controller
	APlayerController* PC = Cast<APlayerController>(GetController());
 
	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// Bind the actions
	PEI->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	PEI->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	PEI->BindAction(IA_PrimaryAttack, ETriggerEvent::Started, this, &ASCharacter::PrimaryAttack);
	PEI->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ASCharacter::JumpAction);
	PEI->BindAction(IA_PrimaryInteract, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
}
