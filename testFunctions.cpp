#include "testFunctions.h"
#include "Navigation.h"

#include <Arduino.h>
void testSonar() {
  unsigned long f = sonar(SonarFront);
  unsigned long l = sonar(SonarLeft);
  unsigned long r = sonar(SonarRight);
  unsigned long b = sonar(SonarBack);
  Serial.print("Front: ");
  Serial.print(f);
  Serial.println();
  Serial.print("Left: ");
  Serial.print(l);
  Serial.println();
  Serial.print("Right: ");
  Serial.print(r);
  Serial.println();
  Serial.print("Back: ");
  Serial.print(b);
  Serial.println();
  Serial.println();
  delay(5000);
}

