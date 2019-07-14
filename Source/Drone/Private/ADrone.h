// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ADrone.generated.h"

class PIDSat;
class ProfilePositionController;

UCLASS()
class AADrone : public APawn
{
	GENERATED_BODY()

private:
	PIDSat* PIDRollRateController = nullptr;
	PIDSat* PIDPitchRateController = nullptr;
	PIDSat* PIDYawRateController = nullptr;
	PIDSat* PIDThrustController = nullptr;

	PIDSat* PIDRollController = nullptr;
	PIDSat* PIDPitchController = nullptr;

	ProfilePositionController* ProfileGeneratorZ = nullptr;
	ProfilePositionController* ProfileGeneratorY = nullptr;
	ProfilePositionController* ProfileGeneratorX = nullptr;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CMD)
		float RollCmd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CMD)
		float PitchCmd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CMD)
		float YawCmd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Err)
		float ErrorRoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Err)
		float ErrorPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Err)
		float ErrorYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float PitchTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float RollTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float YawTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float Thrust = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float TargetX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float TargetY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float TargetZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float Sat = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KPThrust = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KIThrust = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PIDSatController)
		float KDThrust = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float KPPitchRoll = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float KIPitchRoll = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float KDPitchRoll = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AltitudeController)
		bool AltitudeControllerEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AltitudeController)
		bool PositionControllerEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxSpeedX = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxSpeedY = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxSpeedZ = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxAccelerationX = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxAccelerationY = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxAccelerationZ = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxDecelerationX = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxDecelerationY = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProfileController)
		float MaxDecelerationZ = 1.5f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
