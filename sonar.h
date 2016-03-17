#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>

class Sonar
{
public:
  Sonar(int);
  int GetDistance();
private:
  unsigned long timer;
  int trigPin;
  int echoPin;
  int distance;
  void checkSonar();
};

#endif
