#include <AFMotor.h>
#include <Servo.h>

// Define Servos
Servo directional;
Servo turner;

int bit_analog(uint8_t AX) {
  return map(analogRead(AX), 0, 1023, 0, 255);
}
AF_DCMotor motorA(1, MOTOR12_64KHZ);
AF_DCMotor motorB(2, MOTOR12_64KHZ);

void moveUnit(int speed, int d1, int d2) {
motorA.setSpeed(speed);
motorB.setSpeed(speed);
int direction = (d1 * 2) + (d2 * 1);
if (direction == 3 | direction == 0) {
  motorA.run(RELEASE);
} else {
  switch (direction) {
    case 1:
      motorA.run(FORWARD);
      break;
    case 2:
      motorA.run(BACKWARD);
      break;
  }
}
}

void setup() {
pinMode(13, OUTPUT);
  /* those are not used because of static
pinMode(12, INPUT);
pinMode(A4, INPUT);
pinMode(A3, INPUT);
pinMode(A1, INPUT);
pinMode(A0, INPUT); */
pinMode(0, OUTPUT);
pinMode(1, OUTPUT);
// Calibrate the sensor's servo
directional.attach(9);
turner.attach(10);
directional.write(0);
delay(1000);
directional.write(180);
delay(1000);
directional.write(90);
turner.write(66);
digitalWrite(0, LOW);
delay(1000);
turner.write(113);
delay(1000);
turner.write(90);
digitalWrite(1, LOW);

// Signal the main Arduino, telling that it is ready
digitalWrite(13, digitalRead(0));
}

void turnUnit(int t1, int t2) {
  int data = (t2 * 1) + (t1 * 2);
  switch (data) {
    case 1:
      turner.write(66);
      break;
    case 2:
      turner.write(113);
      break;
    default:
      turner.write(90);
      break;  
  }
}

void loop() {
  moveUnit(analogRead(A5), digitalRead(A4), digitalRead(A3));
  directional.write(bit_analog(A2));
  turnUnit(digitalRead(A1), digitalRead(A0));
}
