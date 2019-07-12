// Fill out your copyright notice in the Description page of Project Settings.


#include "ADrone.h"

// Sets default values
AADrone::AADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AADrone::BeginPlay()
{
	Super::BeginPlay();

	PIDRollRateController = new PIDSat(KP, KI, KD);
	PIDPitchRateController = new PIDSat(KP, KI, KD);
	PIDYawRateController = new PIDSat(KP, KI, KD);
}

void AADrone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete PIDRollRateController;
	delete PIDPitchRateController;
	delete PIDYawRateController;
}

// Called every frame
void AADrone::Tick(float DeltaTime)
{
	if (PIDRollRateController && PIDPitchRateController && PIDYawRateController)
	{
		FRotator TargetRotator = FRotator(PitchTarget, YawTarget, RollTarget);
		TargetRotator.Normalize();
		FRotator Error = TargetRotator - GetActorRotation();
		Error.Normalize();

		ErrorRoll = Error.Roll;
		ErrorPitch = Error.Pitch;
		ErrorYaw = Error.Yaw;

		RollCmd = PIDRollRateController->evaluate(Error.Roll, DeltaTime);
		PitchCmd = PIDPitchRateController->evaluate(Error.Pitch, DeltaTime);
		YawCmd = PIDYawRateController->evaluate(Error.Yaw, DeltaTime);
		YawCmd = 0;

		//PWM1 = - YawCmd + RollCmd + PitchCmd;
		//PWM2 = + YawCmd - RollCmd + PitchCmd;
		//PWM3 = - YawCmd - RollCmd - PitchCmd;
		//PWM4 = + YawCmd + RollCmd - PitchCmd;

		PWM1 = Thrust + PitchCmd;
		PWM2 = Thrust + PitchCmd;
		PWM3 = Thrust - PitchCmd;
		PWM4 = Thrust - PitchCmd;

		//PWM1 = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(0.0f, 1.0f), PWM1) * Thrust;
		//PWM2 = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(0.0f, 1.0f), PWM2) * Thrust;
		//PWM3 = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(0.0f, 1.0f), PWM3) * Thrust;
		//PWM4 = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(0.0f, 1.0f), PWM4) * Thrust;
		
	}
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

