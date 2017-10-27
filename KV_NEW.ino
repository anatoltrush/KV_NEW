#include <Servo.h>
#include "MOT_INIT.h"
#include "FLASH.h"
#include "VOLTAGE.h"
#include "DIRECT.h"
#include "GYROSCOPE.h"

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "DIRECT.h"

#define SIZE_OF_DATA 6
#define GYRO_PERIOD 10
//Read voltage
#define VOLTAGE_PIN A2
#define MIN_VOLTAGE 10//min voltage
#define MAX_VOLTAGE 12.6//max voltage
#define TIME_READ_VOLTAGE 3000

Voltage volt(VOLTAGE_PIN, TIME_READ_VOLTAGE, MIN_VOLTAGE, MAX_VOLTAGE);
Flasher led(8, 1000, 1000);
Gyro gyro(GYRO_PERIOD);

Servo motorFR;
Servo motorFL;
Servo motorRR;
Servo motorRL;

uint8_t data_upr[SIZE_OF_DATA] = { 0, 0, 0, 0 , 0 , 0 }; // for test
uint16_t power[4] = {MIN_POWER, MIN_POWER, MIN_POWER, MIN_POWER };

RF24 radio(9, 10);				// "создать" модуль на пинах 9 и 10
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" }; //возможные номера труб
byte pipeNo, gotByte;

void setup() {
	Serial.begin(9600);

  moto_init(motorFR, motorFL, motorRR, motorRL);

  radio.begin();				//активировать модуль
  radio.setAutoAck(1);			//режим подтверждени€ приЄма, 1 вкл 0 выкл
  radio.setRetries(0, 5);		//(врем€ между попыткой достучатьс€, число попыток)
  radio.enableAckPayload();		//разрешить отсылку данных в ответ на вход€щий сигнал
  radio.setPayloadSize(32);		//размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);		//выбираем канал (в котором нет шумов!)

  radio.setPALevel(RF24_PA_MAX);	//уровень мощности передатчика. Ќа выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(RF24_1MBPS);	//скорость обмена. Ќа выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
									//должна быть одинакова на приЄмнике и передатчике!
									//при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  radio.powerUp();					//начать работу
  radio.startListening();			//начинаем слушать эфир, мы приЄмный модуль
}
void loop() {
	while (true)
	{
		gyro.update();

		motorFR.writeMicroseconds(power[0]);
		motorFL.writeMicroseconds(power[1]);
		motorRR.writeMicroseconds(power[2]);
		motorRL.writeMicroseconds(power[3]);

		if (radio.available(&pipeNo)) {
			radio.read(&data_upr, sizeof(data_upr));
			gotByte = volt.update() * 10;
			radio.writeAckPayload(pipeNo, &gotByte, 1);	// отправл€ем обратно то что прин€ли
#ifdef DEBUG
			delay(100);
			Serial.print("DATA: ");
			Serial.println(data_upr[1]);
			Serial.println("power[1]: ");
			Serial.println(power[1]);
#endif // DEBUG
			calculate(power, data_upr);
		}

		led.update();
	}
}