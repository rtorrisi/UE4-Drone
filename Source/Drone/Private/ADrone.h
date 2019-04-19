// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PIDSat.h"
#include "ProfilePositionController.h"
#include "ADrone.generated.h"

UCLASS()
class AADrone : public APawn
{
	GENERATED_BODY()

private:
	PIDSat* PIDRollRateController = nullptr;
	PIDSat* PIDPitchRateController = nullptr;
	PIDSat* PIDYawRateController = nullptr;
	PIDSat* PIDThrustController = nullptr;
	ProfilePositionController* ProfileGenerator = nullptr;

public:
	// Sets default values for this pawn's properties
	AADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PWM)
		float PWM4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float ZTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float PitchTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float RollTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float YawTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float Thrust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KPThrust = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KIThrust = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KDThrust = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float SatThrust = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float Sat;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
