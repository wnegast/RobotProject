#include <Arduino.h>
#include "Encoders.h"
#include "digitalWriteFast.h"
#include "TimerThree.h"


// Quadrature encoders
// Front Left encoder
#define c_FLeftEncoderPinA 2
#define c_FLeftEncoderPinB 4
#define FLeftEncoderIsReversed
volatile bool _FLeftEncoderBSet;
volatile long _FLeftEncoderTicks;

// Front Right encoder
#define c_FRightEncoderPinA 3
#define c_FRightEncoderPinB 5
volatile bool _FRightEncoderBSet;
volatile long _FRightEncoderTicks;

// Back Left encoder
#define c_BLeftEncoderPinA 18
#define c_BLeftEncoderPinB 6
#define BLeftEncoderIsReversed
volatile bool _BLeftEncoderBSet;
volatile long _BLeftEncoderTicks;

// Back Right encoder
#define c_BRightEncoderPinA 19
#define c_BRightEncoderPinB 7
volatile bool _BRightEncoderBSet;
volatile long _BRightEncoderTicks;

//
volatile long _preFLeftEncoderTicks;
volatile long _preFRightEncoderTicks;
volatile long _preBLeftEncoderTicks;
volatile long _preBRightEncoderTicks;

//syncMotors() arguments
float sm_FLspeed;
float sm_FRspeed;
float sm_BLspeed;
float sm_BRspeed;
float sm_LBtolerance;
float sm_HBtolerance;

//The amount of speed that changes when a changes is needed
float IncDecAmount;

int Direction;


void setupEncoders()
{
	  //I color coded the encoder wires. Blue = Front, Black = Back, Yellow = Left, Green = Right. PinA = Yellow wire, PinB = White wire.
	  // Pin A  B
	  // FL: 2  4
	  // FR: 3  5
	  // BL: 18 6
	  // BR: 19 7

	  // Quadrature encoders
	  // Front Left encoder
	  pinMode(c_FLeftEncoderPinA, INPUT);      // sets pin A as input
	  digitalWrite(c_FLeftEncoderPinA, LOW);  // turn on pullup resistors
	  pinMode(c_FLeftEncoderPinB, INPUT);      // sets pin B as input
	  digitalWrite(c_FLeftEncoderPinB, LOW);  // turn on pullup resistors


	  // Front Right encoder
	  pinMode(c_FRightEncoderPinA, INPUT);      // sets pin A as input
	  digitalWrite(c_FRightEncoderPinA, LOW);  // turn on pullup resistors
	  pinMode(c_FRightEncoderPinB, INPUT);      // sets pin B as input
	  digitalWrite(c_FRightEncoderPinB, LOW);  // turn on pullup resistors


	  // Back Left encoder
	  pinMode(c_BLeftEncoderPinA, INPUT);      // sets pin A as input
	  digitalWrite(c_BLeftEncoderPinA, LOW);  // turn on pullup resistors
	  pinMode(c_BLeftEncoderPinB, INPUT);      // sets pin B as input
	  digitalWrite(c_BLeftEncoderPinB, LOW);  // turn on pullup resistors


	  // Back Right encoder
	  pinMode(c_BRightEncoderPinA, INPUT);      // sets pin A as input
	  digitalWrite(c_BRightEncoderPinA, LOW);  // turn on pullup resistors
	  pinMode(c_BRightEncoderPinB, INPUT);      // sets pin B as input
	  digitalWrite(c_BRightEncoderPinB, LOW);  // turn on pullup resistors

    //Sync period for motors
    //Timer3.initialize(1000000);
    //Timer3.initialize(500000); // initialize timer1, and set a 1/2 second period
    //Timer3.initialize(250000); // initialize timer1, and set a 1/4 second period
    Timer3.initialize(100000); // initialize timer1, and set a 1/10 second period

    _FLeftEncoderTicks = 0;
    _FRightEncoderTicks = 0;
    _BLeftEncoderTicks = 0;
    _BRightEncoderTicks = 0;

    _preFLeftEncoderTicks = 0;
    _preFRightEncoderTicks = 0;
    _preBLeftEncoderTicks = 0;
    _preBRightEncoderTicks = 0;

    sm_FLspeed = 90.0;
    sm_FRspeed = 90.0;
    sm_BLspeed = 90.0;
    sm_BRspeed = 90.0;
    sm_LBtolerance = 10.0;
    sm_HBtolerance = 20.0;
    IncDecAmount = 1.0;
    
    Direction = DirForward;
}

void driveOn(int dir, int speeed, int LBtolerance, int HBtolerance)
{
  sm_LBtolerance = LBtolerance;
  sm_HBtolerance = HBtolerance;
  attachInterrupt(digitalPinToInterrupt(c_FLeftEncoderPinA), HandleFLeftMotorInterruptA, RISING); //Comment out if not using FL encoder
  attachInterrupt(digitalPinToInterrupt(c_FRightEncoderPinA), HandleFRightMotorInterruptA, RISING); //Comment out if not using FR encoder
  attachInterrupt(digitalPinToInterrupt(c_BLeftEncoderPinA), HandleBLeftMotorInterruptA, RISING); //Comment out if not using BL encoder
  attachInterrupt(digitalPinToInterrupt(c_BRightEncoderPinA), HandleBRightMotorInterruptA, RISING); //Comment out if not using BR encoder
  Direction = dir;

  switch(dir)
  {
    case 1:
            sm_FLspeed = 90+speeed;
            sm_FRspeed = 90+speeed;
            sm_BLspeed = 90+speeed;
            sm_BRspeed = 90+speeed;
            Timer3.attachInterrupt(syncMotorsForward);
            break;

    case 2:
            sm_FLspeed = 90-speeed;
            sm_FRspeed = 90-speeed;
            sm_BLspeed = 90-speeed;
            sm_BRspeed = 90-speeed;
            Timer3.attachInterrupt(syncMotorsBackward);
            break;

    case 3:
            sm_FLspeed = 90-speeed;
            sm_FRspeed = 90+speeed;
            sm_BLspeed = 90+speeed;
            sm_BRspeed = 90-speeed;
            Timer3.attachInterrupt(syncMotorsLeft);
            break;

    case 4:
            sm_FLspeed = 90+speeed;
            sm_FRspeed = 90-speeed;
            sm_BLspeed = 90-speeed;
            sm_BRspeed = 90+speeed;
            Timer3.attachInterrupt(syncMotorsRight);
            break;

    case 5:
            sm_FLspeed = 90-speeed;
            sm_FRspeed = 90+speeed;
            sm_BLspeed = 90-speeed;
            sm_BRspeed = 90+speeed;
            Timer3.attachInterrupt(syncMotorsRotateLeft);
            break;

    case 6:
            sm_FLspeed = 90+speeed;
            sm_FRspeed = 90-speeed;
            sm_BLspeed = 90+speeed;
            sm_BRspeed = 90-speeed;
            Timer3.attachInterrupt(syncMotorsRotateRight);
            break;

    default:
            sm_FLspeed = 90+speeed;
            sm_FRspeed = 90+speeed;
            sm_BLspeed = 90+speeed;
            sm_BRspeed = 90+speeed;
            Timer3.attachInterrupt(syncMotorsForward);
            break;
  }
}

void driveOff()
{
  //Serial.println("Stopping");
  driveStop();
  detachInterrupt(digitalPinToInterrupt(c_FLeftEncoderPinA)); //Comment out if not using FL encoder
  detachInterrupt(digitalPinToInterrupt(c_FRightEncoderPinA)); //Comment out if not using FR encoder
  detachInterrupt(digitalPinToInterrupt(c_BLeftEncoderPinA)); //Comment out if not using BL encoder
  detachInterrupt(digitalPinToInterrupt(c_BRightEncoderPinA)); //Comment out if not using BR encoder

  Timer3.detachInterrupt();
  resetEncoderTicks();
}

void resetEncoderTicks()
{
  _FLeftEncoderTicks = 0;
  _FRightEncoderTicks = 0;
  _BLeftEncoderTicks = 0;
  _BRightEncoderTicks = 0;

  sm_FLspeed = 90.0;
  sm_FRspeed = 90.0;
  sm_BLspeed = 90.0;
  sm_BRspeed = 90.0;
  sm_LBtolerance = 10.0;
  sm_HBtolerance = 20.0;
  Direction = DirForward;
}
void syncMotorsForward()
{
  long FLdiff = abs(_FLeftEncoderTicks - _preFLeftEncoderTicks);
  long FRdiff = abs(_FRightEncoderTicks - _preFRightEncoderTicks);
  long BLdiff = abs(_BLeftEncoderTicks - _preBLeftEncoderTicks);
  long BRdiff = abs(_BRightEncoderTicks - _preBRightEncoderTicks);

  //Set which wheel sets the base speed the others try to mach
  long baseDiff = FLdiff;
  float baseSpeed = sm_FLspeed;

  if( FLdiff < baseDiff - sm_LBtolerance && FLdiff > baseDiff - sm_HBtolerance)
  {
    sm_FLspeed+=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else if( FLdiff > baseDiff + sm_LBtolerance && FLdiff < baseDiff + sm_HBtolerance)
  {
    sm_FLspeed-=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else
  {
    sm_FLspeed = baseSpeed;
    driveFL(sm_FLspeed);
  }

  if( FRdiff < baseDiff - sm_LBtolerance && FRdiff > baseDiff - sm_HBtolerance)
  {
    sm_FRspeed+=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else if( FRdiff > baseDiff + sm_LBtolerance && FRdiff < baseDiff + sm_HBtolerance)
  {
    sm_FRspeed-=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else
  {
    sm_FRspeed = baseSpeed;
    driveFR(sm_FRspeed);
  }

  if( BLdiff < baseDiff - sm_LBtolerance && BLdiff > baseDiff - sm_HBtolerance)
  {
    sm_BLspeed+=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else if( BLdiff > baseDiff + sm_LBtolerance && BLdiff < baseDiff + sm_HBtolerance)
  {
    sm_BLspeed-=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else
  {
    sm_BLspeed = baseSpeed;
    driveBL(sm_BLspeed);
  }

  if( BRdiff < baseDiff - sm_LBtolerance && BRdiff > baseDiff - sm_HBtolerance)
  {
    sm_BRspeed+=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else if( BRdiff > baseDiff + sm_LBtolerance && BRdiff < baseDiff - sm_HBtolerance)
  {
    sm_BRspeed-=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else
  {
    sm_BRspeed = baseSpeed;
    driveBR(sm_BRspeed);
  }

  /*Serial.print(sm_FLspeed);
  Serial.print("\t");
  Serial.print(sm_FRspeed);
  Serial.print("\t");
  Serial.print(sm_BLspeed);
  Serial.print("\t");
  Serial.print(sm_BRspeed);
  Serial.print("\n");
  Serial.print(FLdiff);
  Serial.print("\t");
  Serial.print(FRdiff);
  Serial.print("\t");
  Serial.print(BLdiff);
  Serial.print("\t");
  Serial.print(BRdiff);
  Serial.print("\n\n");
  /*Serial.print(_FLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_FRightEncoderTicks);
  Serial.print("\t");
  Serial.print(_BLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_BRightEncoderTicks);
  Serial.print("\n");*/

  _preFLeftEncoderTicks = _FLeftEncoderTicks;
  _preFRightEncoderTicks = _FRightEncoderTicks;
  _preBLeftEncoderTicks = _BLeftEncoderTicks;
  _preBRightEncoderTicks = _BRightEncoderTicks;
}

void syncMotorsBackward()
{
  long FLdiff = abs(_FLeftEncoderTicks - _preFLeftEncoderTicks);
  long FRdiff = abs(_FRightEncoderTicks - _preFRightEncoderTicks);
  long BLdiff = abs(_BLeftEncoderTicks - _preBLeftEncoderTicks);
  long BRdiff = abs(_BRightEncoderTicks - _preBRightEncoderTicks);

  //Set which wheel sets the base speed the others try to mach
  long baseDiff = FLdiff;
  float baseSpeed = sm_FLspeed;

  if( FLdiff < baseDiff - sm_LBtolerance && FLdiff > baseDiff - sm_HBtolerance)
  {
    sm_FLspeed-=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else if( FLdiff > baseDiff + sm_LBtolerance && FLdiff < baseDiff + sm_HBtolerance)
  {
    sm_FLspeed+=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else
  {
    sm_FLspeed = baseSpeed;
    driveFL(sm_FLspeed);
  }

  if( FRdiff < baseDiff - sm_LBtolerance && FRdiff > baseDiff - sm_HBtolerance)
  {
    sm_FRspeed-=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else if( FRdiff > baseDiff + sm_LBtolerance && FRdiff < baseDiff + sm_HBtolerance)
  {
    sm_FRspeed+=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else
  {
    sm_FRspeed = baseSpeed;
    driveFR(sm_FRspeed);
  }

  if( BLdiff < baseDiff - sm_LBtolerance && BLdiff > baseDiff - sm_HBtolerance)
  {
    sm_BLspeed-=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else if( BLdiff > baseDiff + sm_LBtolerance && BLdiff < baseDiff + sm_HBtolerance)
  {
    sm_BLspeed+=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else
  {
    sm_BLspeed = baseSpeed;
    driveBL(sm_BLspeed);
  }

  if( BRdiff < baseDiff - sm_LBtolerance && BRdiff > baseDiff - sm_HBtolerance)
  {
    sm_BRspeed-=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else if( BRdiff > baseDiff + sm_LBtolerance && BRdiff < baseDiff - sm_HBtolerance)
  {
    sm_BRspeed+=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else
  {
    sm_BRspeed = baseSpeed;
    driveBR(sm_BRspeed);
  }
  /*
  Serial.print(sm_FLspeed);
  Serial.print("\t");
  Serial.print(sm_FRspeed);
  Serial.print("\t");
  Serial.print(sm_BLspeed);
  Serial.print("\t");
  Serial.print(sm_BRspeed);
  Serial.print("\n");
  Serial.print(FLdiff);
  Serial.print("\t");
  Serial.print(FRdiff);
  Serial.print("\t");
  Serial.print(BLdiff);
  Serial.print("\t");
  Serial.print(BRdiff);
  Serial.print("\n\n");
  /*Serial.print(_FLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_FRightEncoderTicks);
  Serial.print("\t");
  Serial.print(_BLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_BRightEncoderTicks);
  Serial.print("\n");*/

  _preFLeftEncoderTicks = _FLeftEncoderTicks;
  _preFRightEncoderTicks = _FRightEncoderTicks;
  _preBLeftEncoderTicks = _BLeftEncoderTicks;
  _preBRightEncoderTicks = _BRightEncoderTicks;
}

void syncMotorsLeft()
{
  long FLdiff = abs(_FLeftEncoderTicks - _preFLeftEncoderTicks);
  long FRdiff = abs(_FRightEncoderTicks - _preFRightEncoderTicks);
  long BLdiff = abs(_BLeftEncoderTicks - _preBLeftEncoderTicks);
  long BRdiff = abs(_BRightEncoderTicks - _preBRightEncoderTicks);

  //Set which wheel sets the base speed the others try to mach
  long baseDiff = FLdiff;
  //float baseSpeed = sm_FRspeed;
  //float baseSpeedR = sm_FLspeed;
  float baseSpeed = sm_FLspeed + ((90 - sm_FLspeed)*2);
  float baseSpeedR = sm_FLspeed;

  if( FLdiff < baseDiff - sm_LBtolerance && FLdiff > baseDiff - sm_HBtolerance)
  {
    sm_FLspeed-=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else if( FLdiff > baseDiff + sm_LBtolerance && FLdiff < baseDiff + sm_HBtolerance)
  {
    sm_FLspeed+=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else
  {
    sm_FLspeed = baseSpeedR;
    driveFL(sm_FLspeed);
  }

  if( FRdiff < baseDiff - sm_LBtolerance && FRdiff > baseDiff - sm_HBtolerance)
  {
    sm_FRspeed+=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else if( FRdiff > baseDiff + sm_LBtolerance && FRdiff < baseDiff + sm_HBtolerance)
  {
    sm_FRspeed-=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else
  {
    sm_FRspeed = baseSpeed;
    driveFR(sm_FRspeed);
  }

  if( BLdiff < baseDiff - sm_LBtolerance && BLdiff > baseDiff - sm_HBtolerance)
  {
    sm_BLspeed+=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else if( BLdiff > baseDiff + sm_LBtolerance && BLdiff < baseDiff + sm_HBtolerance)
  {
    sm_BLspeed-=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else
  {
    sm_BLspeed = baseSpeed;
    driveBL(sm_BLspeed);
  }

  if( BRdiff < baseDiff - sm_LBtolerance && BRdiff > baseDiff - sm_HBtolerance)
  {
    sm_BRspeed-=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else if( BRdiff > baseDiff + sm_LBtolerance && BRdiff < baseDiff - sm_HBtolerance)
  {
    sm_BRspeed+=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else
  {
    sm_BRspeed = baseSpeedR;
    driveBR(sm_BRspeed);
  }

  /*
  Serial.print(sm_FLspeed);
  Serial.print("\t");
  Serial.print(sm_FRspeed);
  Serial.print("\t");
  Serial.print(sm_BLspeed);
  Serial.print("\t");
  Serial.print(sm_BRspeed);
  Serial.print("\n");
  Serial.print(FLdiff);
  Serial.print("\t");
  Serial.print(FRdiff);
  Serial.print("\t");
  Serial.print(BLdiff);
  Serial.print("\t");
  Serial.print(BRdiff);
  Serial.print("\n\n");
  /*Serial.print(_FLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_FRightEncoderTicks);
  Serial.print("\t");
  Serial.print(_BLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_BRightEncoderTicks);
  Serial.print("\n");*/

  _preFLeftEncoderTicks = _FLeftEncoderTicks;
  _preFRightEncoderTicks = _FRightEncoderTicks;
  _preBLeftEncoderTicks = _BLeftEncoderTicks;
  _preBRightEncoderTicks = _BRightEncoderTicks;
}

void syncMotorsRight()
{
  long FLdiff = abs(_FLeftEncoderTicks - _preFLeftEncoderTicks);
  long FRdiff = abs(_FRightEncoderTicks - _preFRightEncoderTicks);
  long BLdiff = abs(_BLeftEncoderTicks - _preBLeftEncoderTicks);
  long BRdiff = abs(_BRightEncoderTicks - _preBRightEncoderTicks);

  //Set which wheel sets the base speed the others try to mach
  long baseDiff = FLdiff;
  //float baseSpeed = sm_FLspeed;
  //float baseSpeedR = sm_FRspeed;
  float baseSpeed = sm_FLspeed;
  float baseSpeedR = 90 - (sm_FLspeed-90);

  if( FLdiff < baseDiff - sm_LBtolerance && FLdiff > baseDiff - sm_HBtolerance)
  {
    sm_FLspeed+=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else if( FLdiff > baseDiff + sm_LBtolerance && FLdiff < baseDiff + sm_HBtolerance)
  {
    sm_FLspeed-=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else
  {
    sm_FLspeed = baseSpeed;
    driveFL(sm_FLspeed);
  }

  if( FRdiff < baseDiff - sm_LBtolerance && FRdiff > baseDiff - sm_HBtolerance)
  {
    sm_FRspeed-=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else if( FRdiff > baseDiff + sm_LBtolerance && FRdiff < baseDiff + sm_HBtolerance)
  {
    sm_FRspeed+=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else
  {
    sm_FRspeed = baseSpeedR;
    driveFR(sm_FRspeed);
  }

  if( BLdiff < baseDiff - sm_LBtolerance && BLdiff > baseDiff - sm_HBtolerance)
  {
    sm_BLspeed-=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else if( BLdiff > baseDiff + sm_LBtolerance && BLdiff < baseDiff + sm_HBtolerance)
  {
    sm_BLspeed+=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else
  {
    sm_BLspeed = baseSpeedR;
    driveBL(sm_BLspeed);
  }

  if( BRdiff < baseDiff - sm_LBtolerance && BRdiff > baseDiff - sm_HBtolerance)
  {
    sm_BRspeed+=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else if( BRdiff > baseDiff + sm_LBtolerance && BRdiff < baseDiff - sm_HBtolerance)
  {
    sm_BRspeed-=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else
  {
    sm_BRspeed = baseSpeed;
    driveBR(sm_BRspeed);
  }

  /*
  Serial.print(sm_FLspeed);
  Serial.print("\t");
  Serial.print(sm_FRspeed);
  Serial.print("\t");
  Serial.print(sm_BLspeed);
  Serial.print("\t");
  Serial.print(sm_BRspeed);
  Serial.print("\n");
  Serial.print(FLdiff);
  Serial.print("\t");
  Serial.print(FRdiff);
  Serial.print("\t");
  Serial.print(BLdiff);
  Serial.print("\t");
  Serial.print(BRdiff);
  Serial.print("\n\n");
  /*Serial.print(_FLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_FRightEncoderTicks);
  Serial.print("\t");
  Serial.print(_BLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_BRightEncoderTicks);
  Serial.print("\n");*/

  _preFLeftEncoderTicks = _FLeftEncoderTicks;
  _preFRightEncoderTicks = _FRightEncoderTicks;
  _preBLeftEncoderTicks = _BLeftEncoderTicks;
  _preBRightEncoderTicks = _BRightEncoderTicks;
}

void syncMotorsRotateLeft()
{
  long FLdiff = abs(_FLeftEncoderTicks - _preFLeftEncoderTicks);
  long FRdiff = abs(_FRightEncoderTicks - _preFRightEncoderTicks);
  long BLdiff = abs(_BLeftEncoderTicks - _preBLeftEncoderTicks);
  long BRdiff = abs(_BRightEncoderTicks - _preBRightEncoderTicks);

  //Set which wheel sets the base speed the others try to mach
 long baseDiff = FLdiff;
  //float baseSpeed = sm_FRspeed;
  //float baseSpeedR = sm_FLspeed;
  float baseSpeed = sm_FLspeed + ((90 - sm_FLspeed)*2);
  float baseSpeedR = sm_FLspeed;

  if( FLdiff < baseDiff - sm_LBtolerance && FLdiff > baseDiff - sm_HBtolerance)
  {
    sm_FLspeed-=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else if( FLdiff > baseDiff + sm_LBtolerance && FLdiff < baseDiff + sm_HBtolerance)
  {
    sm_FLspeed+=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else
  {
    sm_FLspeed = baseSpeedR;
    driveFL(sm_FLspeed);
  }

  if( FRdiff < baseDiff - sm_LBtolerance && FRdiff > baseDiff - sm_HBtolerance)
  {
    sm_FRspeed+=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else if( FRdiff > baseDiff + sm_LBtolerance && FRdiff < baseDiff + sm_HBtolerance)
  {
    sm_FRspeed-=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else
  {
    sm_FRspeed = baseSpeed;
    driveFR(sm_FRspeed);
  }

  if( BLdiff < baseDiff - sm_LBtolerance && BLdiff > baseDiff - sm_HBtolerance)
  {
    sm_BLspeed-=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else if( BLdiff > baseDiff + sm_LBtolerance && BLdiff < baseDiff + sm_HBtolerance)
  {
    sm_BLspeed+=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else
  {
    sm_BLspeed = baseSpeedR;
    driveBL(sm_BLspeed);
  }

  if( BRdiff < baseDiff - sm_LBtolerance && BRdiff > baseDiff - sm_HBtolerance)
  {
    sm_BRspeed+=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else if( BRdiff > baseDiff + sm_LBtolerance && BRdiff < baseDiff - sm_HBtolerance)
  {
    sm_BRspeed-=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else
  {
    sm_BRspeed = baseSpeed;
    driveBR(sm_BRspeed);
  }
  /*
  Serial.print(sm_FLspeed);
  Serial.print("\t");
  Serial.print(sm_FRspeed);
  Serial.print("\t");
  Serial.print(sm_BLspeed);
  Serial.print("\t");
  Serial.print(sm_BRspeed);
  Serial.print("\n");
  Serial.print(FLdiff);
  Serial.print("\t");
  Serial.print(FRdiff);
  Serial.print("\t");
  Serial.print(BLdiff);
  Serial.print("\t");
  Serial.print(BRdiff);
  Serial.print("\n\n");
  /*Serial.print(_FLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_FRightEncoderTicks);
  Serial.print("\t");
  Serial.print(_BLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_BRightEncoderTicks);
  Serial.print("\n");*/

  _preFLeftEncoderTicks = _FLeftEncoderTicks;
  _preFRightEncoderTicks = _FRightEncoderTicks;
  _preBLeftEncoderTicks = _BLeftEncoderTicks;
  _preBRightEncoderTicks = _BRightEncoderTicks;
}

void syncMotorsRotateRight()
{
  long FLdiff = abs(_FLeftEncoderTicks - _preFLeftEncoderTicks);
  long FRdiff = abs(_FRightEncoderTicks - _preFRightEncoderTicks);
  long BLdiff = abs(_BLeftEncoderTicks - _preBLeftEncoderTicks);
  long BRdiff = abs(_BRightEncoderTicks - _preBRightEncoderTicks);

  //Set which wheel sets the base speed the others try to mach
  long baseDiff = FLdiff;
  //float baseSpeed = sm_FLspeed;
  //float baseSpeedR = sm_FRspeed;
  float baseSpeed = sm_FLspeed;
  float baseSpeedR = 90 - (sm_FLspeed-90);

  if( FLdiff < baseDiff - sm_LBtolerance && FLdiff > baseDiff - sm_HBtolerance)
  {
    sm_FLspeed+=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else if( FLdiff > baseDiff + sm_LBtolerance && FLdiff < baseDiff + sm_HBtolerance)
  {
    sm_FLspeed-=IncDecAmount;
    driveFL(sm_FLspeed);
  }
  else
  {
    sm_FLspeed = baseSpeed;
    driveFL(sm_FLspeed);
  }

  if( FRdiff < baseDiff - sm_LBtolerance && FRdiff > baseDiff - sm_HBtolerance)
  {
    sm_FRspeed-=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else if( FRdiff > baseDiff + sm_LBtolerance && FRdiff < baseDiff + sm_HBtolerance)
  {
    sm_FRspeed+=IncDecAmount;
    driveFR(sm_FRspeed);
  }
  else
  {
    sm_FRspeed = baseSpeedR;
    driveFR(sm_FRspeed);
  }

  if( BLdiff < baseDiff - sm_LBtolerance && BLdiff > baseDiff - sm_HBtolerance)
  {
    sm_BLspeed+=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else if( BLdiff > baseDiff + sm_LBtolerance && BLdiff < baseDiff + sm_HBtolerance)
  {
    sm_BLspeed-=IncDecAmount;
    driveBL(sm_BLspeed);
  }
  else
  {
    sm_BLspeed = baseSpeed;
    driveBL(sm_BLspeed);
  }

  if( BRdiff < baseDiff - sm_LBtolerance && BRdiff > baseDiff - sm_HBtolerance)
  {
    sm_BRspeed-=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else if( BRdiff > baseDiff + sm_LBtolerance && BRdiff < baseDiff - sm_HBtolerance)
  {
    sm_BRspeed+=IncDecAmount;
    driveBR(sm_BRspeed);
  }
  else
  {
    sm_BRspeed = baseSpeedR;
    driveBR(sm_BRspeed);
  }
  /*
  Serial.print(sm_FLspeed);
  Serial.print("\t");
  Serial.print(sm_FRspeed);
  Serial.print("\t");
  Serial.print(sm_BLspeed);
  Serial.print("\t");
  Serial.print(sm_BRspeed);
  Serial.print("\n");
  Serial.print(FLdiff);
  Serial.print("\t");
  Serial.print(FRdiff);
  Serial.print("\t");
  Serial.print(BLdiff);
  Serial.print("\t");
  Serial.print(BRdiff);
  Serial.print("\n\n");
  /*Serial.print(_FLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_FRightEncoderTicks);
  Serial.print("\t");
  Serial.print(_BLeftEncoderTicks);
  Serial.print("\t");
  Serial.print(_BRightEncoderTicks);
  Serial.print("\n");*/

  _preFLeftEncoderTicks = _FLeftEncoderTicks;
  _preFRightEncoderTicks = _FRightEncoderTicks;
  _preBLeftEncoderTicks = _BLeftEncoderTicks;
  _preBRightEncoderTicks = _BRightEncoderTicks;
}

// Interrupt service routines for the front left motor's quadrature encoder
void HandleFLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _FLeftEncoderBSet = digitalReadFast(c_FLeftEncoderPinB);   // read the input pin

  // and adjust counter + if A leads B
  #ifdef FLeftEncoderIsReversed
    _FLeftEncoderTicks -= _FLeftEncoderBSet ? -1 : +1;
  #else
    _FLeftEncoderTicks += _FLeftEncoderBSet ? -1 : +1;
  #endif
}

// Interrupt service routines for the front right motor's quadrature encoder
void HandleFRightMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _FRightEncoderBSet = digitalReadFast(c_FRightEncoderPinB);   // read the input pin

  // and adjust counter + if A leads B
  #ifdef FRightEncoderIsReversed
    _FRightEncoderTicks -= _FRightEncoderBSet ? -1 : +1;
  #else
    _FRightEncoderTicks += _FRightEncoderBSet ? -1 : +1;
  #endif
}

// Interrupt service routines for the back left motor's quadrature encoder
void HandleBLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _BLeftEncoderBSet = digitalReadFast(c_BLeftEncoderPinB);   // read the input pin

  // and adjust counter + if A leads B
  #ifdef BLeftEncoderIsReversed
    _BLeftEncoderTicks -= _BLeftEncoderBSet ? -1 : +1;
  #else
    _BLeftEncoderTicks += _BLeftEncoderBSet ? -1 : +1;
  #endif
}

// Interrupt service routines for the back right motor's quadrature encoder
void HandleBRightMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _BRightEncoderBSet = digitalReadFast(c_BRightEncoderPinB);   // read the input pin

  // and adjust counter + if A leads B
  #ifdef BRightEncoderIsReversed
    _BRightEncoderTicks -= _BRightEncoderBSet ? -1 : +1;
  #else
    _BRightEncoderTicks += _BRightEncoderBSet ? -1 : +1;
  #endif
}
