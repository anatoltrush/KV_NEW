#ifndef DIRECT_H
#define DIRECT_H

#include "GYROSCOPE.h"

#define DEBUG
#define TEST

#define MIN_POWER 800
#define MAX_POWER 2300
#define POWER_TO_START 880
#define POWER_TO_UP 1000
#define PERIOD 100

uint16_t standart[4] = { MIN_POWER, MIN_POWER, MIN_POWER, MIN_POWER };
unsigned long currr = 0;
unsigned long prevvv = 0;
uint8_t add = 0;

uint16_t height(uint8_t value, uint8_t flag) {
	if (currr - prevvv >= PERIOD)
	{
		//begin action
		for (uint8_t i = 0; i < 4; i++) {
			if (flag == 0)
				standart[i] += value;
			else if(flag == 1)
				standart[i] -= value;
		}
		//end action
		prevvv = currr; //remember time
		return 1;
	}
	else return 0;
}
void gyro_stab(uint16_t(&power)[4]) {

}
//LEFT_X
void axis_x_l(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	if (standart[0] > POWER_TO_UP && standart[1] > POWER_TO_UP && standart[2] > POWER_TO_UP && standart[3] > POWER_TO_UP)
	{
		if (data_upr[0] >= 127)
		{
			power[0] += map(data_upr[0], 127, 255, 0, 1000);/////////////////[0]///////////////////
			power[0] += map(data_upr[0], 127, 255, 0, 1000);/////////////////[0]///////////////////
			power[0] -= map(data_upr[0], 127, 255, 0, 1000);/////////////////[0]///////////////////
			power[0] -= map(data_upr[0], 127, 255, 0, 1000);/////////////////[0]///////////////////
		}
		else if (data_upr[0] < 127)
		{
			power[0] += map(data_upr[0], 127, 0, 0, 1000);/////////////////[0]///////////////////
			power[0] += map(data_upr[0], 127, 0, 0, 1000);/////////////////[0]///////////////////
			power[0] -= map(data_upr[0], 127, 0, 0, 1000);/////////////////[0]///////////////////
			power[0] -= map(data_upr[0], 127, 0, 0, 1000);/////////////////[0]///////////////////
		}
	}
}
//LEFT_Y
void axis_y_l(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
#ifdef DEBUG
	if (standart[0]<POWER_TO_UP && standart[1]<POWER_TO_UP && standart[2]<POWER_TO_UP && standart[3]<POWER_TO_UP) {
#endif // DEBUG
		if (data_upr[1] > 127) {
			add = map(data_upr[1], 127, 255, 0, 100);
			height(add, 0);
		}
		else if (data_upr[1] < 127) {
			add = map(data_upr[1], 127, 0, 0, 100);
			height(add, 1);
		}
#ifdef DEBUG
	}
#endif // DEBUG
}
//RIGHT_X
void axis_x_r(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {//CHANGE_power[0]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (standart[0]>POWER_TO_UP && standart[1]>POWER_TO_UP && standart[2]>POWER_TO_UP && standart[3]>POWER_TO_UP)
	{
		if (data_upr[2] >= 127)
		{
			power[0] += map(data_upr[2], 127, 255, 0, 1000);/////////////////[0]///////////////////
			power[0] += map(data_upr[2], 127, 255, 0, 1000);/////////////////[0]///////////////////
		}
		else if (data_upr[2] < 127)
		{
			power[0] += map(data_upr[2], 127, 0, 0, 1000);/////////////////[0]///////////////////
			power[0] += map(data_upr[2], 127, 0, 0, 1000);/////////////////[0]///////////////////
		}
	}
}
//RIGHT_Y
void axis_y_r(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {//CHANGE_power[0]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (standart[0]>POWER_TO_UP && standart[1]>POWER_TO_UP && standart[2]>POWER_TO_UP && standart[3]>POWER_TO_UP)
	{
		if (data_upr[3] >= 127)
		{
			power[0] += map(data_upr[3], 127, 255, 0, 1000);/////////////////[0]///////////////////
			power[0] += map(data_upr[3], 127, 255, 0, 1000);/////////////////[0]///////////////////
		}
		else if (data_upr[3] < 127)
		{
			power[0] += map(data_upr[3], 127, 0, 0, 1000);/////////////////[0]///////////////////
			power[0] += map(data_upr[3], 127, 0, 0, 1000);/////////////////[0]///////////////////
		}
	}
}
//
void left_joystick(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	axis_x_l(power, data_upr);
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
	right_joystick(power, data_upr);
	check(power);
}
#endif // DIRECT_H