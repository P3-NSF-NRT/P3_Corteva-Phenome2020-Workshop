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
