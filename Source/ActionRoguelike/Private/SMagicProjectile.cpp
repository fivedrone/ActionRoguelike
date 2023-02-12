// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	ProjectileSpeed = 1000.0f;
	LifeTime = 5.0f;
	Damage = 20.0f;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName(ProjectileType);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = ProjectileSpeed;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = LifeTime;
	AttackRange = LifeTime * ProjectileSpeed;
}

void ASMagicProjectile::BeginPlay()
{
	
	Super::BeginPlay();

	MovementComp->StopMovementImmediately();

	Owner = IsValid(GetOwner()) ? GetOwner() : GetInstigator();

	SphereComp->IgnoreActorWhenMoving(Owner, true);
	
	InitTrace() ? SceneTrace() : ObjectTrace();

	FVector Vel = MovementComp->InitialSpeed * UKismetMathLibrary::GetForwardVector(GetActorRotation());

	MovementComp->Velocity = Vel;
	
}

bool ASMagicProjectile::InitTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("InitTrace"));
	if(!IsValid(Owner)) return false; // Owner가 없는 MagicProjectile의 경우에는? ex: projectile Cannon

	APlayerController* FirstPlayerController = Owner->GetWorld()->GetFirstPlayerController(); // HUD 정보를 가지고있는 FirstPlayerController를 Owner에서 뽑아냄
	if (!IsValid(FirstPlayerController)) return false;

	int32   SizeX;
	int32   SizeY;
	FirstPlayerController->GetViewportSize(SizeX, SizeY);
	// GetViewportSize에서는 매개변수를 0으로 초기화하기에 값을 줄 필요 없음

	ScreenPosition.X = SizeX * 0.5f;
	ScreenPosition.Y = SizeY * 0.5f;
	// 스크린 좌표의 중간값은 곧 화면 중앙을 의미.
	AHUD* HUD = FirstPlayerController->GetHUD();
	if (!IsValid(HUD) && HUD->GetHitBoxAtCoordinates(ScreenPosition, true)) return false;

	ULocalPlayer* const LP = FirstPlayerController ? FirstPlayerController->GetLocalPlayer() : nullptr;

	if(LP && LP->ViewportClient)
	{
		FSceneViewProjectionData ProjectionData;
		if(LP->GetProjectionData(LP->ViewportClient->Viewport, ProjectionData))
		{
			FMatrix const InvViewProjMatrix = ProjectionData.ComputeViewProjectionMatrix().InverseFast();
			FSceneView::DeprojectScreenToWorld(ScreenPosition, ProjectionData.GetConstrainedViewRect(), InvViewProjMatrix, WorldPosition, WorldDirection);
			return true;
			// 나중에 계산식 찾아보고 정리해놓을것.
		}
	}
	WorldPosition = FVector::ZeroVector;
	WorldDirection = FVector::ZeroVector;
	return false;
	
}

void ASMagicProjectile::SceneTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("SceneTrace"));
	FHitResult HitResult;
	// UE_LOG(LogTemp, Warning, TEXT("end up %s, %s"), *WorldPosition.ToString(), *WorldDirection.ToString());
	FVector End = WorldPosition + WorldDirection * AttackRange;
	
	UE_LOG(LogTemp, Warning, TEXT("End %s"), *End.ToString());
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	// 나중에 배열로 묶고 for문으로 처리하기로... blueprint에서 배열 수정도 가능하게?
	float Radius = 30.f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	bool bBlockHit = GetWorld()->LineTraceSingleByObjectType(HitResult, WorldPosition, End, ObjectQueryParams);
	if(IsValid(HitResult.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLocation %s"), *HitResult.GetActor()->GetClass()->GetFName().ToString());
	}
	FVector TargetLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
	UE_LOG(LogTemp, Warning, TEXT("TargetLocation %s"), *TargetLocation.ToString());
	SetActorRotation((TargetLocation-GetActorLocation()).Rotation());
	UE_LOG(LogTemp, Warning, TEXT("SetActorRotator %s"), *(TargetLocation-GetActorLocation()).Rotation().ToString());
	FColor Linecolor = bBlockHit ? FColor::Green : FColor::Red;
	
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 32, Linecolor, false, 2.0f);
	DrawDebugLine(GetWorld(),WorldPosition, End, Linecolor, false, 2.0f, 0, 2.0f);
}

void ASMagicProjectile::ObjectTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("ObjectTrace"));
	SetActorRotation(Owner->GetActorRotation());
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-Damage);
		}
		
		Destroy();
	}
}

void ASMagicProjectile::Destroyed()
{
	if(IsValid(Owner))
	{
		Super::Destroyed();
	
		UE_LOG(LogTemp, Warning, TEXT("M_Destroyed"));
		if(IsValid(ExplodeSlot))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeSlot, FTransform(GetActorRotation(), GetActorLocation()));
		}
	}
}
