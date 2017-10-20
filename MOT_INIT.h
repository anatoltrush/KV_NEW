#ifndef MOT_INIT_H
#define MOT_INIT_H

#include <Servo.h>
#include "DIRECT.h"

#define DEBUG

#define FR_pin_out 4
#define FL_pin_out 5
#define RR_pin_out 3
#define RL_pin_out 2

void moto_init(Servo &motorFR, Servo &motorFL, Servo &motorRR, Servo &motorRL) {
  pinMode(FR_pin_out, OUTPUT);
  pinMode(FL_pin_out, OUTPUT);
  pinMode(RR_pin_out, OUTPUT);
  pinMode(RL_pin_out, OUTPUT);

  motorFR.attach(FR_pin_out);
  motorFL.attach(FL_pin_out);
  motorRR.attach(RR_pin_out);
  motorRL.attach(RL_pin_out);

  motorFR.writeMicroseconds(MAX_POWER);
  motorFL.writeMicroseconds(MAX_POWER);
  motorRR.writeMicroseconds(MAX_POWER);
  motorRL.writeMicroseconds(MAX_POWER);
  delay(2000);
  motorFR.writeMicroseconds(MIN_POWER);
  motorFL.writeMicroseconds(MIN_POWER);
  motorRR.writeMicroseconds(MIN_POWER);
  motorRL.writeMicroseconds(MIN_POWER);
  delay(2000);
}
#endif // MOT_INIT_H
