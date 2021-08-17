// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGamemodeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurrent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Controller/PlayerControllerBase.h"

void ATankGamemodeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATankGamemodeBase::HandleGameStart()
{
    TargetTurrents = GetTargetTurrentCount();

    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    
    GameStart();

    if(PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);
        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}
void ATankGamemodeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}
void ATankGamemodeBase::ActorDied(AActor* DeadActor)
{
    if(DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);
        
        if(PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if(APawnTurrent* DestroyedTurrent = Cast<APawnTurrent>(DeadActor)) // if destroyed actor is turrent
    {
        DestroyedTurrent->HandleDestruction();

        if(--TargetTurrents == 0)
        {
            HandleGameOver(true);
        }
    }
}   


int ATankGamemodeBase::GetTargetTurrentCount()
{
    TArray<AActor*> TurrentActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurrent::StaticClass(), TurrentActors);
    return TurrentActors.Num();


}