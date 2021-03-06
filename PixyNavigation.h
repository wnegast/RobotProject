#include <SPI.h>
#include <Pixy.h>
#include <stdio.h>
#define PIXY_MAX_X 320
#define PIXY_MAX_Y 200
#define DIST_CONST 20269
#define MIN_DIST 1000
#define MIN_TOLERANCE 25
#define MAX_SPEED 25
#define HOR_OFFSET 4
#define ROT_TOL 30
#define SONAR_1 22
#define SONAR_2 23
#define horRelay 48
#define vertRelay 49


//Layouts
#define LAYOUT_A 0
#define LAYOUT_B 1

//Zones
#define ZONE_A 0
#define ZONE_B 1
#define ZONE_C 2

//Colors
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define TAN 5

#define FAR 0
#define MED 1
#define CLOSE 2

#define GRIPPER_PIN 9

void SetupPixy();
void CheckPixy();

void MoveForward();
void PickupBlock();
void FindDestination();
void CheckHeight(int distance);
void CenterRobot();
int CheckDistance();
void setupSteppers();
void CenterClaw();
void RaiseClaw();
void test();
void CloseGripper();
void OpenGripper();

//Functions for finding the correct block containers
void ZoneA();
void ZoneB();
void ZoneC();

//Functions for releasing blocks
void PutInTruck();
void PutOnBoat();

bool FindBlockByColor(int _col);
