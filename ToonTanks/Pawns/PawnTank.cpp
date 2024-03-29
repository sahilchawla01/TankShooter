// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Rotate();
    Move();

    if(PlayerControllerRef)
    {
        FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurrent(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateTurnInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);

}

void APawnTank::CalculateMoveInput(float Value)
{
    MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}
void  APawnTank::CalculateTurnInput(float Value)
{
    float TurnAmount = Value * TurnSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, TurnAmount, 0);
    TurnDirection = FQuat(Rotation);
}

void  APawnTank::Move()
{
    AddActorLocalOffset(MoveDirection, false);
}
void  APawnTank::Rotate()
{
    AddActorLocalRotation(TurnDirection, false);
}


void APawnTank::HandleDestruction() 
{
    //Call base for playing effects
    Super::HandleDestruction();
    bIsPlayerAlive = false;
    
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

}

bool APawnTank::GetIsPlayerAlive()
{
    return bIsPlayerAlive;
}