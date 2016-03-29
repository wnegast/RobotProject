
#include <Arduino.h>
#include <Stepper.h>
#include "testFunctions.h"
#include "dataLogger.h"
#include "Encoders.h"
#include "PixyNavigation.h"


//Define pin for switch Right
#define switchRight A1

//Define pin for Horizontal Stepper motor relay



void setup() {

  setupMotors();
  
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
      //driveOn(DirRotateRight, 10, 5, 25);
      //delay(2000);
      //driveOff();
      //GoThroughBox();
      /*digitalWrite(horRelay,LOW);
      horStepper.step(200);
      digitalWrite(horRelay, HIGH);
      GoThroughBox();
      //driveOn(DirForward, 15, 3, 5);
      /*
      //driveOn(DirLeft, 30, 5, 25); //Args:  Direction (DirForward, DirBackward, DirLeft, DirRight, DirRotateLeft, DirRotateRight)
                                                   //       Speed (min 1 to max 90)
                                                   //       Low Bound Tolerance (Keep this as is unless need to tweek.)
                                                   //       High Bound Tolerance (Keep this as is unless need to tweek.)
      //delay(3000);
      //driveOff();
  
  
      //Testing stepper moters
  
        //digitalWrite(horRelay,LOW);
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
    //CenterClaw();
    }
    CheckPixy();
    //CenterClaw();
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


void GoThroughBox()
{
  driveOn(DirForward, 15, 5, 25);
  delay(5000);
  driveOff();
}




