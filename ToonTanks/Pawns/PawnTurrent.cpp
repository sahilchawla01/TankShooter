// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurrent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"


void APawnTurrent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurrent::CheckFireCondition, FireRate, true); 

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}   

// Called every frame
void APawnTurrent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if(!PlayerPawn || ReturnDistanceToPlayer() > Range)
        return;

    RotateTurrent(PlayerPawn->GetActorLocation());
    
}

void APawnTurrent::CheckFireCondition()
{
    if(!PlayerPawn || !PlayerPawn->GetIsPlayerAlive()) //ded
        return;

    if(ReturnDistanceToPlayer() <= Range)
    {
        Fire();
    }

    
}


float APawnTurrent::ReturnDistanceToPlayer()
{
    if(!PlayerPawn)
        return 0.f;

    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());

}

void APawnTurrent::HandleDestruction() 
{
    //Call base for playing effects
    Super::HandleDestruction();

    Destroy();
    
}