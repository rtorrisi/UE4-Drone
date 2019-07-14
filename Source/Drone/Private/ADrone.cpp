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
	
	PIDRollController = new PIDSat(KPPitchRoll, KIPitchRoll, KDPitchRoll, 40.0f);
	PIDPitchController = new PIDSat(KPPitchRoll, KIPitchRoll, KDPitchRoll, 40.0f);

	ProfileGeneratorX = new ProfilePositionController(MaxSpeedX, MaxAccelerationX, MaxDecelerationX);
	ProfileGeneratorY = new ProfilePositionController(MaxSpeedY, MaxAccelerationY, MaxDecelerationY);
	ProfileGeneratorZ = new ProfilePositionController(MaxSpeedZ, MaxAccelerationZ, MaxDecelerationZ);
}

void AADrone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete PIDRollRateController;
	delete PIDPitchRateController;
	delete PIDYawRateController;
	delete PIDThrustController;
	delete PIDRollController;
	delete PIDPitchController;
	delete ProfileGeneratorX;
	delete ProfileGeneratorY;
	delete ProfileGeneratorZ;
}

// Called every frame
void AADrone::Tick(float DeltaTime)
{
	if (PIDRollRateController && PIDPitchRateController && PIDYawRateController && PIDThrustController && ProfileGeneratorX && ProfileGeneratorY && ProfileGeneratorZ)
	{
		if (AltitudeControllerEnable)
		{
			float ErrorZ = TargetZ - GetActorLocation().Z;
			float ErrorInMeterZ = ErrorZ / 100.f;
			float VelocityInMeterZ = GetVelocity().Z / 100.f;

			float TargetSpeedZ = ProfileGeneratorZ->evaluate(ErrorInMeterZ, VelocityInMeterZ, DeltaTime);
			Thrust = PIDThrustController->evaluate(TargetSpeedZ - VelocityInMeterZ, DeltaTime);
		}

		if (PositionControllerEnable)
		{
			FVector2D Error = FVector2D(TargetX - GetActorLocation().X, TargetY - GetActorLocation().Y);
			FVector2D ErrorInMeter = Error / 100.f;

			FVector2D VelocityInMeter = FVector2D(GetVelocity().X / 100.f, GetVelocity().Y / 100.f);
			
			FVector2D GlobalTargetSpeed = FVector2D(
				ProfileGeneratorX->evaluate(ErrorInMeter.X, VelocityInMeter.X, DeltaTime),
				ProfileGeneratorY->evaluate(ErrorInMeter.Y, VelocityInMeter.Y, DeltaTime)
			);
			
			float CosYaw = FMath::Cos(FMath::DegreesToRadians(-GetActorRotation().Yaw));
			float SinYaw = FMath::Sin(FMath::DegreesToRadians(-GetActorRotation().Yaw));

			FVector2D LocalCurrentSpeed = FVector2D(CosYaw * VelocityInMeter.X - SinYaw * VelocityInMeter.Y, SinYaw * VelocityInMeter.X + CosYaw * VelocityInMeter.Y);
			FVector2D LocalTargetSpeed = FVector2D(CosYaw * GlobalTargetSpeed.X - SinYaw * GlobalTargetSpeed.Y, SinYaw * GlobalTargetSpeed.X + CosYaw * GlobalTargetSpeed.Y);

			RollTarget = PIDRollController->evaluate(LocalTargetSpeed.Y - LocalCurrentSpeed.Y, DeltaTime);
			PitchTarget = -PIDPitchController->evaluate(LocalTargetSpeed.X - LocalCurrentSpeed.X, DeltaTime);
		}

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

