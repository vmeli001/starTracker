/* Stepper Unipolar Advanced
 * -------------------------
 *
 * Program to drive a stepper motor coming from a 5'25 disk drive
 * according to the documentation I found, this stepper: "[...] motor 
 * made by Copal Electronics, with 1.8 degrees per step and 96 ohms 
 * per winding, with center taps brought out to separate leads [...]"
 * [http://www.cs.uiowa.edu/~jones/step/example.html]
 *
 * It is a unipolar stepper motor with 5 wires:
 * 
 * - red: power connector, I have it at 5V and works fine
 * - orange and black: coil 1
 * - brown and yellow: coil 2
 *
 * (cleft) 2005 DojoDave for K3
 * http://www.0j0.org | http://arduino.berlios.de
 *
 * @author: David Cuartielles
 * @date: 20 Oct. 2005
 */
#include <TimeLib.h>
#include <TimeAlarms.h>

AlarmId id;

int motorPins[] = {
  8,9, 10, 11};
int limitPin = 12;
int count = 0;
int count2 = 0;
int delayTime = 365;
int val = 0;
int incomingByte = 0;
boolean Step = true;
boolean newStep = true;



void setup() {
  for (count = 0; count < 4; count++) {
    pinMode(motorPins[count], OUTPUT);
    digitalWrite(motorPins[count],HIGH);
  }
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(limitPin,INPUT);
  //pinMode(4,INPUT);
  //Serial.begin(115200);

}

void startTrack(){

  moveForward();
  //delay is set so that the rotationmatches the movement of stars
  // with a 10-32 drive screw one revolution per minute tracks the star
  // with a 10-24 75% of that tracks the stars
  delay(delayTime);
}



void moveForward() {

  if ((count2 == 0) || (count2 == 1)) {
    count2 = 16;
  }

  count2>>=1;

  for (count = 3; count >= 0; count--) {
    digitalWrite(motorPins[count], !(count2>>count&0x01));
    //Serial.print(motorPins[count]);
    //Serial.print(" , ");
    //Serial.println(count2>>count&0x01);
  }
  //Serial.println("***");
  //delay(10);


}

void moveBackward() {
  if(digitalRead(limitPin)){
    if ((count2 == 0) || (count2 == 1)) {
      count2 = 16;
    }
    count2>>=1;
    for (count = 3; count >= 0; count--) {
      digitalWrite(motorPins[3 - count], !(count2>>count&0x01));
    }
    //delay(10);
  }
  else
    motorOFF();

}

void fastForward() {
  moveForward();
  delay(10);
}

void fastBackward() {
  moveBackward();
  delay(10);
}

void calibration() {
  for(int n = 0; n <100; n++){
    moveForward();
    delay(10);
  }
}

void adjustDelay() {
  delayTime = delayTime + 10;
  delay (1000);
}

void motorOFF() {
  for (int n = 0; n < 4; n++) {
    //pinMode(motorPins[count], OUTPUT);
    digitalWrite(motorPins[n],HIGH);
  }
}

void loop() {

  if (digitalRead(3)){
    startTrack();

  }
  else if(digitalRead(4)){
    fastBackward();
  }
  else if(digitalRead(5)){
    fastForward();
  }
  // If the rate needs to be adjusted set pin 6 (Slower) or 7 (Faster) high
  //  The program will wait one second before resuming normal operation.

  else if(!digitalRead(6)){
    //calibration();
    adjustDelay();
  }
  else
    motorOFF();
}

