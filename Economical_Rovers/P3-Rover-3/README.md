# Rover 3

![](../Rover_Images/Rover_3.jpg)

This section covers a more advanced rover. This rover takes a similar concept from [Rover 1](../P3-Rover-1) and [Rover 2](../P3-Rover-2) in using arduino board and two motors to control a small rover. This project takes it a step further by adding a servo and wireless control. It is highly suggested to look at the linked SparkFun guides. These were heavily used for help in coding but some modifications were made to fit our project. It is also suggested to have some knowledge in how to solder, install drivers on a computer and troubleshoot. There are many components to this rover which leads to many possible errors when things are not working. Now let's dig into the details!

## Wiring

The wiring for this project involves the two main components, the controller and the Arduino controlling the motors. Below there are some guides listed with how to assemble/hookup these things. There is also a component mentioned called an XBee Module and this is not a part that required wiring but it is attached to both components to make the setup wireless. These XBee modules need to be configured and SparkFun has some [guides](https://learn.sparkfun.com/tutorials/exploring-xbees-and-xctu?_ga=2.60464681.545162938.1581579323-794898768.1581579323) about how to do this but it is highly suggested to read through the other guides first to make sure you configure the XBees correctly.

Note: [SparkFun](https://www.sparkfun.com/) provides a lot of guides that were very helpful when I ran into problems. They have guides to help install the drivers necessary to run all to parts of this project you just make have to look for some. I found most of them were linked or related in the hookup guides. 

### Wireless Controller

The wireless controller we used was ordered off [SparkFun](https://www.sparkfun.com/) and they have a [hookup guide](https://learn.sparkfun.com/tutorials/wireless-joystick-hookup-guide) for this product. For this portion of the project some soldering is required. For our rover we also chose the dual joystick design. It is also important to pay attention to the requirements when using an XBee 3 module.

### Motor Driver Shield

For this project a wireless motor driver shield was used. This is a board that is added to the top of a RedBoard or Arduino and has a location to attach an XBee Module. The shield also has locations to either solder on wires for motors or attach them with the pin wires we used on our other rovers. Another benefit of using this shield is it breaks down the analog pins on a regular Arduino into ground (GND), power (PWR), and signal (SIG) which makes it significantly easier to hookup servos to the board. This shield also has a [guide](https://learn.sparkfun.com/tutorials/wireless-motor-driver-shield-hookup-guide?_ga=2.58641130.1360432469.1581004979-1999190486.1581004979) and this is where most of the code for the motor shield came from.

## Code

There are two different codes needed for this project. The first portion of code is what will be loaded onto the [motor driver shield](./motordriver.ino/motordriver.ino). There are some data values in this code that can be modified to improve the performance of your own rover.

#### Motor Driver Shield Code
```
 /*
 * This code has been mainly taken from:
 * SparkFun Ludus ProtoShield Example Code
 * SparkFun Electronics
 * Nick Poole 2015
 * 
 * There have been some changes to fit the specific project/scope of this rover
 * This code controls two motors and a servo to control a vehicle
 * The servo is cordinated with the motors for turning
 * 
 * Many of the values may need modified to work with your own project
*/

#include <Servo.h> 
Servo swivel;
int pwm_a = 3;   // Channel A speed
int pwm_b = 6;   // Channel B speed
int dir_a0 = 4;  // Channel A direction 0
int dir_a1 = 5;  // Channel A direction 1
int dir_b0 = 7;  // Channel B direction 0
int dir_b1 = 8;  // Channel B direction 1
char inbit; // A place to store serial input
int swivelpos = 90; // Servo position
int fordrivetime = 5; //delay time for forward driving
int revdrivetime = 5; //delay time for driving in reverse
int rightturntime = 50; //delay time for turning right
int leftturntime = 50; //delay time for turning left
void setup()
{
  Serial.begin(9600); // Start serial communication
  swivel.attach(11); // Attach servo to pin 11
  swivel.write(swivelpos); 
  pinMode(pwm_a, OUTPUT);  // Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a0, OUTPUT);
  pinMode(dir_a1, OUTPUT);
  pinMode(dir_b0, OUTPUT);
  pinMode(dir_b1, OUTPUT);
}
void loop()
{ 
if(Serial.available()){ // Wait for serial input
  inbit = Serial.read();
  switch(inbit){ // Switch based on serial in
    case 'w': // Move Forward
      servoC();
      delay(10);
      forward(200);
      delay(fordrivetime);
      brake();
      break;
    case 's': // Move Backward
      servoC();
      delay(10);
      reverse(200);
      delay(revdrivetime);
      brake();
      break;
    case 'a': // Spin Left in place
      servoC();
      delay(10);
      servoL();
      spinL(200);
      delay(leftturntime);
      brake();
      break;
    case 'd': // Spin Right in place
      servoC();
      delay(10);
      servoR();
      spinR(200);
      delay(rightturntime);
      brake();
      break;
    case 'x': // Shut off motors w/o braking
      shutoff();
      break;
    case 'z': // Spin servo (on pin 11) left
      servoL();
      break;
    case 'c': // Spin servo (on pin 11) right
      servoC();
      break;
    case 'v':
      servoR();
      break;
    }
  }  
}

void forward(int speed) // Move Forward
{ 
digitalWrite(dir_a0, 1);
digitalWrite(dir_a1, 0);
digitalWrite(dir_b0, 0);
digitalWrite(dir_b1, 1);
analogWrite(pwm_a, speed); 
analogWrite(pwm_b, speed); 
}
void reverse(int speed) // Move Backward 
{
digitalWrite(dir_a0, 0);
digitalWrite(dir_a1, 1);
digitalWrite(dir_b0, 1);
digitalWrite(dir_b1, 0);
analogWrite(pwm_a, speed); 
analogWrite(pwm_b, speed); 
}
void spinL(int speed) // Spin Left in place
{ 
digitalWrite(dir_a0, 0);
digitalWrite(dir_a1, 1);
digitalWrite(dir_b0, 0);
digitalWrite(dir_b1, 1);
analogWrite(pwm_a, speed/3); 
analogWrite(pwm_b, speed); 
}
void spinR(int speed) // Spin Right in place
{ 
digitalWrite(dir_a0, 1);
digitalWrite(dir_a1, 0);
digitalWrite(dir_b0, 1);
digitalWrite(dir_b1, 0);
analogWrite(pwm_a, speed); 
analogWrite(pwm_b, speed/3);
}
void brake() // Short brake
{ 
digitalWrite(dir_a0, 1);
digitalWrite(dir_a1, 1);
digitalWrite(dir_b0, 1);
digitalWrite(dir_b1, 1);
analogWrite(pwm_a, 0); 
analogWrite(pwm_b, 0);
}
void shutoff() // Stop Motors w/o braking
{ 
digitalWrite(dir_a0, 0);
digitalWrite(dir_a1, 0);
digitalWrite(dir_b0, 0);
digitalWrite(dir_b1, 0);
analogWrite(pwm_a, 0); 
analogWrite(pwm_b, 0);
}
void servoL() // Spin servo (on pin 11) left 
{
  if(swivelpos>10){
    swivelpos = swivelpos-60;
    swivel.write(swivelpos);
  }
}
void servoR() // Spin servo (on pin 11) right
{
  if(swivelpos<170){
    swivelpos = swivelpos+70;
    swivel.write(swivelpos);
  }
}
void servoC() //Resets the servo to original position
{
 if(swivelpos != 90){
   swivelpos = 90;
   swivel.write(swivelpos);
 }
}
```


## Body

## Parts List



https://learn.sparkfun.com/tutorials/wireless-rc-robot-with-arduino-and-xbees/experiment-1-sending-and-receiving-a-signal
