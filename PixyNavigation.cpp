
#include "PixyNavigation.h"
#include "Encoders.h"
// This is the main Pixy object
Pixy pixy;

int16_t blocksig;
bool haveBlock;
int zone;
int layout;
int color;

//This will hold the function that releases the block into the right container
void (*ptrBlockRelease)();

void SetupPixy()
{
  Serial.begin(9600);
  Serial.println("Starting...\n");
  haveBlock = false;
  blocksig = -1;
  pixy.init();

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
  int diff = ((PIXY_MAX_Y / 2) - (pixy.blocks[blocksig].y + (pixy.blocks[blocksig].height / 2)) );
  
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

int CheckDistance()
{

  int distance = DIST_CONST / pixy.blocks[blocksig].width;
  static bool movingLeft = false;
  static bool movingRight = false;
  static bool movingFW = false;
  Serial.println(pixy.blocks[blocksig].x);
  //Serial.println(pixy.blocks[0].width);

  float var = distance / 254;

  int diff = (PIXY_MAX_X / 2) - (pixy.blocks[blocksig].x + (pixy.blocks[blocksig].width / 2));
  var = 30.0 * var;

   if (diff > var)
   {
      if(!movingLeft)
      {
        movingLeft = true;
        movingRight = false;
        movingFW = false;
        driveOff();
        driveOn(DirLeft, 30, 5, 25);
        Serial.println("Begin Left");
      }
      //Serial.println("Moving Left");
    }
    else if(diff < -var)
    {
      if(!movingRight)
      {
        movingLeft = false;
        movingRight = true;
        movingFW = false;
        driveOff();
        driveOn(DirRight, 30, 5, 25);
        Serial.println("Begin Right");
      }
      //Serial.println("Moving Right");
      
    }
    else if (distance > 254)
    {
      if(!movingFW)
      {
        movingLeft = false;
        movingRight = false;
        movingFW = true;
        driveOff();
        driveOn(DirForward, 30, 5, 25);
        Serial.println("Begin FW");
      }  
      Serial.println("Moving Forward");
    }
    else
    {
      driveOff();
      movingLeft = false;
      movingRight = false;
      movingFW = false;
      Serial.println("Stopping");
    }
  
    return distance;
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

