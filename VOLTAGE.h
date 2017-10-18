#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <LiquidCrystal.h>
#define DIFFER 0.76
#define LOW_VOLT 5.1

class Voltage
{
    char result[7] = {'E', 'R', 'R', 'O', 'R', '_', 'V'};
	uint8_t volt_min;//min voltage
	uint8_t voltage;//max voltage
	uint8_t perc;//voltage in percent
    float signal;//analog signal
    float output;//output signal
	uint8_t inPin; //analog pin
    long period; //period
    unsigned long previousMillis; //last read
  public:
    Voltage(uint8_t pin, long per, byte _min, byte _max):
		volt_min(_min),
		voltage(_max),
		perc(0),
		signal(0.0),
		output(0.0),
		inPin(pin),
		period(per),
		previousMillis(0)
	{
		pinMode(inPin, INPUT);
	}
    float update() {
      unsigned long currentMillis = millis(); //current time
      if (currentMillis - previousMillis >= period)
      {
        signal = analogRead(inPin); //read
        output = signal * voltage / 1024 + DIFFER;
        previousMillis = currentMillis ; //remember time
        return output;
      }
      else return output;
    }
    void lcd_abs(LiquidCrystal &lcd, byte x, byte y) {
      update();
      lcd.setCursor(x, y);
      dtostrf(output, 3, 1, result);
      for (int i = 0; i < 3; i++)
        lcd.print(result[i]);
      lcd.print("V");
      if (output < 10)
        lcd.print(" ");
    }
    void lcd_per(LiquidCrystal &lcd, byte x, byte y) {
      update();
      lcd.setCursor(x, y);
      if (output >= LOW_VOLT) {
        perc = ((output - volt_min) / (voltage - volt_min)) * 100;
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
	void show_all_kvadro_voltage() {

	}
};
#endif // VOLTAGE_H
//example---> Voltage volt(5, 1000, 9, 5);(pin, ms, max voltage, min voltage)
//--->float vvv = volt.update();
//--->volt.upd_lcd(lcd, 5, 1);
