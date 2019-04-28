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
	PIDThrustController = new PIDSat(KPThrust, KIThrust, KDThrust, SatThrust, 0.0f);
	ProfileGenerator = new ProfilePositionController(10.0f, 5.0f, 2.5f);
}

void AADrone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete PIDRollRateController;
	delete PIDPitchRateController;
	delete PIDYawRateController;
	delete PIDThrustController;
	delete ProfileGenerator;
}

// Called every frame
void AADrone::Tick(float DeltaTime)
{
	if (PIDRollRateController && PIDPitchRateController && PIDYawRateController && PIDThrustController && ProfileGenerator)
	{
		FRotator TargetRotator = FRotator(PitchTarget, YawTarget, RollTarget);
		TargetRotator.Normalize();
		FRotator Error = TargetRotator - GetActorRotation();
		Error.Normalize();
		float RollCmd = PIDRollRateController->evaluate(Error.Roll, DeltaTime);
		float PitchCmd = PIDPitchRateController->evaluate(Error.Pitch, DeltaTime);
		float YawCmd = PIDYawRateController->evaluate(Error.Yaw, DeltaTime);

		float TargetSpeed = ProfileGenerator->evaluate(ZTarget / 100.f, GetActorLocation().Z / 100.f, GetVelocity().Z / 100.f, DeltaTime);
		Thrust = PIDThrustController->evaluate(TargetSpeed - GetVelocity().Z / 100.f, DeltaTime);

		float PWM1cmd = - YawCmd + RollCmd + PitchCmd;
		float PWM2cmd = + YawCmd - RollCmd + PitchCmd;
		float PWM3cmd = - YawCmd - RollCmd - PitchCmd;
		float PWM4cmd = + YawCmd + RollCmd - PitchCmd;

		PWM1cmd = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM1cmd);
		PWM2cmd = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM2cmd);
		PWM3cmd = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM3cmd);
		PWM4cmd = FMath::GetMappedRangeValueClamped(FVector2D(-3.0f, 3.0f), FVector2D(0.0f, 1.0f), PWM4cmd);

		const float ThrustPerc = 0.70f;
		const float CmdPerc = 0.3f;

		PWM1 = ThrustPerc * Thrust + CmdPerc * PWM1cmd;
		PWM2 = ThrustPerc * Thrust + CmdPerc * PWM2cmd;
		PWM3 = ThrustPerc * Thrust + CmdPerc * PWM3cmd;
		PWM4 = ThrustPerc * Thrust + CmdPerc * PWM4cmd;
	}
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

