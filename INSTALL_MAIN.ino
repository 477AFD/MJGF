
#include <NewPing.h>

#define TRIG_PIN 1
#define ECHO_PIN 0
#define MAX_DISTANCE 200 
#define MAX_SPEED 190 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

bool goesForward=false;
int distance = 100;
int speedSet = 0;

void turnRight() {
// 6 is A0
  moveStop();
  digitalWrite(5, HIGH);
  delay(1000);
  moveForward(false);
  delay(500);
  moveStop();
  digitalWrite(5, LOW);
  delay(1000);
  moveForward(false);      
} 
 
void turnLeft() {
  moveStop();
  digitalWrite(6, HIGH);
  delay(1000);
  moveForward(false);
  delay(500);
  moveStop();
  digitalWrite(6, LOW);
  delay(1000);
  moveForward(false); 
}  

void setup() {
  // Anything not defined in pinMode() is considered input.
  pinMode(2, OUTPUT); // connect to A4
  pinMode(4, OUTPUT); // connect to A3
  pinMode(5, OUTPUT); // connect to A1
  pinMode(6, OUTPUT); // connect to A0
  pinMode(13, OUTPUT); // on-boot response pin
/*
Inputs & analog outputs:
RX - ECHO pin
TX - TRIG pin
3 - analog for directional sensor
7 - IR input
8 - No connection
9 - analog for speed of unit, connect to A5
12 - on-boot counter-response pin
*/  
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  while (digitalRead(12) == LOW) {
  digitalWrite(13, HIGH);
  delayMicroseconds(5);
  digitalWrite(13, LOW);
  }
digitalWrite(13, LOW);  
}

void analogOut(int bitval, uint8_t pin) {
  analogWrite(pin, map(bitval, 0, 255, 0, 1023));
}

void loop() {
 int distanceR = 0;
 int distanceL = 0;
 delay(40);
 
 if(digitalRead(7) == HIGH)
 {
  moveStop();
  delay(100);
  moveBackward(true) ;
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward(true);
 }
 // distance = readPing();
}

int lookRight()
{
    analogOut(0, 3); 
    delay(500);
    int distance = readPing();
    delay(100);
    analogOut(127, 3); 
    return distance;
}

int lookLeft()
{
    analogOut(255, 3);
     
    delay(500);
    int distance = readPing();
    delay(100);
    analogOut(127, 3); 
    return distance;
    delay(100);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  digitalWrite(4, LOW);
  digitalWrite(2, HIGH);
  analogOut(0, 9);
  } 
  
void moveForward(bool load) {
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
 if(!goesForward)
  {
    goesForward=true;
    digitalWrite(4, LOW);
    digitalWrite(2, HIGH);  
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    analogOut(speedSet, 9);
    delay(5);
   }
  }
}

void moveBackward(bool load) {
    goesForward=false;
    digitalWrite(4, HIGH);
    digitalWrite(2, LOW);
if (load) {
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    analogOut(speedSet, 9);
    delay(5);
  } 
  } else {
    analogOut(MAX_SPEED, 9);
  }
}
  


