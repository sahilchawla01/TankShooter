// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurrent.generated.h"


class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurrent : public APawnBase
{
	GENERATED_BODY()
	

	private:

	UPROPERTY(EditAnywhere, Category = "Turrent Mechanics", meta = (AllowPrivateAccess = "true"))
	float FireRate = 5.f;
	UPROPERTY(EditAnywhere, Category = "Turrent Mechanics", meta = (AllowPrivateAccess = "true"))
	float Range = 500.f;


	FTimerHandle FireRateTimerHandle;
	APawnTank* PlayerPawn;

	float ReturnDistanceToPlayer();

	void CheckFireCondition();
	
	public:

		virtual void Tick(float DeltaTime) override;

		virtual void HandleDestruction() override;

	protected:

		virtual void BeginPlay() override;

		
};
