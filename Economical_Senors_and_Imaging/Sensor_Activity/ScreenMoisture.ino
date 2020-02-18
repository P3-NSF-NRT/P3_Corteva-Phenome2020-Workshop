/*
SparkFun Inventor’s Kit
Circuit 4B - Temperature Sensor

The LCD will display readings from a temperature sensor in degrees Celsius and Fahrenheit.

This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
This code is completely free for any use.

View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code
*/

#include <LiquidCrystal.h>                  //the liquid crystal library contains commands for printing to the display
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);    // tell the RedBoard what pins are connected to the display

float moisture = 0;                       //the temperature in Fahrenheit, calculated from the voltage

void setup() {

  lcd.begin(16, 2);                         //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                              //clear the display    
}

void loop() {
  
  moisture = analogRead(A0);   //convert the analog reading, which varies from 0 to 1023, back to a voltage value from 0-5 volts
  //convert the voltage to a temperature in degrees Fahrenheit

  lcd.clear();                              //clear the LCD

  lcd.setCursor(0,0);                       //set the cursor to the top left position
  lcd.print("Moisture: ");                 //print a label for the data
  lcd.print(moisture);                      //print the degrees Celsius
  

  
  delay(1000);                              //delay for 1 second between each reading (this makes the display less noisy)
}
