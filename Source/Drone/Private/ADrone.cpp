// Fill out your copyright notice in the Description page of Project Settings.


#include "ADrone.h"

// Sets default values
AADrone::AADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PIDRollRateController = new PIDSat(2.0f, 4.0f, 3.0f, 15.0f);
	PIDPitchRateController = new PIDSat(2.0f, 4.0f, 3.0f, 15.0f);
	PIDYawRateController = new PIDSat(2.0f, 4.0f, 3.0f, 15.0f);
}

// Called when the game starts or when spawned
void AADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float RollCmd = PIDRollRateController->evaluate(RollTarget, GetActorRotation().Roll, DeltaTime);
	float PitchCmd = PIDRollRateController->evaluate(PitchTarget, GetActorRotation().Pitch, DeltaTime);
	float YawCmd = 0.0f;
	
	PWM1 = Thrust - YawCmd + RollCmd + PitchCmd;
	PWM2 = Thrust + YawCmd - RollCmd + PitchCmd;
	PWM3 = Thrust - YawCmd - RollCmd - PitchCmd;
	PWM4 = Thrust + YawCmd + RollCmd - PitchCmd;
}

// Called to bind functionality to input
void AADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

