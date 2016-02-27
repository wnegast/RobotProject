#include "dataLogger.h"
#include <arduino.h>

void printFrontSonar(unsigned long f) {
  Serial.print("Front Sonar: ");
  Serial.println(f);
}


