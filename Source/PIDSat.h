#pragma once

class PIDSat 
{

private:

	float kp = 0.0f;
	float ki = 0.0f;
	float kd = 0.0f;
	float saturation = 0.0f;
	float integral = 0.0f;
	float prev_error = 0.0f;
	bool saturation_flag = false;

public:

	PIDSat(float kp, float ki, float kd, float sat)
	{
		this->kp = kp;
		this->ki = ki;
		this->kd = kd;
		this->saturation = sat;
	}

	float evaluate(float error, float delta_t)
	{
		if (!saturation_flag)
		{
			integral = integral + error * delta_t;
		}		
		float deriv = (error - prev_error) / delta_t;
		prev_error = error;
		float output = kp * error + ki * integral + kd * deriv;
		if (output > saturation)
		{
			output = saturation;
			saturation_flag = true;
		}
		else if (output < -saturation)
		{
			output = -saturation;
			saturation_flag = true;
		}
		else
		{
			saturation_flag = false;
		}
		return output;
	}
};
	