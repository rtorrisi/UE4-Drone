#pragma once

class PIDSat 
{

private:

	float kp = 0.0f;
	float ki = 0.0f;
	float kd = 0.0f;
	float integral = 0.0f;
	float prev_error = 0.0f;

public:

	PIDSat(float kp, float ki, float kd)
	{
		this->kp = kp;
		this->ki = ki;
		this->kd = kd;
	}

	float evaluate(float error, float delta_t)
	{
		integral = integral + error * delta_t;	// -0.64
		float deriv = (error - prev_error) / delta_t; // -2500
		prev_error = error;
		float output = kp * error + ki * integral + kd * deriv;
		return output;
	}
};
