#ifndef DIRECT_H
#define DIRECT_H

#include "GYROSCOPE.h"

#define MIN_POWER 870
#define MAX_POWER 2280

uint16_t standart[4] = { MIN_POWER, MIN_POWER, MIN_POWER, MIN_POWER };

void gyro_stab(uint16_t(&power)[4]) {

}
void axis_x_l() {
	for (uint8_t i = 0; i < 4; i++)
	{
	}
}
void axis_y_l() {
	for (uint8_t i = 0; i < 4; i++)
	{
	}
}
void axis_x_r(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	for (uint8_t i = 0; i < 4; i++)
	{
		if (data_upr[2] >= 127)
			power[i] = standart[i] + map(data_upr[1], 127, 255, 0, 1400);
		else if (data_upr[2] < 127)
			power[i] = standart[i] - map(data_upr[1], 127, 255, 0, 1400);
	}
}
void axis_y_r(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	for (uint8_t i = 0; i < 4; i++)
	{
		if (data_upr[3] >= 127)
			power[i] = standart[i] + map(data_upr[1], 127, 255, 0, 1400);
		else if (data_upr[3] < 127)
			power[i] = standart[i] - map(data_upr[1], 127, 255, 0, 1400);
	}
}
void left_joystick(uint16_t(&power)[4]) {
	axis_x_l();
	axis_y_l();
}
void right_joystick(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	axis_x_r(power, data_upr);
	axis_y_r(power, data_upr);
}
void calculate(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	gyro_stab(power);
	left_joystick(power);
	right_joystick(power, data_upr);
}

#endif // DIRECT_H