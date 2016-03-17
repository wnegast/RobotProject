
#include <Arduino.h>
#include <Stepper.h>
#include "testFunctions.h"
#include "dataLogger.h"
#include "Encoders.h"
#include "PixyNavigation.h"



//Setup Vertical Stepper Motors
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// initialize the stepper library on pins 30 through 33:
Stepper vertStepper(stepsPerRevolution, 34, 35, 36, 37);
int vertStepCount; // number of steps the motor has taken

//Setup Horizontal Stepper Motor
// initialize the stepper library on pins 30 through 33:
Stepper horStepper(stepsPerRevolution, 30, 31, 32, 33);
int horStepCount; // number of steps the motor has taken

//Define pin for switch Right
#define switchRight A1

//Define pin for Horizontal Stepper motor relay
#define horRelay 48
#define vertRelay 49


void setup() {

  setupMotors();
  setupSteppers();
  setupEncoders();
  SetupPixy();
  Serial.begin(9600);

  //Set pin for starting switch right
  pinMode(switchRight, INPUT);

//Set pin for Horizontal Stepper motor relay to output
  pinMode(horRelay, OUTPUT);
  digitalWrite(horRelay, HIGH);

  //Set pin for Vertical Stepper motor relay to output
  pinMode(vertRelay, OUTPUT);
  digitalWrite(vertRelay, HIGH);
  
}


bool flag = false;

void loop() {


  
  if(analogRead(switchRight)>1015)
  {  
    
    
    if(!flag) // Runs this code once
    {
      
      flag = true;
      //driveOn(DirForward, 15, 3, 5);
      /*
      //driveOn(DirLeft, 30, 5, 25); //Args:  Direction (DirForward, DirBackward, DirLeft, DirRight, DirRotateLeft, DirRotateRight)
                                                   //       Speed (min 1 to max 90)
                                                   //       Low Bound Tolerance (Keep this as is unless need to tweek.)
                                                   //       High Bound Tolerance (Keep this as is unless need to tweek.)
      //delay(3000);
      //driveOff();
  
  
      //Testing stepper moters
  
       // digitalWrite(horRelay,LOW);
        //horStepper.step(750);
        //digitalWrite(horRelay, HIGH);
        
        digitalWrite(vertRelay,LOW);
        vertStepper.step(600);
        vertStepper.step(-600);
        digitalWrite(vertRelay, HIGH);
  
       // digitalWrite(horRelay,LOW);
        //horStepper.step(-750);
        //digitalWrite(horRelay, HIGH);
  
  
  
    
      // step one revolution  in one direction:
      //Serial.println("clockwise");
      //myStepper.step(stepsPerRevolution);
      //delay(500);
    
      // step one revolution in the other direction:
      //Serial.println("counterclockwise");
      //myStepper.step(-stepsPerRevolution);
      //delay(500);
      
    */
    }

    CheckPixy();
    /*Serial.print("Sonar1: ");
    Serial.println(sonar1.GetDistance());
    Serial.print("Sonar2: ");
    Serial.println(sonar2.GetDistance());*/
    
    
    
  }
  else
  {
    driveOff();
  }
  
}

void setupSteppers() {

    // set the speed at 60 rpm:
    vertStepper.setSpeed(60);
    vertStepCount = 0; // number of steps the motor has taken

    // set the speed at 60 rpm:
    horStepper.setSpeed(60);
    horStepCount = 0; // number of steps the motor has taken

}


