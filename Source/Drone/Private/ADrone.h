// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PIDSat.h"
#include "ADrone.generated.h"

UCLASS()
class AADrone : public APawn
{
	GENERATED_BODY()

private:
	PIDSat* PIDRollRateController = nullptr;
	PIDSat* PIDPitchRateController = nullptr;
	PIDSat* PIDYawRateController = nullptr;

public:
	// Sets default values for this pawn's properties
	AADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM4;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float PitchTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float RollTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float Thrust;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
