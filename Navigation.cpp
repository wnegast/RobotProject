#include <Servo.h>
#include "Navigation.h"
#include <Arduino.h>

Servo FR;
Servo FL;
Servo BR;
Servo BL;

void setupMotors() {
  FR.attach(10, 1000, 2000); 
  FL.attach(11, 1000, 2000);
  BR.attach(13, 1000, 2000);
  BL.attach(12, 1000, 2000);
  FR.write(90);     //  x - x
  FL.write(90);     //  - - -8
  BR.write(90);     //  x - x
  BL.write(90);
}

void driveTest(){
    FR.write(115);
    delay(1225);
    FR.write(90); 
    delay(1000);
    FL.write(115);
    delay(1225);
      FL.write(90);
      delay(1000);
    BR.write(115);
    delay(1225);
      BR.write(90); 
      delay(1000);
    BL.write(115);
    delay(1225);
    BL.write(90);
}
void driveStop() {
  FR.write(90);     //  x - x
  FL.write(90);     //  - - -
  BR.write(90);     //  x - x
  BL.write(90);
}

void driveFL(int speeed)
{
  FL.write(speeed);
}

void driveFR(int speeed)
{
  FR.write(speeed);
}

void driveBL(int speeed)
{
  BL.write(speeed);
}

void driveBR(int speeed)
{
  BR.write(speeed);
}

void driveForward(int speeed) {
  
  //FR.write(180);    //  F - F
  //FL.write(180);    //  - - -
  //BR.write(180);    //  F - F
  //BL.write(180);

  for(int i = 90; i <= speeed; i++)
  {
    FR.write(i);
    FL.write(i);
    BR.write(i);
    BL.write(i);
  }
  
  
}

void driveBack() {
  FR.write(0);      //  B - B
  FL.write(0);      //  - - -
  BR.write(0);      //  B - B
  BL.write(0);
}

void driveSlideLeft() {
  FR.write(180);    //  B - F
  FL.write(0);      //  - - -
  BR.write(0);      //  F - B
  BL.write(180);
}

void driveSlideRight() {
  FR.write(0);      //  F - B
  FL.write(180);    //  - - -
  BR.write(180);    //  B - F
  BL.write(0);
}

void driveForwardLeft() {
  FR.write(180);    //  x - F  
  FL.write(90);     //  - - -
  BR.write(90);     //  F - x
  BL.write(180);
}

void driveForwardRight() {
  FR.write(90);     //  F - x
  FL.write(180);    //  - - -
  BR.write(180);    //  x - F
  BL.write(90);
}

void driveBackLeft() {
  FR.write(90);     //  B - x
  FL.write(0);      //  - - -
  BR.write(0);      //  x - B
  BL.write(90);
}

void driveBackRight() {
  FR.write(0);      //  x - B
  FL.write(90);     //  - - -
  BR.write(90);     //  B - x
  BL.write(0);
}

void driveRotateClockwise() {
  FR.write(0);      //  F - B
  FL.write(135);    //  - - -
  BR.write(0);      //  F - B
  BL.write(135);
}

void driveRotateCounterClockwise() {
  FR.write(135);    //  B - F
  FL.write(0);      //  - - -
  BR.write(135);    //  B - F
  BL.write(0);
}

void driveForwardTurnLeft() {
  FR.write(180);    //  f - F  
  FL.write(135);    //  - - -
  BR.write(180);    //  f - F
  BL.write(135);
}

void driveForwardTurnRight() {
  FR.write(135);    //  F - f
  FL.write(180);    //  - - -
  BR.write(135);    //  F - f
  BL.write(180);
}

void driveBackTurnLeft() {
  FR.write(45);    //  B - b  
  FL.write(0);     //  - - -
  BR.write(45);    //  B - b
  BL.write(0);
}

void driveBackTurnRight() {
  FR.write(0);    //  b - B  
  FL.write(45);   //  - - -
  BR.write(0);    //  b - B
  BL.write(45);
}

void setupSonar() {
  pinMode(SonarFront, OUTPUT);
  digitalWrite(SonarFront, LOW);
  pinMode(frontEcho, INPUT);
  pinMode(SonarLeft, OUTPUT);
  digitalWrite(SonarLeft, LOW);
  pinMode(leftEcho, INPUT);
  pinMode(SonarRight, OUTPUT);
  digitalWrite(SonarRight, LOW);
  pinMode(rightEcho, INPUT);
  pinMode(SonarBack, OUTPUT);
  digitalWrite(SonarBack, LOW);
  pinMode(backEcho, INPUT);
}
unsigned long timer = 0;

unsigned long sonar(int i) {
  unsigned long distance = 0;
  unsigned long duration = 0;
  while (millis() - timer < 20) delay(1);
  timer = 0;
  digitalWrite(i, HIGH);
  delay(10);
  digitalWrite(i, LOW);
  duration = pulseIn((i+1), HIGH, MAX_DISTANCE);
  timer = millis();
  distance = duration / 58.2;
  return (distance);
}
