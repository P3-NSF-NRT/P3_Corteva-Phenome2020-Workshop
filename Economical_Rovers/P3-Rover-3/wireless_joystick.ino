/* This sketch is mainly taken from:
 *************************
 *  3.l_Remote_Control_SAMD21.ino
   Full Remote Control SAMD21 Example
   Written by: Ho Yun Bobby Chan
   Date: 2/15/19
   SparkFun Electronics

   license: Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0)
   Do whatever you'd like with this code, use it for any purpose.
   Please attribute and keep this license.

   This is example code for the Wireless Joystick with SAMD21. Any character entered through the
   Serial Monitor or when a condition statement is satisfied will be sent to the hardware UART pins.
   Assuming that you have a pair of XBees Series 1 modules (or Series 3 modules configured with 802.15.4 protocol) on the
   same channel, a character will be transmitted wirelessly between the XBees. The receiving
   XBee will then pass the character to the an ATmega328P microcontroller to move the robot forward.

   Pressing down on D2 (if you soldered the joystick on the right or a button) will check
   the joystick on the left. A character will be transmitted when moving the joystick.

       up = forward
       right = forward turn right
       down = reverse
       left = forward turn left

   When D2 is not being pressed, a character will be sent to stop the motors.

   Note: You may need to connect A5 to the XBee Series 3's reset pin on the Wireless Joystick
   for certain XBee Series 3 modules. For more details, check out the xbee3_RESET() function.
 *************************
 * The code was modified and changed slightly to better fit the needs for this project.
 * 
 * The code functions close to the above description with some added functions like:
 * Pressing the right trigger button centers the servo
 * 
*/

#define FORWARD_REVERSE_JOYSTICK A3   // Pin used for left joystick's y-component
#define TURN_JOYSTICK A2   // Pin used for left joystick x-component

int prev_buttonACCELERATE_State;    //value to store the previous state of the button press
int current_buttonACCELERATE_State; //value to store the current state of the button press
#define ACCELERATE_BUTTON 2 // Pin used for right trigger
#define TRIGGER_BUTTON 3 // Trigger button to strighten tires
int center; //variable for centering servo
// We'll store the the analog joystick values here
int16_t forward_reverse_Stick_value;
int16_t turnStick_value;

char c_data;//send values through the serial monitor for debugging

//LED to check if the LED is initialized.
const int status_LED = 13;

//needed for certain XBee Series 3 modules
#define xbee_reset A5

void setup() {

  SerialUSB.begin(9600);// Initialize Serial Monitor for DEBUGGING

  //Uncomment this if you want to wait until the serial monitor is open.
  //while (!SerialUSB); //Wait for Serial Monitor to Open

  SerialUSB.println("Wireless Joystick Controller Initializing");
  Serial1.begin(9600); // Start serial communication with XBee at 9600 baud

  xbee3_RESET();//in case XBee3 has issues initializing, hardware reset

  pinMode(ACCELERATE_BUTTON, INPUT_PULLUP); // Enable pullup resistor for accelerate button D2
  pinMode(TRIGGER_BUTTON, INPUT_PULLUP); //Button for centering servo
  //Status LED to see if the Controller is initializing
  pinMode(status_LED, OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(status_LED, HIGH);//set Status LED on
    delay(50);
    digitalWrite(status_LED, LOW); //set Status LED off
    delay(50);
  }
  SerialUSB.println("Wireless Joystick Controller's XBee Ready to Communicate");
  delay(10);
}//end setup

void loop() {

  //initialize variables to read buttons
  current_buttonACCELERATE_State = digitalRead(ACCELERATE_BUTTON);
  center = digitalRead(TRIGGER_BUTTON);
  /***button1state
    - LOW or 0 means pressed
    - HIGH or 1 means not pressed
   ****/
  //Store values read joystick
  forward_reverse_Stick_value = analogRead(FORWARD_REVERSE_JOYSTICK);
  turnStick_value = analogRead(TURN_JOYSTICK);
  //send commands via serial monitor for testing here
  if (SerialUSB.available()) {
    c_data = SerialUSB.read();//take character from serial monitor and store in variable

    Serial1.print(c_data);//send to XBee

    //echo back what was sent to serial monitor
    SerialUSB.println("Sending Character Here, ");
    SerialUSB.println(c_data);

  }

  if(center == LOW){
    SerialUSB.println("Center");
    Serial1.print('c');
  }
  
  if (current_buttonACCELERATE_State == LOW) {
    SerialUSB.println("Accelerate Button has been pressed!");

    if (forward_reverse_Stick_value > 1000) {
      SerialUSB.println("Forward");
      Serial1.print('w');//transmit via XBees on the same channel
      digitalWrite(status_LED, HIGH); //turn ON Status LED
    }
    else if ( turnStick_value < 10) {
     SerialUSB.println("Turn Right");
      Serial1.print('a');
      digitalWrite(status_LED, HIGH); //turn ON Status LED
    }
    else if (forward_reverse_Stick_value < 10) {

      SerialUSB.println("Reverse");
      Serial1.print('s');
      digitalWrite(status_LED, HIGH); //turn ON Status LED
    }
    else if (turnStick_value > 1000) {
      SerialUSB.println("Turn Left");
      Serial1.print('d');
      digitalWrite(status_LED, HIGH); //turn ON Status LED
    }
    else {
      SerialUSB.println("Brake");
      Serial1.print('x');
      digitalWrite(status_LED, HIGH);
    }
    
    }
    //Debug left analog joystick here
    //Boundaries vary depending on the joystick's read value
    //You may need to adjust the values in the condition statements to calibrate
    //Additional condition statements will need to be written for pivoting
    //and turning in reverse
    //SerialUSB.print("forward_reverse_Stick_value  = "); //~1023 up, ~7-9 down
    //SerialUSB.println(forward_reverse_Stick_value);
    //SerialUSB.println("turnStick_value = "); //~1023 left, ~5-6 right
    //SerialUSB.println(turnStick_value);

  else {//current_buttonACCELERATE_State == HIGH
    //if not sending a command to drive, automatically have the robot stop moving
      //SerialUSB.println("Stop");
      digitalWrite(status_LED, HIGH); //turn ON Status LED
      Serial1.print('x');

  }
  prev_buttonACCELERATE_State = current_buttonACCELERATE_State; //save current state

  //delay(100); //add short delay for LED for feedback, this can be commented out if it is affecting performance
  digitalWrite(status_LED, LOW); //turn OFF Status LED
}//end loop

void xbee3_RESET() {

  //HARDWARE RESET
  /*
    - XBee Series 3 Hardware Reference Manual
    - Pg 31 Power Supply Design recommends decoupling capacitor between Vcc and GND.
      Tested with 10uF capacitor and without. This was not necessary.
    - Pg 60 Brown Out Detection. This is REQUIRED. Add a jumper between the XBee's Reset and A5
    https://www.digi.com/resources/documentation/digidocs/pdfs/90001543.pdf

    - Power cycle XBee Series 3 by grounding RESET Pin to avoid dicontinuities in ramp up and brown out detection
    https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2017/06/14/rmu_e203_avdd_ramp-j176

    - Minimum Time to Force Reset:
    - EFM32 devices = 50ns; EFM32PG/JG: Pearl and Jade Gecko =100ns
    https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2016/07/22/minimum_reset_holdt-PglD
  */
  pinMode(xbee_reset, OUTPUT);
  digitalWrite(xbee_reset, HIGH);
  delayMicroseconds(1);
  digitalWrite(xbee_reset, LOW);
  delayMicroseconds(1);
  digitalWrite(xbee_reset, HIGH);
}
