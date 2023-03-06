// MJGF Alpha 1.0 (untested)
#define pindL A2
#define pindR A3
#define pindF A1
#define pindE 1
#define pindT 2
#define pindED 3
#define pindTD 4
#define dat1 5
#define dat2 6
#define dat3 7
#define dat4 8
#define dat5 9
#define dat6 10

void setup() {
  // put your setup code here, to run once:  
pinMode(A0, OUTPUT); //5V
pinMode(0, OUTPUT);
digitalWrite(A0, HIGH);
digitalWrite(0, HIGH);
pinMode(pindL, INPUT); //IR left
pinMode(pindR, INPUT); //IR right
pinMode(pindF, INPUT); //IR forward
pinMode(pindE, INPUT); //ECHO front
pinMode(pindT, OUTPUT); //TRIG front
pinMode(pindED, INPUT); //ECHO directional
pinMode(pindTD, OUTPUT); //TRIG directional
pinMode(dat1, OUTPUT); //Data (directional sensor)
pinMode(dat2, OUTPUT); //Data (special)
pinMode(dat3, OUTPUT); //Data (Bit 0)
pinMode(dat4, OUTPUT); //Data (Bit 1)
pinMode(dat5, OUTPUT); //Data (Bit 2)
pinMode(dat6, OUTPUT); //Data (Bit 3)
clearDataPins(); // Make sure everything between 5 to 10 is set to LOW
pinMode(11, OUTPUT); //Response Pin
pinMode(12, INPUT); //Counter-response Pin
pinMode(A4, INPUT);
respondInstructionForced(); // make sure that the motor board is ready
moveUnit(0, 2, false);
}
float distU, distD; //Duration in microseconds, distance calculated variable

float distanceWall(bool IR, bool DIR) {
if (IR == false) {
  if (DIR) {
  digitalWrite(pindTD, HIGH);
  delayMicroseconds(10);
  digitalWrite(pindTD, LOW);

  // measure duration of pulse from ECHO pin
  distU = pulseIn(pindED, HIGH);
} else {
  digitalWrite(pindT, HIGH);
  delayMicroseconds(10);
  digitalWrite(pindT, LOW);

  // measure duration of pulse from ECHO pin
  distU = pulseIn(pindE, HIGH);  
}

  // calculate the distance
  distD = 0.017 * distU;
  return distD;
} else {
    if (DIR) {
      int isLeft = digitalRead(pindL);
      int isRight = digitalRead(pindR);
      if (isLeft > isRight) {
        return 0;
      } else {
        return 1;
      }
    } else {
      if (digitalRead(pindF) == HIGH)
      return 1;
    }
}
}

void respondInstruction() { // Initiate a response
  digitalWrite(11, LOW);
  digitalWrite(11, HIGH);
  if (digitalRead(12) == HIGH) { // If counter-response is received,
    digitalWrite(11, LOW); // immediately stop
  } else { // else,
    delay(1000); // wait 1 second, and
    digitalWrite(11, LOW); // stop.
  }
}

void respondInstructionForced() { // Initiate a response
  digitalWrite(11, LOW);
  digitalWrite(11, HIGH);
  while (digitalRead(12) == LOW) { // If counter-response is received,
    digitalWrite(11, HIGH); // immediately stop
  }
  digitalWrite(11, LOW);
}

void clearDataPins() { // Clear all data from datX
digitalWrite(dat1, LOW);
digitalWrite(dat2, LOW);
digitalWrite(dat3, LOW);
digitalWrite(dat4, LOW);
digitalWrite(dat5, LOW);
digitalWrite(dat6, LOW);  
}

void dataWrite(bool bin1, bool bin2, bool bin3, bool bin4, bool bin5, bool bin6) {
  clearDataPins();
  if (bin1) {
    digitalWrite(dat1, HIGH);    
  } else {
    digitalWrite(dat1, LOW);
  }
  if (bin2) {
    digitalWrite(dat2, HIGH);    
  } else {
    digitalWrite(dat2, LOW);
  }
  if (bin3) {
    digitalWrite(dat3, HIGH);    
  } else {
    digitalWrite(dat3, LOW);
  }
  if (bin4) {
    digitalWrite(dat4, HIGH);    
  } else {
    digitalWrite(dat4, LOW);
  }
  if (bin5) {
    digitalWrite(dat5, HIGH);    
  } else {
    digitalWrite(dat5, LOW);
  }
  if (bin6) {
    digitalWrite(dat6, HIGH);    
  } else {
    digitalWrite(dat6, LOW);
  }
}

void directionalSend(bool direction, bool returnPos) {
  // Send directional distance receptor instruction (dat1-6) (100X0X)
digitalWrite(dat1, HIGH);
digitalWrite(dat2, LOW);
digitalWrite(dat3, LOW);
if (direction) { // Direction of unit (FALSE = left, return; TRUE = right)
  digitalWrite(dat4, LOW);
} else {
  digitalWrite(dat4, HIGH);
}
digitalWrite(dat5, LOW);
if (returnPos) { // Return to 0 degrees
digitalWrite(dat6, HIGH);
} else {
digitalWrite(dat6, LOW);  
}
delayMicroseconds(602);
respondInstruction();
delay(1500);
clearDataPins();
}

void mvmtExt(int speed) {
    bool speed1 = false;
    bool speed2 = false;
    bool isBackward = speed < 0;
    bool isForward = speed > 0;
    if (speed == 1 | speed == -1) {
      speed1 = false;
      speed2 = false;  
    }
    if (speed == 2 | speed == -2) {
      speed1 = true;
      speed2 = false;    
    }
    if (speed == 3 | speed == -3) {
      speed1 = false;
      speed2 = true;    
    }
    if (speed == 4 | speed == -4) {
      speed1 = true;
      speed2 = true;    
    }
    dataWrite(false, false, isBackward, isForward, speed2, speed1);
}

void moveUnit(int direction, int speedRaw, bool willMove) { // Direction: -1 = left, 0 = forward, 1 = right; Speed: negative 4 to positive 4, willMove: ignored if direction = 0
    // Move unit (dat1-6) (0XXXXX)
    // Move forward: 0001XX
    // Move backward: 0010XX
    // Stop: 0011XX, 0000XX
    // Go left: 010000
    // Go right: 010100
    // Go forward: 011100  
    // Speed (min-max): 0XXX00, 0XXX01, 0XXX10, 0XXX11
  if (speedRaw == 0) {
    dataWrite(false, false, true, true, false, false);
  } else {
    if (direction == -1) {
      dataWrite(false, true, false, false, false, false);
      if (willMove) {
        mvmtExt(speedRaw);
      }
    } else if (direction == 1) {
      dataWrite(false, true, false, true, false, false);
      if (willMove) {
        mvmtExt(speedRaw);
      }
    } else if (direction == 0) {
      dataWrite(false, true, true, true, false, false);
      mvmtExt(speedRaw);
    }
    delay(500);
    if (digitalRead(12) == LOW) { // The system must receive the counterresponse. If not, wait extra 1.5 seconds.
    delay(1500);
    }
    clearDataPins();
  }
  respondInstruction();
    
  
}
int rge;
int senseWall(bool RANGE) {
    // Used only in directional sensor circuit, returns -1 if left, 1 if right or 0 if not
    if (RANGE) {
      rge = 21;
    } else {
      rge = 7;
    }
    directionalSend(false, false);
    delay(750);   
    float dist_cl = distanceWall(false, true);
    delay(750);
    directionalSend(true, false);
    delay(1500);   
    float dist_cr = distanceWall(false, true);
    delay(750);
    directionalSend(false, true);
    if (dist_cl <= rge && dist_cl < dist_cr) { // Definitely wall is left
      return -1;
    } else if (dist_cr <= rge && dist_cr < dist_cl) { // Definitely wall is right
      return 1;
    } else {
      return 0;
    }
}

void loop() {
  while (distanceWall(false, false) > 3) {
    clearDataPins();
    respondInstruction();
  }
  moveUnit(0, 0, false);
  int wallDir = senseWall(digitalRead(A4) == HIGH);
  if (wallDir == -1) {
    moveUnit(0, -4, false);
    delay(750);
    moveUnit(0, 0, false);
    moveUnit(1, 4, true) ;         
  }  
  if (wallDir == 1) {
    moveUnit(0, -4, false);
    delay(750);
    moveUnit(0, 0, false);
    moveUnit(-1, 4, true);         
  }
}
