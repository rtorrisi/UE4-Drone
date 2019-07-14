#pragma once

#include "GenericPlatform/GenericPlatformMath.h"

class ProfilePositionController
{
	float accel;
	float max_speed;
	float decel;
	float decel_distance;
	float output_speed;
public:
	ProfilePositionController(float max_speed, float accel, float decel)
	{
		this->accel = accel;
		this->max_speed = max_speed;
		this->decel = decel;
		this->decel_distance = max_speed * max_speed / (2.0 * decel);
		this->output_speed = 0.0f; // la velocita' a cui andremo
	}

	float evaluate(float error, float current_speed, float delta_t)
	{
		// calcoliamo il segno e usiamo distanze sempre positive
		float s;
		if (error >= 0)
		{
			s = 1;
		}
		else
		{
			s = -1;
			error = -error;
		}

		if (error < decel_distance)
		{
			//ok siamo nella fase di decelerazione
			float vel_attesa = FGenericPlatformMath::Sqrt(FMath::Max(0.0f, max_speed * max_speed - 2 * decel * (decel_distance - error)));
			if (vel_attesa > output_speed)
			{
				//uhm... strana condizione,
				//vuol dire che siamo ancora in accelerazione(fase 1)
				//continuiamo ad accelerare
				output_speed += accel * delta_t;
				//controlliamo se abbiamo comunque raggiunto(e superato) la velocita' attesa
				if (output_speed > vel_attesa)
				{
					output_speed = vel_attesa;
				}
				// evitiamo anche di superare la velocita' massima
				if (output_speed > max_speed)
				{
					output_speed = max_speed;
				}
			}
			else
			{
				//qui siamo effettivamente in decelerazione
				output_speed = vel_attesa;
			}
		}
		else
		{
			//non siamo nella fase di decelerazione quindi...
			if (output_speed < max_speed)
			{
				//se non siamo gia' a velocita' massima, acceleriamo
				output_speed += accel * delta_t;
				//ma evitiamo sempre di superare la velocita' massima
				if (output_speed > max_speed)
				{
					output_speed = max_speed;
				}

			}
		}
		//applichiamo il segno
		return s * output_speed;
	}
};
