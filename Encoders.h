#ifndef Encoders_H
#define Encoders_H

#include <Arduino.h>
#include "Navigation.h"

void setupEncoders();

#define DirForward 1
#define DirBackward 2
#define DirLeft 3
#define DirRight 4
#define DirRotateLeft 5
#define DirRotateRight 6

void driveOn(int, int, int, int);
void driveOff();
void resetEncoderTicks();
void syncMotorsForward();
void syncMotorsBackward();
void syncMotorsLeft();
void syncMotorsRight();
void syncMotorsRotateLeft();
void syncMotorsRotateRight();
void HandleFLeftMotorInterruptA();
void HandleFRightMotorInterruptA();
void HandleBLeftMotorInterruptA();
void HandleBRightMotorInterruptA();

#endif
