# Rover 1

## Wiring

Here is an overveiw of how the Arduino/Redboard is wired using the motor driver provided in the parts list below.

![](../Rover_Images/redboard_rover_basic.png)

For a more detailed description of how to wire the redboard you can refer to page 96 of the ["SparkFun Inventor's Kit"](https://github.com/P3-NSF-NRT/P3_Corteva-Phenome2020-Workshop/blob/master/SIK%20v4.1%20Book%202019%20WEB.pdf) manual or the [SparkFun](https://www.sparkfun.com/) website.

## Code

The code used for this rover is mainly provided by the example code provided with the Inventor Kit. There are a few areas that the code was modified to better fit our use and design.

**Note:** Code mentioned here is used with [Arduino IDE](https://www.arduino.cc/en/main/software) which can be used online or through a program downloaded to a computer. **This is not Python or R code, it is Arduino IDE code**

``r
/*
SparkFun Inventor’s Kit
Circuit 5B - Remote Control Robot

Control a two wheeled robot by sending direction commands through the serial monitor.
This sketch was adapted from one of the activities in the SparkFun Guide to Arduino.
Check out the rest of the book at
https://www.sparkfun.com/products/14326

This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
This code is completely free for any use.

View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code
*/

//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 13;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 12;           //control pin 2 on the motor driver for the right motor
const int PWMA = 11;           //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 10;          //speed control pin on the motor driver for the left motor
const int BIN2 = 9;           //control pin 2 on the motor driver for the left motor
const int BIN1 = 8;           //control pin 1 on the motor driver for the left motor

int switchPin = 7;            //switch to turn the robot on and off

const int driveTime = 60;     //this is the number of milliseconds that it takes the robot to drive 1 inch (or unit of choice)
                              //you can modify this number to 'calibrate' the motors to go about 1 inch when fed a value of 1
                              //The greater the number the longer the motors run, so if the robot does not move far enough increase
                              //this value, and if it goes too far decrease this number
                              
const int rightturnTime = 5.3;   //this is the number of milliseconds that it takes to turn the robot 1 degree to the right
                                 //you can modify this number to 'calibrate' the motors to go about 1 degree when fed a value of 1
                                 //The greater the number the longer the motors run, so if the robot does not move far enough increase
                                 //this value, and if it goes too far decrease this number

const int leftturnTime = 6;    //This is the same as the rightturnTime but for turning left
                               //There are two values because it allows the user to more accurately 'calibrate' the robot
                               //The same could be done for forwards and backwards if necessary
                               
                               //Note: these numbers will vary a little bit based on how you mount your motors, the friction of the
                               //surface that your driving on, and fluctuations in the power to the motors.
                               //You can change the driveTime and turnTime to make them more accurate

String botDirection;           //the direction that the roobt will drive in (this change which direction the two motors spin in)
String distance;               //the distance to travel in each direction

/********************************************************************************/
void setup()
{
  pinMode(switchPin, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);           //begin serial communication with the computer

  //prompt the user to enter a command
  Serial.println("Enter a diretion followed by a distance.");
  Serial.println("f = forward, b = backward, r = turn right, l = turn left");
  Serial.println("Example command: f 50");
}

/********************************************************************************/
/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, LOW);                         //set pin 1 to high
    digitalWrite(AIN2, HIGH);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, HIGH);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}
void loop()
{
  if(digitalRead(7) == LOW)
  {                                                     //if the switch is in the ON position 
    if (Serial.available() > 0)                         //if the user has sent a command to the RedBoard
    {
      botDirection = Serial.readStringUntil(' ');       //read the characters in the command until you reach the first space
      distance = Serial.readStringUntil(' ');           //read the characters in the command until you reach the second space
  
      //print the command that was just received in the serial monitor
      Serial.print(botDirection);                       
      Serial.print(" ");                                
      Serial.println(distance.toInt());                 
  
      if(botDirection == "f")                          //if the entered direction is forward                          
      {
        rightMotor(200);                                //drive the right wheel forward
        leftMotor(200);                                 //drive the left wheel forward
        delay(driveTime * distance.toInt());            //drive the motors long enough travel the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
        delay(500);
      }
      else if(botDirection == "b")                     //if the entered direction is backward  
      {
        rightMotor(-200);                               //drive the right wheel backward
        leftMotor(-200);                                //drive the left wheel backward
        delay(driveTime * distance.toInt());            //drive the motors long enough travel the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
        delay(500);
      }
      else if(botDirection == "r")                      //if the entered direction is right  
      {
        rightMotor(-200);                               //drive the right wheel forward
        leftMotor(255);                                 //drive the left wheel forward
        delay(rightturnTime * distance.toInt());             //drive the motors long enough turn the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
        delay(500);
      }
      else if(botDirection == "l")                    //if the entered direction is left  
      { 
        rightMotor(255);                                //drive the right wheel forward
        leftMotor(-200);                                //drive the left wheel forward
        delay(leftturnTime * distance.toInt());       //drive the motors long enough turn the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
        delay(500);
      }
    }
  }
  else
  {
     rightMotor(0);                                  //turn the right motor off
     leftMotor(0);                                   //turn the left motor off 
     delay(500);
  }
}
``

## Body


## Parts List

Item  | Cost  
------|-------
[RedBoard](https://www.sparkfun.com/products/13975)| 19.95
[Breadboard](https://www.sparkfun.com/products/12002) | 4.95
[Basic Motor Driver](https://www.sparkfun.com/products/14450) | 5.45
[Tires](https://www.sparkfun.com/products/13259) | 2.95
[Switch](https://www.sparkfun.com/products/102)|1.50
[Basic Motors](https://www.sparkfun.com/products/13302)|4.11
[USB Cable for RedBoard](https://www.sparkfun.com/products/11301) | 5.10
[Wires](https://www.sparkfun.com/products/11026)|2.25
