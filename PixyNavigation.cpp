
#include "PixyNavigation.h"
#include "Encoders.h"
#include "Sonar.h"
#include <Stepper.h>
#include <Servo.h>
// This is the main Pixy object

Pixy pixy;

int16_t blocksig;
bool haveBlock;
int zone;
int layout;
int state;
Servo gripper;

int vertStepCount; // number of steps the motor has taken
int horStepCount; // number of steps the motor has taken

const int stepsPerRevolution = 200;

Stepper vertStepper(stepsPerRevolution, 34, 35, 36, 37);

//Setup Horizontal Stepper Motor
// initialize the stepper library on pins 30 through 33:
Stepper horStepper(stepsPerRevolution, 30, 31, 32, 33);

int color;

//This will hold the function that releases the block into the right container
void (*ptrBlockRelease)();

void SetupPixy()
{
  Serial.begin(9600);
  haveBlock = false;
  blocksig = -1;
  pixy.init();
  setupSteppers();
  SetupSonar(SONAR_1, SONAR_2);
  vertStepCount = 0;
  state = FAR;

  gripper.attach(GRIPPER_PIN);
  gripper.write(180);

  //Assume version 1 for now
  layout = LAYOUT_A;

  //Start with the blocks immediately in front of us
  if(layout == LAYOUT_A)
  {
    zone = ZONE_C;
  }
  else
  {
    zone = ZONE_A;
  }
  
}

void CheckPixy()
{
  //test();
  //RaiseClaw();

  static int i = 0;
  int j;
  uint16_t numBlocks;

  //In mm
  int distance = 0;

  numBlocks = pixy.getBlocks();

  // If there are detect blocks, print them!
  if (numBlocks)
  {
    if(blocksig == -1)
    {
      blocksig = 0;
      color = pixy.blocks[blocksig].signature;
    }
    //pixy.blocks[0].print();
    distance = CheckDistance();
    //CheckHeight(distance);
  }
  else
  {
    //Rotate robot until blocks are detected
    //RotateClockwise(20);
  }
}

void CheckHeight(int distance)
{
  int diff = ((PIXY_MAX_Y / 2) - (pixy.blocks[blocksig].y + (pixy.blocks[blocksig].height / 2)) ) ;
  
  if(distance < 60)
  { 
    //Serial.println(diff);

    if (diff > 5)
    {
      //LinearMotorMoveDown();
    }
    else if(diff < -5)
    {
      //LinearMotorMoveDown();
    }
    else
    {
      Serial.println("Good");
    }
  }
  else if(haveBlock)
  {
    ptrBlockRelease();
    haveBlock = false;
    blocksig = -1;
  }
  else
  {
    PickupBlock();
    FindDestination();
    haveBlock = true;
  }
}

void test()
{
  unsigned long distanceSonar;
  unsigned long distanceSonar2;

  GetDistance(distanceSonar, distanceSonar2);
  Serial.print("Sonar1: ");
  Serial.println(distanceSonar);
  Serial.print("Sonar2: ");
  Serial.println(distanceSonar2);
}

int CheckDistance()
{

  int distance = DIST_CONST / pixy.blocks[blocksig].width;
  unsigned long distanceSonar;
  unsigned long distanceSonar2;

  GetDistance(distanceSonar, distanceSonar2);
  
  if (distanceSonar < distanceSonar2)
  {
    distanceSonar = distanceSonar2;
  }
  static bool movingLeft = false;
  static bool movingRight = false;
  static bool movingFW = false;
  static bool centered = false;
  //Serial.println(pixy.blocks[blocksig].x);
  //Serial.println(pixy.blocks[0].width);

  float centeringMod = ((float)distanceSonar / (float)MIN_DIST) * (float)MIN_TOLERANCE;
  int speed = 14;
  
  int diff = (PIXY_MAX_X / 2) - (pixy.blocks[blocksig].x + (pixy.blocks[blocksig].width / 2)) + HOR_OFFSET;

  if (distanceSonar <= MIN_DIST)
  {
    state = CLOSE;
  }
  else if (distanceSonar <= (MIN_DIST + 1200))
  {
    state = MED;
  }
  
  if (state == FAR)
  {
    /*
    Serial.print("Diff: ");
    Serial.println(diff);

    Serial.print("Mod: ");
    Serial.println(centeringMod);
    */
    if (diff > centeringMod)
    {
      if(!movingLeft)
      {
        movingLeft = true;
        movingRight = false;
        movingFW = false;
        driveOff();
        driveOn(DirLeft, speed, 5, 25);
        //Serial.println("Begin Left");
      }
      //Serial.println("Moving Left");
    }
    else if(diff < -centeringMod)
    {
      if(!movingRight)
      {
        movingLeft = false;
        movingRight = true;
        movingFW = false;
        driveOff();
        driveOn(DirRight, speed, 5, 25);
        //Serial.println("Begin Right");
      }
      //Serial.println("Moving Right");
      
    }
    else
    {
      if(!movingFW)
      {
        movingLeft = false;
        movingRight = false;
        movingFW = true;
        driveOff();
        driveOn(DirForward, speed, 5, 25);
        //Serial.println("Begin FW");
      }  
      //Serial.println("Moving Forward");
    }
  }
  else if (state == MED)
  {
    if (!centered)
    {
      driveOff();
      movingLeft = false;
      movingRight = false;
      movingFW = false;
      CenterRobot();
      centered = true;
    }
    else if (diff > centeringMod)
    {
      if(!movingLeft)
      {
        movingLeft = true;
        movingRight = false;
        movingFW = false;
        driveOff();
        driveOn(DirLeft, speed, 5, 25);
        //Serial.println("Begin Left");
      }
      //Serial.println("Moving Left");
    }
    else if(diff < -centeringMod)
    {
      if(!movingRight)
      {
        movingLeft = false;
        movingRight = true;
        movingFW = false;
        driveOff();
        driveOn(DirRight, speed, 5, 25);
        //Serial.println("Begin Right");
      }
      //Serial.println("Moving Right");
      
    }
    else
    {
      if(!movingFW)
      {
        movingLeft = false;
        movingRight = false;
        movingFW = true;
        driveOff();
        driveOn(DirForward, speed, 5, 25);
        //Serial.println("Begin FW");
      }  
      //Serial.println("Moving Forward");
    }
  }
  else
  {
    driveOff();
    movingLeft = false;
    movingRight = false;
    movingFW = false;
    CenterRobot();
    CenterClaw();
    RaiseClaw();
    driveOn(DirForward, speed, 5, 25);
    delay(700);
    driveOff();
    delay(2400);
    CloseGripper();
    driveOn(DirBackward, speed, 5, 25);
    delay(3000);
    driveOff();

    vertStepper.step(-vertStepCount);
    digitalWrite(vertRelay, HIGH);
  }
  
    return distance;
}

void CenterRobot()
{
  Serial.println("Centering");
  unsigned long distanceSonar;
  unsigned long distanceSonar2;
  GetDistance(distanceSonar, distanceSonar2);

  /*
  Serial.print("Sonar1: ");
  Serial.println(distanceSonar);
  Serial.print("Sonar2: ");
  Serial.println(distanceSonar2);
  */
  
  int difference = distanceSonar2 - distanceSonar;
  bool right = true;
  if (difference > ROT_TOL)
    {
      right = true;
      //driveOn(DirRotateRight, 15, 5, 25);
    }
    else if(difference < -ROT_TOL)
    {
      //driveOn(DirRotateLeft, 15, 5, 25);
      right = false;
    }
  if(right)
  {
    while(difference >= 0)
    {
      driveOn(DirRotateRight, 15, 5, 25);
      delay(60);
      driveOff();
      GetDistance(distanceSonar, distanceSonar2);
      
      difference = distanceSonar2 - distanceSonar;
      //Serial.print("Diff: ");
      //Serial.println(difference);
    }
  }
  else
  {
    while(difference <= 0)
    {
      driveOn(DirRotateLeft, 15, 5, 25);
      delay(50);
      driveOff();
      GetDistance(distanceSonar, distanceSonar2);
      difference = distanceSonar2 - distanceSonar;
    }
  }
  
  driveOff();
}

void CenterClaw()
{
  blocksig = 0;
  int numBlocks = pixy.getBlocks();
  int tolerance = 5;
  int diff = (PIXY_MAX_X / 2) - (pixy.blocks[blocksig].x + (pixy.blocks[blocksig].width / 2)) + 20;
  int stp = 5;
  while(diff > tolerance || diff < -tolerance)
  {
    Serial.print("Diff: ");
    Serial.println(diff);
    if (diff > tolerance)
    {
      if (diff > 2*tolerance)
      {
        stp = 10;
      }
      else
      {
        stp = 5;
      }
      digitalWrite(horRelay,LOW);
      horStepper.step(-stp);
      horStepCount += -stp;
      digitalWrite(horRelay, HIGH);
    }
    else if(diff < -tolerance)
    {
      if (diff < -2*tolerance)
      {
        stp = 10;
      }
      else
      {
        stp = 5;
      }
      digitalWrite(horRelay,LOW);
      horStepper.step(stp);
      horStepCount += stp;
      digitalWrite(horRelay, HIGH);
    }
    delay(200);
    pixy.getBlocks();
    diff = (PIXY_MAX_X / 2) - (pixy.blocks[blocksig].x + (pixy.blocks[blocksig].width / 2)) + 20;
    if(horStepCount >= 500 || horStepCount <= -500)
    {
      break;
    }
  }
  

  driveOff();
  //driveOn(DirForward, 15, 5, 25);
  delay(800);
  driveOff();
}

void RaiseClaw()
{
    digitalWrite(vertRelay,LOW);
    vertStepper.step(375);
    vertStepCount += 375;
    delay(1000);
      
}

void CloseGripper()
{
  Serial.println("OPENING");
  gripper.write(130);
  
}

void OpenGripper()
{
  gripper.write(0);
}

void MoveForward()
{
  //Serial.print("Moving \n");
}

void PickupBlock()
{
  //Do something
}

void FindDestination()
{

  switch(zone)
  {
    case ZONE_A:
      ZoneA();
      break;
    case ZONE_B:
      ZoneB();
      break;
    case ZONE_C:
      ZoneC();
      break;
  }
  //Turn180()
}

void ZoneA()
{
  if(layout == LAYOUT_A)
  {
    //RotateClockwise(180);
    while(!FindBlockByColor(TAN))
    {
      //RotateClockwise(10);
    }
  }
  else
  {
    //RotateClockwise(170);
    while(!FindBlockByColor(TAN))
    {
      //RotateClockwise(10);
    }
  }
  ptrBlockRelease = &PutOnBoat;
}
void ZoneB()
{
  
}

void ZoneC()
{
  if(layout == LAYOUT_A)
  {
    if(color != BLUE)
    {
      //TurnLeft(170);
      while(!FindBlockByColor(BLUE))
      {
        //RotateCounterClockwise(10);
      }
      
      ptrBlockRelease = &PutInTruck;
    }
    else
    {
      //TurnLeft(120);
      while(!FindBlockByColor(TAN))
      {
        //RotateCounterClockwise(10);
      }

      ptrBlockRelease = &PutOnBoat;
    }
    
  }
  else
  {
    //RotateCounterClockwise(100);
  }
}

bool FindBlockByColor(int _col)
{
  int numBlocks = pixy.getBlocks();
  
  if (numBlocks)
  {
    for(int i = 0; i < numBlocks; ++i)
    {
      if(pixy.blocks[i].signature == _col)
      {
        blocksig = i;
      }
    }
  }
}

void PutInTruck()
{
  //Move linear motors all the way down
  //Open gripper

  if(layout == LAYOUT_A)
  {
    //RotateClockwise(180);
  }
  else
  {
    
  }
}

void PutOnBoat()
{
  //Move linear motors all the way down
  //Open gripper
  
  if(layout == LAYOUT_A)
  {
    //RotateClockwise(180);
  }
  else
  {
    
  }
}

void setupSteppers() {

    // set the speed at 60 rpm:
    vertStepper.setSpeed(50);
    vertStepCount = 0; // number of steps the motor has taken

    // set the speed at 60 rpm:
    horStepper.setSpeed(60);
    horStepCount = 0; // number of steps the motor has taken

}



