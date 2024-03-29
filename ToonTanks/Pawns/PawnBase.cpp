// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurrentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turrent Mesh"));
	TurrentMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurrentMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void APawnBase::RotateTurrent(FVector LookAtTarget)
{
	//Update turrent rotation to look at target
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, TurrentMesh->GetComponentLocation().Z); //cleans LookAtTarget so we don't have weird rotations
	FVector StartLocation = TurrentMesh->GetComponentLocation();
	FRotator TurrentRotation = FVector(LookAtTargetClean - StartLocation).Rotation();

	TurrentMesh->SetWorldRotation(TurrentRotation);

}

void APawnBase::Fire()
{
	if(ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this); //This is done so that the projectile doesn't harm the actor that spawned it
	}
}

void APawnBase::HandleDestruction()
{
	//Play death effects particle, sound and camera shake
	UGameplayStatics::SpawnEmitterAtLocation(this , DeathParticle, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathShake);
	//Child overrides:
	//PawnTurrent Turrent died
	//PawnTank Player died, hide all visual components and stop movement input
}


