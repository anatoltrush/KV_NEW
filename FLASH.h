#ifndef FLASH_H
#define FLASH_H

class Flasher
{
	uint8_t ledPin; // номер пина со светодиодом
    long OnTime; // время включения в миллисекундах
    long OffTime; // время, когда светодиод выключен
	uint8_t ledState; // состояние ВКЛ/ВЫКЛ
    unsigned long previousMillis; // последний момент смены состояния
  public:
    Flasher(uint8_t pin, long on, long off) {
      ledPin = pin;
      pinMode(ledPin, OUTPUT);

      OnTime = on;
      OffTime = off;

      ledState = LOW;
      previousMillis = 0;
    }
    void update() {
      unsigned long currentMillis = millis(); // текущее время в миллисекундах
      if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
      {
        ledState = LOW; // выключаем
        previousMillis = currentMillis; // запоминаем момент времени
        digitalWrite(ledPin, ledState); // реализуем новое состояние
      }
      else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
      {
        ledState = HIGH; // выключаем
        previousMillis = currentMillis ; // запоминаем момент времени
        digitalWrite(ledPin, ledState); // реализуем новое состояние
      }
    }
};
#endif // FLASH_H
//example---> Flasher led1(5, 500, 400);
//--->led1.Update();
