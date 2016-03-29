#include "Sonar.h"

volatile unsigned long stimer;
volatile unsigned long start1;
volatile unsigned long start2;
volatile unsigned long dist1;
volatile unsigned long dist2;
volatile boolean bolInterrupt1;
volatile boolean bolInterrupt2;
int trigPin1;
int trigPin2;

void SetupSonar(int trig1, int trig2)
{
  stimer = 0;
  start1 = 0;
  start2 = 0;
  trigPin1 = trig1;
  trigPin2 = trig2;
  dist1 = 9999;
  dist2 = 9999;
  bolInterrupt1 = false;
  bolInterrupt2 = false;
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
}

void GetDistance(unsigned long& sonar1, unsigned long& sonar2)
{
  if( ((millis() - stimer)) >= 50UL)
  {
    checkSonar();
  }
  //Serial.print("Sonar ");
  //Serial.println(distance);
  sonar1 = dist1;
  sonar2 = dist2;
}


void checkSonar()
{
  unsigned long duration1;
  unsigned long duration2;
  trigger(trigPin1);
  duration1 = pulseIn(trigPin1, HIGH);
  delay(25);
  trigger(trigPin2);
  duration2 = pulseIn(trigPin2, HIGH);
  
  stimer = millis();
  if(duration1 >= 0)
  {
    dist1 = duration1;
  }

  if(duration2 >= 0)
  {
    dist2 = duration2;
  }
  
}


void trigger(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
  digitalWrite(pin, HIGH);
  delayMicroseconds(15);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  delayMicroseconds(10);
}



