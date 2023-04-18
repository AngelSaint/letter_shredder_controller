#include <Servo.h>

Servo right;

int R_pin = 6; //J5

void setup() {
right.attach(R_pin);

right.write(180);
}

void loop() {
  right.write(45);
  delay(1000);
  right.write(135);
  delay(5000);
}