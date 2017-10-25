#ifndef DIRECT_H
#define DIRECT_H

#include "GYROSCOPE.h"

#define DEBUG

#define MIN_POWER 800
#define MAX_POWER 2300
#define POWER_TO_START 880
#define POWER_TO_UP 1200
#define POWER_TANG_KREN 500
#define POWER_YAW 500
#define POWER_FOR_PERIOD 5
#define PERIOD 20

uint16_t standart[4] = { MIN_POWER, MIN_POWER, MIN_POWER, MIN_POWER };
unsigned long currr = 0;
unsigned long prevvv = 0;
unsigned long _currr = 0;
unsigned long _prevvv = 0;
uint8_t add = 0;

void height_up(uint8_t value) {
	currr = millis();
	if (currr - prevvv >= PERIOD)
	{
		for (uint8_t i = 0; i < 4; i++)
			standart[i] += value;
		prevvv = currr; //remember time
	}
}
void height_down(uint8_t value) {
	_currr = millis();
	if (_currr - _prevvv >= PERIOD)
	{
		for (uint8_t i = 0; i < 4; i++)
			standart[i] -= value;
		_prevvv = _currr; //remember time
	}
}
void gyro_stab(uint16_t(&power)[4]) {

}
//LEFT_X
void axis_x_l(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	if (standart[0] > POWER_TO_UP && standart[1] > POWER_TO_UP && standart[2] > POWER_TO_UP && standart[3] > POWER_TO_UP)
	{
		if (data_upr[0] >= 127)
		{
			power[0] += map(data_upr[0], 127, 255, 0, POWER_YAW);
			power[2] += map(data_upr[0], 127, 255, 0, POWER_YAW);
			power[1] -= map(data_upr[0], 127, 255, 0, POWER_YAW);
			power[3] -= map(data_upr[0], 127, 255, 0, POWER_YAW);
		}
		else if (data_upr[0] < 127)
		{
			power[1] += map(data_upr[0], 127, 0, 0, POWER_YAW);
			power[3] += map(data_upr[0], 127, 0, 0, POWER_YAW);
			power[0] -= map(data_upr[0], 127, 0, 0, POWER_YAW);
			power[2] -= map(data_upr[0], 127, 0, 0, POWER_YAW);
		}
	}
}
//LEFT_Y
void axis_y_l(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
#ifdef DEBUG
	if (standart[0]<POWER_TO_UP && standart[1]<POWER_TO_UP && standart[2]<POWER_TO_UP && standart[3]<POWER_TO_UP) {
#endif // DEBUG
		if (data_upr[1] > 127) {
			add = map(data_upr[1], 127, 255, 0, POWER_FOR_PERIOD);
			height_up(add);
		}
		else if (data_upr[1] < 127) {
			add = map(data_upr[1], 127, 0, 0, POWER_FOR_PERIOD*10);
			height_down(add);
		}
#ifdef DEBUG
	}
	else
	{
		if (data_upr[1] < 127) {
			add = map(data_upr[1], 127, 0, 0, POWER_FOR_PERIOD*10);
			height_down(add);
		}
	}
#endif // DEBUG
}
//RIGHT_X
void axis_x_r(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	if (standart[0]>POWER_TO_UP && standart[1]>POWER_TO_UP && standart[2]>POWER_TO_UP && standart[3]>POWER_TO_UP)
	{
		if (data_upr[2] >= 127)
		{
			power[1] += map(data_upr[2], 127, 255, 0, POWER_TANG_KREN);
			power[2] += map(data_upr[2], 127, 255, 0, POWER_TANG_KREN);
		}
		else if (data_upr[2] < 127)
		{
			power[0] += map(data_upr[2], 127, 0, 0, POWER_TANG_KREN);
			power[3] += map(data_upr[2], 127, 0, 0, POWER_TANG_KREN);
		}
	}
}
//RIGHT_Y
void axis_y_r(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	if (standart[0]>POWER_TO_UP && standart[1]>POWER_TO_UP && standart[2]>POWER_TO_UP && standart[3]>POWER_TO_UP)
	{
		if (data_upr[3] >= 127)
		{
			power[2] += map(data_upr[3], 127, 255, 0, POWER_TANG_KREN);
			power[3] += map(data_upr[3], 127, 255, 0, POWER_TANG_KREN);
		}
		else if (data_upr[3] < 127)
		{
			power[0] += map(data_upr[3], 127, 0, 0, POWER_TANG_KREN);
			power[1] += map(data_upr[3], 127, 0, 0, POWER_TANG_KREN);
		}
	}
}
//
void left_joystick(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	//axis_x_l(power, data_upr);
	axis_y_l(power, data_upr);
}
void right_joystick(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	axis_x_r(power, data_upr);
	axis_y_r(power, data_upr);
}
void check(uint16_t(&power)[4]) {
	for (uint8_t i = 0; i < 4; i++) {
		if (power[i] >= 2300)
			power[i] = 2290;
		if (power[i] <= 800)
			power[i] = 800;

		if (standart[i] >= 2300)
			standart[i] = 2290;
		if (standart[i] <= 800)
			standart[i] = 800;
	}
}
void calculate(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	//ASSIGNMENT
	for (uint8_t i = 0; i < 4; i++)
		power[i] = standart[i];
	//CALCULATE
	gyro_stab(power);
	left_joystick(power, data_upr);
	//right_joystick(power, data_upr);
	check(power);
}
#endif // DIRECT_H