#pragma once

class PIDSat 
{

private:

	float kp = 0.0f;
	float ki = 0.0f;
	float kd = 0.0f;
	float saturation = 0.0f;
	float base_saturation = 0.0f;
	float integral = 0.0f;
	float prev_error = 0.0f;
	bool saturation_flag = false;
	bool first = true;

public:

	PIDSat(float kp, float ki, float kd, float sat) : PIDSat(kp, ki, kd, sat, -sat) {}

	PIDSat(float kp, float ki, float kd, float sat, float base_sat)
	{
		this->kp = kp;
		this->ki = ki;
		this->kd = kd;
		this->saturation = sat;
		this->base_saturation = base_sat;
	}

	float evaluate(float error, float delta_t)
	{
		if (!saturation_flag)
		{
			integral = integral + error * delta_t;
		}
		float deriv = 0.0f;
		if (!first)
		{
			deriv = (error - prev_error) / delta_t;
		}
		else
		{
			first = false;
		}
		prev_error = error;

		float output = kp * error + ki * integral + kd * deriv;
		if (output > saturation)
		{
			output = saturation;
			saturation_flag = true;
		}
		else if (output < base_saturation)
		{
			output = base_saturation;
			saturation_flag = true;
		}
		else
		{
			saturation_flag = false;
		}
		return output;
	}
};
	