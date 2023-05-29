#include <AFMotor.h> // AF_Motor library
#include <NewPing.h> // NewPing library
#include <Servo.h> // Servo library

#define TRIG_PIN A0
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 255 // sets speed of DC motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   
Servo turnservo;
bool goesForward=false;
int distance = 100;
int speedSet = 0;
bool power = false;
bool powerRaw = false;

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void setup() {
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, INPUT_PULLUP);
digitalWrite(A2, HIGH);
digitalWrite(A3, HIGH);
myservo.attach(10);  
turnservo.attach(9);
  myservo.write(15);
  delay(1000);
  myservo.write(165);
  delay(1000);
myservo.write(90);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
turnservo.write(90); 
//digitalWrite(A2, LOW);
delay(3000);
}
int isTouched = 0;
void loop() {
 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 if (digitalRead(A5) == LOW) {
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
 if(distance<=15)
 {
  moveStop();
  delay(105);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200); /*
  moveBackward();
  delay(900);
  moveStop();
  delay(205); */

  if(distanceR<=distanceL)
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
  moveForward();
 }
 distance = readPing();
 } else {
   digitalWrite(A2, HIGH);
   digitalWrite(A3, HIGH);
 }
}

int lookRight()
{
    myservo.write(15); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(90); 
    return distance;
}

int lookLeft()
{
    myservo.write(165); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(90); 
    return distance;
    delay(100);
}



void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}

void moveBackward() {
    goesForward=false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

int turnTime = 931;

void turnRight() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(turnTime);
  moveStop();
  turnservo.write(70);
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(turnTime);
  turnservo.write(90);  
  moveStop();
} 
 
void turnLeft() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(turnTime);
  moveStop();
  turnservo.write(130);
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(turnTime);
  turnservo.write(90);  
  moveStop();
}  
