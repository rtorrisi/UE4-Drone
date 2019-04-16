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

	PIDRollRateController = new PIDSat(KP, KI, KD, Sat);
	PIDPitchRateController = new PIDSat(KP, KI, KD, Sat);
	PIDYawRateController = new PIDSat(KP, KI, KD, Sat);
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
		float RollCmd = PIDRollRateController->evaluate(RollTarget, GetActorRotation().Roll, DeltaTime);
		float PitchCmd = PIDPitchRateController->evaluate(PitchTarget, GetActorRotation().Pitch, DeltaTime);
		float YawCmd = 0.0f;

		PWM1 = - YawCmd + RollCmd + PitchCmd;
		PWM2 = + YawCmd - RollCmd + PitchCmd;
		PWM3 = - YawCmd - RollCmd - PitchCmd;
		PWM4 = + YawCmd + RollCmd - PitchCmd;

		PWM1 = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM1) * Thrust;
		PWM2 = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM2) * Thrust;
		PWM3 = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM3) * Thrust;
		PWM4 = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM4) * Thrust;
		
	}
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

