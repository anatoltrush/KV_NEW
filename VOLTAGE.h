#ifndef VOLTAGE_H
#define VOLTAGE_H

#define DEBUG

#include <LiquidCrystal.h>
#define DIFFER 0.76
#define LOW_VOLT 5.1
#define KV_LOW_VOLT 10.0
#define KV_MAX_VOLT 12.6
#define SIZE_OF_DATA 6

class Voltage
{
	char result[7] = { 'E', 'R', 'R', 'O', 'R', '_', 'V' };
	char result_kv[9] = { 'K', 'V', '_', 'V', 'O', 'L', 'T', 'A', 'G' };
	float temp;
	uint8_t volt_min;	//min voltage
	uint8_t voltage;	//max voltage
	uint8_t perc;		//voltage in percent
	float signal;		//analog signal
	float output;		//output signal
	uint8_t inPin;		//analog pin
	long period;		//period
	unsigned long previousMillis; //last read
	unsigned long prevMillis;	
public:
	bool is_conn;
	Voltage(uint8_t pin, long per, byte _min, byte _max) :
		temp(0),
		volt_min(_min),
		voltage(_max),
		perc(0),
		signal(0.0),
		output(0.0),
		inPin(pin),
		period(per),
		previousMillis(0),
		prevMillis(0),
		is_conn(false)
	{
		pinMode(inPin, INPUT);
	}
	void no_conn(LiquidCrystal &lcd) {
		if (is_conn == true) {
			lcd.setCursor(12, 0);
			lcd.print("!OK!");			
		}
		else
		{
			lcd.setCursor(12, 0);
			lcd.print("NO C");
		}
	}
	float update() {
		unsigned long currentMillis = millis(); //current time
		if (currentMillis - previousMillis >= period)
		{
			signal = analogRead(inPin); //read
			output = signal * voltage / 1024 + DIFFER;
			previousMillis = currentMillis; //remember time
			return output;
		}
		else return output;
	}
	void lcd_abs(LiquidCrystal &lcd, byte x, byte y) {
		lcd.setCursor(x, y);
		dtostrf(output, 3, 1, result);
		for (int i = 0; i < 3; i++)
			lcd.print(result[i]);
		lcd.print("V");
		if (output < 10)
			lcd.print(" ");
	}
	void lcd_abs(LiquidCrystal &lcd, byte x, byte y, float kv_volt) {
		if (kv_volt>=10)
		{
			lcd.setCursor(x, y);
			dtostrf(kv_volt, 4, 1, result_kv);
			for (int i = 0; i < 4; i++)
				lcd.print(result_kv[i]);
			lcd.print("V");
		}
		else if (kv_volt < 10) {
			lcd.setCursor((x-1), y);
			dtostrf(kv_volt, 4, 1, result_kv);
			for (int i = 0; i < 4; i++)
				lcd.print(result_kv[i]);
			lcd.print("V");
			lcd.print(" ");
		}
	}
	void lcd_per(LiquidCrystal &lcd, byte x, byte y) {
		lcd.setCursor(x, y);
		if (output >= LOW_VOLT) {
			perc = (output / voltage) * 100;
			lcd.print(perc);
			lcd.print("%");
			if (perc < 10)
				lcd.print(" ");
		}
		else
			lcd.print("LOW");
	}
	void lcd_per(LiquidCrystal &lcd, byte x, byte y, float kv_volt) {
		lcd.setCursor(x, y);
		if (kv_volt >= KV_LOW_VOLT) {
			perc = (kv_volt / KV_MAX_VOLT) * 100;
			lcd.print(perc);
			lcd.print("%");
			if (perc < 10)
				lcd.print(" ");
		}
		else
			lcd.print("LOW");
	}
	void show_all_pult_voltage(LiquidCrystal &lcd) {
		update();
		lcd_per(lcd, 2, 1);
		lcd_abs(lcd, 5, 1);
	}
	void show_all_kvadro_voltage(LiquidCrystal &lcd, float value) {
		unsigned long curMillis = millis(); //current time		
		if (curMillis - prevMillis >= period)
		{			
				temp = output;
				output = value;
				lcd_abs(lcd, 5, 0, output);
				lcd_per(lcd, 2, 0, output);
				output = temp;

				no_conn(lcd);

			prevMillis = curMillis; //remember time
		}
	}
};
#endif // VOLTAGE_H
//example---> Voltage volt(5, 1000, 9, 5);(pin, ms, max voltage, min voltage)
//--->float vvv = volt.update();
//--->volt.upd_lcd(lcd, 5, 1);
