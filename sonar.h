#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>


  void SetupSonar(int, int);
  void GetDistance(unsigned long& sonar1, unsigned long& sonar2);
  void checkSonar();
  void trigger(int);
  

#endif
