// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	InitialLifeSpan = 2.f;

	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	ProjectileMesh->OnComponentHit.AddDynamic(this , &AProjectileBase::OnHit);
}


void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* ThisActor = GetOwner();


	UE_LOG(LogTemp, Error, TEXT("OnHit"));
	if(!ThisActor)
		return;
	
	if(OtherActor && OtherActor != this && OtherActor != ThisActor) //If the actor isn't self and exists
	{
		UE_LOG(LogTemp, Error, TEXT("Actor itself"));
		UGameplayStatics::ApplyDamage(OtherActor, Damage, ThisActor->GetInstigatorController(), this, DamageType);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, OtherActor->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitShake);
		Destroy();
	}
	
}

