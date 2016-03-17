#include "Sonar.h"

Sonar::Sonar(int trig)
{
  timer = 0;
  trigPin = trig;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
}

int Sonar::GetDistance()
{
  if( ((millis() - timer)) >= 60UL)
  {
    checkSonar();
  }
  return distance;
}

void Sonar::checkSonar()
{
  long duration;

  pinMode(trigPin, OUTPUT);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);

  pinMode(trigPin, INPUT);
  duration = pulseIn(trigPin, HIGH);
  timer = millis();
  distance = duration;
}

