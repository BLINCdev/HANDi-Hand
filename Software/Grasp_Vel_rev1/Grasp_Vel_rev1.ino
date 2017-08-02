// Handi-Hand Individual Digit Velocity Controller
// Version 0.1
// Author: Michael (Rory) Dawson
//
// Objective: This sketch provides open loop control of each individual digit of the handi-hand by  mapping 
// the analog input from a thumb joystick to the angular velocity of an RC servomotor. The momentary switch on the 
// thumb joystick is used to switch control from one digit to another in a sequential fashion.
// 
// Definitions:
// d0: thumb rotation
// d1: thumb flex
// d2: index digit
// d3: middle digit
// d4: ring digit
// d5: baby digit
//
// grasp 0: standard precision pinch closed
// grasp 1: standard 3 jaw chuck closed
// grasp 2: column grip
// grasp 3: lateral grip
//
// References:
// arduino servo sweep example
// arduino thumb joystick example https://www.sparkfun.com/tutorials/272
// Structs: http://playground.arduino.cc/Code/Struct
// Sample compiler for initial testing: http://ideone.com/IPWy58

#include <Servo.h> 

// Create a servo object to control the servos
// int servo_number = 5;     // the number of servos in the hand - 1
Servo object[6]; // for some reason for pin 9 PWM to work this needs to be set to 6

// Define global array of structs to hold the parameters for each digit
struct digitParam {
  int pin;                 // the pwm pin that this digit will be assigned to
  int flex_limit;          // the position limit for flexion in microseconds
  int extend_limit;        // the position limit for extension in microseconds
  int min_vel;             // the minimum velocity
  int max_vel;             // the maximum velocity
  int pos;                 // the current target position
  int vel;                 // the current target velocity
} digit[6]; // need to make one more than the amount used because of some kind of bug in the compiler digit[5].flex_limit kept messing up the program (if i commented it out the servo would run fine)

// Assign joystick axis and buttons to pins on the arduino
// Also connect the joystick VCC to Arduino 5V, and joystick GND to Arduino GND.

const int VERT = 0; // analog
const int HORIZ = 1; // analog
const int SEL =  4; // digital
int prev_state = 1;
int servoID = 0;
int graspID = 0;
int chA_deadband = 50;  
int chA_neutral = 512;
int chB_deadband = 50; 
int chB_neutral = 512;

// This sketch outputs serial data at 9600 baud (open Serial Monitor to view).

void setup()
{
  // make the SEL line an input
  pinMode(SEL,INPUT_PULLUP);
  // turn on the pull-up resistor for the SEL line (see http://arduino.cc/en/Tutorial/DigitalPins)
  digitalWrite(SEL,HIGH);
  
  // Define thumb rotation parameters
  digit[0].pin = 3;              // the pwm pin that this digit will be assinged to
  digit[0].flex_limit = 2306;     // the position limit for flexion in microseconds
  digit[0].extend_limit = 1800;    // the position limit for extension in microseconds
  digit[0].min_vel = 0;           // the minimum velocity
  digit[0].max_vel = 60;         // the maximum velocity
  digit[0].pos = 1500;            // the current target position
  digit[0].vel = 0;               // the current target velocity
// digit[0] = {11 , 2500, 500, 0, 50, 0, 1};
  
  // Define thumb flexion parameters
  digit[1].pin = 5;              // the pwm pin that this digit will be assinged to
  digit[1].flex_limit = 1835;     // the position limit for flexion in microseconds
  digit[1].extend_limit = 1330;    // the position limit for extension in microseconds
  digit[1].min_vel = 0;           // the minimum velocity
  digit[1].max_vel = 60;         // the maximum velocity
  digit[1].pos = 1500;            // the current target position
  digit[1].vel = 0;               // the current target velocity

  // Define index finger flexion parameters
  digit[2].pin = 6;              // the pwm pin that this digit will be assinged to
  digit[2].flex_limit = 2353;     // the position limit for flexion in microseconds
  digit[2].extend_limit = 1000;    // the position limit for extension in microseconds
  digit[2].min_vel = 0;           // the minimum velocity
  digit[2].max_vel = 60;         // the maximum velocity
  digit[2].pos = 1500;            // the current target position
  digit[2].vel = 0;               // the current target velocity
  
  // Define middle finger flexion parameters
  digit[3].pin = 9;              // the pwm pin that this digit will be assinged to
  digit[3].flex_limit = 2198;     // the position limit for flexion in microseconds
  digit[3].extend_limit = 1016;    // the position limit for extension in microseconds
  digit[3].min_vel = 0;           // the minimum velocity
  digit[3].max_vel = 60;         // the maximum velocity
  digit[3].pos = 1500;            // the current target position
  digit[3].vel = 0;               // the current target velocity
  
  // Define ring finger flexion parameters
  digit[4].pin = 10;              // the pwm pin that this digit will be assinged to
  digit[4].flex_limit = 2109;     // the position limit for flexion in microseconds
  digit[4].extend_limit = 787;    // the position limit for extension in microseconds
  digit[4].min_vel = 0;           // the minimum velocity
  digit[4].max_vel = 60;         // the maximum velocity
  digit[4].pos = 1500;            // the current target position
  digit[4].vel = 0;               // the current target velocity
  
  // Define baby finger paramaters
  digit[5].pin = 11;              // the pwm pin that this digit will be assinged to
  digit[5].flex_limit = 1900;     // the position limit for flexion in microseconds     
  digit[5].extend_limit = 600;    // the position limit for extension in microseconds
  digit[5].min_vel = 0;           // the minimum velocity
  digit[5].max_vel = 60;         // the maximum velocity
  digit[5].pos = 1500;            // the current target position
  digit[5].vel = 0;               // the current target velocity

  // attaches the servo each pin to the corresponding servo object 
  object[0].attach(digit[0].pin);
  object[1].attach(digit[1].pin);
  object[2].attach(digit[2].pin);
  object[3].attach(digit[3].pin);
  object[4].attach(digit[4].pin);
  object[5].attach(digit[5].pin);
  
  // Set initial servo ID
  servoID = 1;
  
//  // set up serial port for output
//  Serial.begin(9600);
}

void loop() 
{
  int vertical, horizontal, select;
  
  // read all values from the joystick
  vertical = analogRead(VERT); // will be 0-1023
  horizontal = analogRead(HORIZ); // will be 0-1023
  select = digitalRead(SEL); // will be HIGH (1) if not pressed, and LOW (0) if pressed
  
  if (select == LOW && prev_state == HIGH)
  {
    graspID = graspID + 1;
     if (graspID > 3)
     {
     graspID = 0;
     }
  }
  // Update the switch state
  prev_state = select;
  
  if(vertical > chA_neutral + chA_deadband)
  {
    servoID = 1;
    digit[servoID].vel = map(vertical, chA_neutral, 1023, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos - digit[servoID].vel;
    if(digit[servoID].pos <= digit[servoID].extend_limit)
      {
        digit[servoID].pos = digit[servoID].extend_limit;
      }
    
    servoID = 5;
    digit[servoID].vel = map(vertical, chA_neutral, 1023, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos - digit[servoID].vel;
    if(digit[servoID].pos <= digit[servoID].extend_limit)
      {
        digit[servoID].pos = digit[servoID].extend_limit;
      }
    
    for(servoID = 2; servoID <= 4; servoID++)
    {
    digit[servoID].vel = map(vertical, chA_neutral, 1023, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos + digit[servoID].vel;
    if(digit[servoID].pos >= digit[servoID].flex_limit)
      {
        digit[servoID].pos = digit[servoID].flex_limit;
      }
    }
    
    servoID = 0;
    digit[servoID].vel = map(vertical, chA_neutral, 1023, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos + digit[servoID].vel;
    if(digit[servoID].pos >= digit[servoID].flex_limit)
      {
        digit[servoID].pos = digit[servoID].flex_limit;
      }
  
  }
  else if(vertical < chB_neutral - chB_deadband)
  {
    servoID = 1;
    digit[servoID].vel = map(vertical, chB_neutral, 0, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos + digit[servoID].vel;
    if(digit[servoID].pos >= digit[servoID].flex_limit)
      {
        digit[servoID].pos = digit[servoID].flex_limit;
      }
   
    servoID = 5;
    digit[servoID].vel = map(vertical, chB_neutral, 0, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos + digit[servoID].vel;
    if(digit[servoID].pos >= digit[servoID].flex_limit)
      {
        digit[servoID].pos = digit[servoID].flex_limit;
      }
   
    for(servoID = 2; servoID <= 4; servoID++)
    {
    digit[servoID].vel = map(vertical, chB_neutral, 0, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos - digit[servoID].vel;
    if(digit[servoID].pos <= digit[servoID].extend_limit)
      {
        digit[servoID].pos = digit[servoID].extend_limit;
      }
    }
    
    servoID = 0;
    digit[servoID].vel = map(vertical, chB_neutral, 0, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos - digit[servoID].vel;
    if(digit[servoID].pos <= digit[servoID].extend_limit)
      {
        digit[servoID].pos = digit[servoID].extend_limit;
      }
  }
  switch(graspID)
  {
    case 0:
      // GRIP 0 - Standard precision pinch closed
      digit[1].max_vel = 60;
      digit[0].pos = 1000;
      digit[1].flex_limit = 1305;
      digit[2].extend_limit = 657;
      digit[3].extend_limit = 600;
      digit[3].pos = digit[3].extend_limit + 100;
      digit[4].pos = digit[4].extend_limit + 100;
      digit[5].pos = digit[5].flex_limit - 100;
      break;
    
    case 1:
      // GRIP 1 - Standard 3 jaw chuck closed
      digit[1].max_vel = 60;
      digit[0].pos = 850;
      digit[1].flex_limit = 1938;
      digit[2].extend_limit = 1184;
      digit[3].extend_limit = 1321;
      digit[4].pos = digit[4].extend_limit + 100;
      digit[5].pos = digit[5].flex_limit - 100;
      break;
    
    case 2:
      // GRIP 2 - Column grip
      digit[1].max_vel = 11;
      digit[0].pos = 850;
      digit[1].flex_limit = 2150;
      digit[2].extend_limit = 748;
      digit[3].extend_limit = 1016;
      break;
      
    case 3:
      // GRIP 3 - Lateral grip
      digit[1].max_vel = 60;
      digit[1].flex_limit = 2150;
      digit[2].pos = digit[2].extend_limit + 100;
      digit[3].pos = digit[3].extend_limit + 100;
      digit[4].pos = digit[4].extend_limit + 100;
      digit[5].pos = digit[5].flex_limit - 100;
      break;
  }
  
//      object[servoID].writeMicroseconds(digit[servoID].pos);    // tell servo to go to position in variable 'pos'
    object[0].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos'
    object[1].writeMicroseconds(digit[1].pos);    // tell servo to go to position in variable 'pos' 
    object[2].writeMicroseconds(digit[2].pos);    // tell servo to go to position in variable 'pos' 
    object[3].writeMicroseconds(digit[3].pos);    // tell servo to go to position in variable 'pos' 
    object[4].writeMicroseconds(digit[4].pos);    // tell servo to go to position in variable 'pos' 
    object[5].writeMicroseconds(digit[5].pos);    // tell servo to go to position in variable 'pos'  
    delay(10);                       // waits 30ms for the servo to reach the position  
    
//  // print out the values
//  Serial.print(" target_pos: ");
//  Serial.print(digit[servoID].pos,DEC);
//  Serial.print(" servoID: ");
//  Serial.print(servoID,DEC);
//  Serial.print(" vertical: ");
//  Serial.print(vertical,DEC);
//  Serial.print(" horizontal: ");
//  Serial.print(horizontal,DEC);
//  Serial.print(" select: ");
//  if(select == HIGH)
//    Serial.println("not pressed");
//  else
//    Serial.println("PRESSED!");
}  
