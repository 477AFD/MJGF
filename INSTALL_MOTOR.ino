// Version a1.0
#define dat1 A0
#define dat2 A1
#define dat3 A2
#define dat4 A3
#define dat5 A4
#define dat6 A5

#include <AFMotor.h>
#include <Servo.h>

Servo directional;
Servo turner;

AF_DCMotor motorLB(1, MOTOR12_1KHZ);
AF_DCMotor motorRB(2, MOTOR12_1KHZ);
AF_DCMotor motorLT(3, MOTOR12_1KHZ);
AF_DCMotor motorRT(4, MOTOR12_1KHZ);

void setup() {
pinMode(dat1, INPUT); //Data (directional sensor)
pinMode(dat2, INPUT); //Data (special)
pinMode(dat3, INPUT); //Data (Bit 0)
pinMode(dat4, INPUT); //Data (Bit 1)
pinMode(dat5, INPUT); //Data (Bit 2)
pinMode(dat6, INPUT); //Data (Bit 3)
pinMode(0, OUTPUT); // RX pin (12V)
pinMode(1, OUTPUT); // TX pin (VACUUM)
pinMode(2, INPUT); // RESPONSE
pinMode(13, OUTPUT); // COUNTER-RESPONSE
directional.attach(9);
turner.attach(10);
digitalWrite(0, HIGH);
delay(3000);
digitalWrite(1, HIGH);
while (digitalRead(2) == LOW) {
  delayMicroseconds(10);
}
digitalWrite(13, HIGH);
delay(500);
digitalWrite(13, LOW);
}

void directionalMove(int degrees) {
  int servo_RawDegrees = map(degrees, -90, 90, 0, 180);
  directional.write(servo_RawDegrees);  
}

void turnGear(int direction) {
  switch (direction) {
    case -1:
      turner.write(0);
      break;
    case 0:
      turner.write(90);
      break;
    case 1:
      turner.write(180);
      break;
  }
}

void moveUnit(int directionMT, int speed) {
int speedrw;
  if (speed <= 0) { // it can cause an issue when speed is 0, because X / 0 and negative is not defined/tolerated
    speedrw = 255;
  } else {
    speedrw = 255 / (speed); // the higher the value, the lower the speed
  }
  motorLB.setSpeed(speedrw);
  motorLT.setSpeed(speedrw);
  motorRB.setSpeed(speedrw);
  motorRT.setSpeed(speedrw);
  switch (directionMT) {
    case -1:
      motorLB.run(BACKWARD);
      motorLT.run(BACKWARD);
      motorLB.run(BACKWARD);
      motorLB.run(BACKWARD);
      break;
    case 0:
      motorLB.run(RELEASE);
      motorLT.run(RELEASE);
      motorLB.run(RELEASE);
      motorLB.run(RELEASE);
      break;
    case 1:
      motorLB.run(FORWARD);
      motorLT.run(FORWARD);
      motorLB.run(FORWARD);
      motorLB.run(FORWARD);
      break;         
  }
}

void sendCT() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
}

int receiveData() {
  // Data 0-5 (000000), wherein last bit is data 0, first bit is data 5 (6-bit value)
  int data0 = digitalRead(dat1);
  int data1 = digitalRead(dat2);
  int data2 = digitalRead(dat3);
  int data3 = digitalRead(dat4);
  int data4 = digitalRead(dat5);
  int data5 = digitalRead(dat6);
  return ((data0 * 1) + (data1 * 2) + (data2 * 4) + (data3 * 8) + (data4 * 16) + (data5 * 32)); // this can output from 0 to 64
}

int resultData;
int frame = -1;


void loop() {
  frame = frame + 1;
  while (digitalRead(2) == LOW) {
    resultData = receiveData();
    delay(500);
  } 
  switch (resultData) {
    case 0:
      moveUnit(0, 1);
      sendCT();
      break;
    case 4:
      moveUnit(1, 4);
      sendCT();
      break;
    case 5:
      moveUnit(1, 3);
      sendCT();
      break;
    case 6:
      moveUnit(1, 2);
      sendCT();
      break;
    case 7:
      moveUnit(1, 1);
      sendCT();
      break;
    case 8:
      moveUnit(-1, 4);
      sendCT();
      break;
    case 9:
      moveUnit(-1, 3);
      sendCT();
      break;
    case 10:
      moveUnit(-1, 2);
      sendCT();
      break;    
    case 11:
      moveUnit(-1, 1);
      sendCT();
      break;
    case 16:
      turnGear(-1);
      sendCT();
      break;
    case 20:
      turnGear(1);
      sendCT();
      break;
    case 28:
      turnGear(0);
      sendCT();
      break;
    case 32:
      directionalMove(-90);
      sendCT();
      break;
    case 33:
      directionalMove(0);
      sendCT();
      break;
    case 34:
      sendCT();
      break;
    case 35:
      sendCT();
      break;
    case 36:
      directionalMove(90);
      sendCT();
      break;
    case 37:
      sendCT();
      break;
    case 38:
      sendCT();
      break;
    case 39:
      sendCT();
      break;
    default:
      moveUnit(0, 1);
      sendCT();
      break;
  }
}
