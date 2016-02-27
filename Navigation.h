#ifndef NAVIGATION_H
#define NAVIGATION_H
#define MAX_DISTANCE 20000

//Drive commands
void setupMotors();
void driveTest();
void driveStop();
void driveForward(int);
void driveBack();
void driveSlideLeft();
void driveSlideRight();
void driveForwardLeft();
void driveForwardRight();
void driveBackLeft();
void driveBackRight();
void driveRotateClockwise();
void driveRotateCounterClockwise();
void driveForwardTurnLeft();
void driveForwardTurnRight();
void driveBackTurnLeft();
void driveBackTurnRight();
void driveFL(int);
void driveFR(int);
void driveBL(int);
void driveBR(int);

//Sonar
#define SonarFront 30
#define frontEcho 31
#define SonarLeft 34
#define leftEcho 35
#define SonarRight 32
#define rightEcho 33
#define SonarBack 36
#define backEcho 37

void setupSonar();
unsigned long sonar(int i);

#endif
