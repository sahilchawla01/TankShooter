// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGamemodeBase.generated.h"

/**
 * 
 */
class APawnTurrent;
class APawnTank;


UCLASS()
class TOONTANKS_API ATankGamemodeBase : public AGameModeBase
{
	GENERATED_BODY()
	private:
		APawnTank* PlayerTank;
		int32 TargetTurrents = 0;

		int32 GetTargetTurrentCount();

		void HandleGameStart();
		void HandleGameOver(bool PlayerWon);
		

	public:

		void ActorDied(AActor* DeadActor);

	protected:

		virtual void BeginPlay() override;
		UFUNCTION(BlueprintImplementableEvent)
		void GameStart();
		UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool PlayerWon);
};
