// Fill out your copyright notice in the Description page of Project Settings.


#include "ADrone.h"
#include "PIDSat.h"
#include "ProfilePositionController.h"

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
	PIDThrustController = new PIDSat(KPThrust, KIThrust, KDThrust, 1.0f, 0.0f);
	ProfileGenerator = new ProfilePositionController(10.0f, 3.0f, 1.5f);

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

		ErrorRoll = Error.Roll;
		ErrorPitch = Error.Pitch;
		ErrorYaw = Error.Yaw;

		RollCmd = PIDRollRateController->evaluate(Error.Roll, DeltaTime);
		PitchCmd = PIDPitchRateController->evaluate(Error.Pitch, DeltaTime);
		YawCmd = PIDYawRateController->evaluate(Error.Yaw, DeltaTime);

		if(AltitudeControllerEnable)
		{
			float TargetSpeed = ProfileGenerator->evaluate(ZTarget / 100.f, GetActorLocation().Z / 100.f, GetVelocity().Z / 100.f, DeltaTime);
			Thrust = PIDThrustController->evaluate(TargetSpeed - GetVelocity().Z / 100.f, DeltaTime);
		}

		float MinThrustPWM1 = FMath::Max(0.f, -(-YawCmd + RollCmd + PitchCmd));
		float MinThrustPWM2 = FMath::Max(0.f, -(+YawCmd - RollCmd + PitchCmd));
		float MinThrustPWM3 = FMath::Max(0.f, -(-YawCmd - RollCmd - PitchCmd));
		float MinThrustPWM4 = FMath::Max(0.f, -(+YawCmd + RollCmd - PitchCmd));

		float MaxThrustPWM1 = FMath::Min(1.f, (1.f - (-YawCmd + RollCmd + PitchCmd)));
		float MaxThrustPWM2 = FMath::Min(1.f, (1.f - (+YawCmd - RollCmd + PitchCmd)));
		float MaxThrustPWM3 = FMath::Min(1.f, (1.f - (-YawCmd - RollCmd - PitchCmd)));
		float MaxThrustPWM4 = FMath::Min(1.f, (1.f - (+YawCmd + RollCmd - PitchCmd)));

		float ThrustPWM1 = FMath::Clamp(Thrust, MinThrustPWM1, MaxThrustPWM1);
		float ThrustPWM2 = FMath::Clamp(Thrust, MinThrustPWM2, MaxThrustPWM2);
		float ThrustPWM3 = FMath::Clamp(Thrust, MinThrustPWM3, MaxThrustPWM3);
		float ThrustPWM4 = FMath::Clamp(Thrust, MinThrustPWM4, MaxThrustPWM4);

		PWM1 = - YawCmd + RollCmd + PitchCmd + ThrustPWM1;
		PWM2 = + YawCmd - RollCmd + PitchCmd + ThrustPWM2;
		PWM3 = - YawCmd - RollCmd - PitchCmd + ThrustPWM3;
		PWM4 = + YawCmd + RollCmd - PitchCmd + ThrustPWM4;
		
	}
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

