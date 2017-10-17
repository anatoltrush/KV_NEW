#include <Servo.h>
#include "MOT_INIT.h"
#include "FLASH.h"
#include "DIRECT.h"

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define SIZE_OF_DATA 6

Flasher led(13, 1000, 1000);

Servo motorFR;
Servo motorFL;
Servo motorRR;
Servo motorRL;

uint8_t data_upr[SIZE_OF_DATA] = { 0, 0, 0, 0 , 0 , 0 }; // for test
uint16_t power[4] = { 870, 870, 870, 870};

RF24 radio(9, 10); // "�������" ������ �� ����� 9 � 10
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" }; //��������� ������ ����

void setup() {
	Serial.begin(9600);

  moto_init(motorFR, motorFL, motorRR, motorRL);

  radio.begin(); //������������ ������
  radio.setAutoAck(1);         //����� ������������� �����, 1 ��� 0 ����
  radio.setRetries(0, 15);     //(����� ����� �������� �����������, ����� �������)
  radio.enableAckPayload();    //��������� ������� ������ � ����� �� �������� ������
  radio.setPayloadSize(32);     //������ ������, � ������

  radio.openReadingPipe(1, address[0]);      //����� ������� ����� 0
  radio.setChannel(0x60);  //�������� ����� (� ������� ��� �����!)

  radio.setPALevel(RF24_PA_MAX); //������� �������� �����������. �� ����� RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(RF24_250KBPS); //�������� ������. �� ����� RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
								   //������ ���� ��������� �� �������� � �����������!
								   //��� ����� ������ �������� ����� ����� ������� ���������������� � ���������!!
  radio.powerUp(); //������ ������
  radio.startListening();  //�������� ������� ����, �� ������� ������
}
void loop() {
	motorFR.writeMicroseconds(power[0]);
	motorFL.writeMicroseconds(power[1]);
	motorRR.writeMicroseconds(power[2]);
	motorRL.writeMicroseconds(power[3]);
	byte pipeNo;
	//while (radio.available(&pipeNo)) {
		radio.read(&data_upr, sizeof(data_upr));
		Serial.print("DATA: ");
		Serial.println(data_upr[1]);
		Serial.println("power[1]: ");		
		Serial.println(power[1]);
		delay(50);
	//}
  change(power, data_upr);

  led.update();
}
