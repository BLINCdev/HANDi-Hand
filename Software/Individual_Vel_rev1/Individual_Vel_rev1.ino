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
  int chA_deadband = 50;  
  int chA_neutral = 512;
  int chB_deadband = 50; 
  int chB_neutral = 512;
  int max_vel = 50;

  int D0_Angle = 0;
  int D1P_Angle = 0;
  int D1D_Angle = 0;
  int D2P_Angle = 0;
  int D2I_Angle = 0;
  int D3P_Angle = 0;
  int D3I_Angle = 0;
  int D4P_Angle = 0;
  int D5P_Angle = 0;
  
  int D4I_Angle = -10; //dummy variable for spacing in GUI
  int D5I_Angle = -10; //dummy variable for spacing in GUI
    
  int D1_Voltage = 0;
  int D2_Voltage = 0;
  int D3_Voltage = 0;
  int D4_Voltage = 0;
  int D5_Voltage = 0;
  
    //Potentiometer Max and Min readouts for mapping to degrees of flexion:
  int D0_max = 175;
  int D0_min = 0;
  int D1P_max = 301;
  int D1P_min = 487;
  int D1D_max = 336;
  int D1D_min = 515;
  int D2P_max = 289;
  int D2P_min = 512;
  int D2I_max = 264;
  int D2I_min = 518;
  int D3P_max = 759;
  int D3P_min = 530;
  int D3I_max = 270;
  int D3I_min = 500;
  int D4P_max = 753;
  int D4P_min = 500;
  int D5P_max = 755;
  int D5P_min = 568;

// This sketch outputs serial data at 9600 baud (open Serial Monitor to view).

void setup()
{
  // make the SEL line an input
  pinMode(SEL,INPUT_PULLUP);
  // turn on the pull-up resistor for the SEL line (see http://arduino.cc/en/Tutorial/DigitalPins)
  digitalWrite(SEL,HIGH);
  
  // Define thumb rotation parameters
  digit[0].pin = 3;              // the pwm pin that this digit will be assinged to
  digit[0].flex_limit = 2400;     // the position limit for flexion in microseconds
  digit[0].extend_limit = 800;    // the position limit for extension in microseconds
  digit[0].min_vel = 0;           // the minimum velocity
  digit[0].max_vel = max_vel;         // the maximum velocity
  digit[0].pos = 2400;            // the current target position
  digit[0].vel = 0;               // the current target velocity
// digit[0] = {11 , 2500, 500, 0, 50, 0, 1};
  
  // Define thumb flexion parameters
  digit[1].pin = 5;              // the pwm pin that this digit will be assinged to
  digit[1].flex_limit = 2400;     // the position limit for flexion in microseconds
  digit[1].extend_limit = 600;    // the position limit for extension in microseconds
  digit[1].min_vel = 0;           // the minimum velocity
  digit[1].max_vel = max_vel;         // the maximum velocity
  digit[1].pos = 600;            // the current target position
  digit[1].vel = 0;               // the current target velocity

  // Define index finger flexion parameters
  digit[2].pin = 6;              // the pwm pin that this digit will be assinged to
  digit[2].flex_limit = 2400;     // the position limit for flexion in microseconds
  digit[2].extend_limit = 600;    // the position limit for extension in microseconds
  digit[2].min_vel = 0;           // the minimum velocity
  digit[2].max_vel = max_vel;         // the maximum velocity
  digit[2].pos = 2400;            // the current target position
  digit[2].vel = 0;               // the current target velocity
  
  // Define middle finger flexion parameters
  digit[3].pin = 9;              // the pwm pin that this digit will be assinged to
  digit[3].flex_limit = 2400;     // the position limit for flexion in microseconds
  digit[3].extend_limit = 600;    // the position limit for extension in microseconds
  digit[3].min_vel = 0;           // the minimum velocity
  digit[3].max_vel = max_vel;         // the maximum velocity
  digit[3].pos = 2400;            // the current target position
  digit[3].vel = 0;               // the current target velocity
  
  // Define ring finger flexion parameters
  digit[4].pin = 10;              // the pwm pin that this digit will be assinged to
  digit[4].flex_limit = 2400;     // the position limit for flexion in microseconds
  digit[4].extend_limit = 600;    // the position limit for extension in microseconds
  digit[4].min_vel = 0;           // the minimum velocity
  digit[4].max_vel = max_vel;         // the maximum velocity
  digit[4].pos = 2400;            // the current target position
  digit[4].vel = 0;               // the current target velocity
  
  // Define baby finger paramaters
  digit[5].pin = 11;              // the pwm pin that this digit will be assinged to
  digit[5].flex_limit = 2400;     // the position limit for flexion in microseconds     
  digit[5].extend_limit = 600;    // the position limit for extension in microseconds
  digit[5].min_vel = 0;           // the minimum velocity
  digit[5].max_vel = max_vel;         // the maximum velocity
  digit[5].pos = 600;            // the current target position
  digit[5].vel = 0;               // the current target velocity

  // attaches the servo each pin to the corresponding servo object 
  object[0].attach(digit[0].pin);
  object[1].attach(digit[1].pin);
  object[2].attach(digit[2].pin);
  object[3].attach(digit[3].pin);
  object[4].attach(digit[4].pin);
  object[5].attach(digit[5].pin);
  
  // Set initial servo ID
  servoID = 0;
  
  // set up serial port for output
  Serial.begin(9600);
  // set up GUI for output:
  gBegin(35001);
  

}

void loop() 
{
  //**** Main update call for the guino:
  guino_update();
  
  int vertical, horizontal, select;
  
  // read all values from the joystick
  vertical = analogRead(VERT); // will be 0-1023
  horizontal = analogRead(HORIZ); // will be 0-1023
  select = digitalRead(SEL); // will be HIGH (1) if not pressed, and LOW (0) if pressed
  
  if (select == LOW && prev_state == HIGH)
  {
    servoID = servoID + 1;
     if (servoID > 5)
     {
     servoID = 0;
     }
  }
  // Update the switch state
  prev_state = select;
  
  if(vertical > chA_neutral + chA_deadband)
  {
    digit[servoID].vel = map(vertical, chA_neutral, 1023, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos + digit[servoID].vel;
    if(digit[servoID].pos >= digit[servoID].flex_limit)
      {
        digit[servoID].pos = digit[servoID].flex_limit;
      }
  }
  else if(vertical < chB_neutral - chB_deadband)
  {
    digit[servoID].vel = map(vertical, chB_neutral, 0, digit[servoID].min_vel, digit[servoID].max_vel);
    digit[servoID].pos = digit[servoID].pos - digit[servoID].vel;
    if(digit[servoID].pos <= digit[servoID].extend_limit)
      {
        digit[servoID].pos = digit[servoID].extend_limit;
      }
  }

      object[servoID].writeMicroseconds(digit[servoID].pos);    // tell servo to go to position in variable 'pos'
//    object[0].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos'
//    object[1].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos' 
//    object[2].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos' 
//    object[3].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos' 
//    object[4].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos' 
//    object[5].writeMicroseconds(digit[0].pos);    // tell servo to go to position in variable 'pos'  
    delay(5);                       // waits 30ms for the servo to reach the position  
// Read in the values from each of the potentiometers and FSRs:
//  int D0_Voltage = analogRead(A2);
//  int D1P_Voltage = analogRead(A3);
//  int D1D_Voltage = analogRead(A4);
//  int D2P_Voltage = analogRead(A5);
//  int D2I_Voltage = analogRead(A6);
//  int D3P_Voltage = analogRead(A7);
//  int D3I_Voltage = analogRead(A8);
//  int D4P_Voltage = analogRead(A9);
//  int D5P_Voltage = analogRead(A10);
//  D1_Voltage = analogRead(A11);
//  D2_Voltage = analogRead(A12);
//  D3_Voltage = analogRead(A13);
//  D4_Voltage = analogRead(A14);
//  D5_Voltage = analogRead(A15);
  
  // Map the Potentiometer voltages to % flexion:
//  D0_Angle = map(D0_Voltage,D0_min,D0_max,100,0);
//  D1P_Angle = map(D1P_Voltage,D1P_min,D1P_max,100,0);
//  D1D_Angle = map(D1D_Voltage,D1D_min,D1D_max,100,0);
//  D2P_Angle = map(D2P_Voltage,D2P_min,D2P_max,100,0);
//  D2I_Angle = map(D2I_Voltage,D2I_min,D2I_max,100,0);
//  D3P_Angle = map(D3P_Voltage,D3P_min,D3P_max,100,0);
//  D3I_Angle = map(D3I_Voltage,D3I_min,D3I_max,100,0);
//  D4P_Angle = map(D4P_Voltage,D4P_min,D4P_max,100,0);
//  D5P_Angle = map(D5P_Voltage,D5P_min,D5P_max,100,0);
  

  // print out the values
//  Serial.print(servoID,DEC);
//  Serial.print(",");
//  Serial.print(digit[servoID].pos,DEC);
//  Serial.print(",");
//  Serial.print(D0_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D1P_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D1D_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D2P_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D2I_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D3P_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D3I_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D4P_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D5P_Angle,DEC);
//  Serial.print(",");
//  Serial.print(D1_Voltage,DEC);
//  Serial.print(",");
//  Serial.print(D2_Voltage,DEC);
//  Serial.print(",");
//  Serial.print(D3_Voltage,DEC);
//  Serial.print(",");
//  Serial.print(D4_Voltage,DEC);
//  Serial.print(",");
//  Serial.println(D5_Voltage,DEC);
 
// Sent the values to the GUI:
gUpdateValue(&servoID);
gUpdateValue(&digit[servoID].pos);
//gUpdateValue(&D0_Angle);
//gUpdateValue(&D1P_Angle);
//gUpdateValue(&D1D_Angle);
//gUpdateValue(&D2P_Angle);
//gUpdateValue(&D2I_Angle);
//gUpdateValue(&D3P_Angle);
//gUpdateValue(&D3I_Angle);
//gUpdateValue(&D4P_Angle);
//gUpdateValue(&D5P_Angle);
//gUpdateValue(&D1_Voltage);
//gUpdateValue(&D2_Voltage);
//gUpdateValue(&D3_Voltage);
//gUpdateValue(&D4_Voltage);
//gUpdateValue(&D5_Voltage);
}  

//Set up the guino interface:
void gInit()
{
//  gAddLabel("SERVO POSITION",1);
//  gAddSpacer(1);
  
//  gAddColumn();
//  gAddLabel("THUMB ROTATION",1);
//  gAddSpacer(1);
//  gAddMovingGraph("DO", 0, 100, &D0_Angle, 10);
//  gAddSlider(600,2400,"D0 Servo Pos",&digit[0].pos);
//  
//  gAddColumn();
//  gAddLabel("THUMB FLEXION",1);
//  gAddSpacer(1);
//  gAddMovingGraph("D1P",0,100,&D1P_Angle,10);
//  gAddMovingGraph("D1D",0,100,&D1D_Angle,10);
//  gAddMovingGraph("FSR - D1",0,1000,&D1_Voltage,10);
//  gAddSlider(600,2400,"D1 Servo Pos",&digit[1].pos);  
//  
//  gAddColumn();
//  gAddLabel("INDEX",1);
//  gAddSpacer(1);
//  gAddMovingGraph("D2P",0,100,&D2P_Angle,10);
//  gAddMovingGraph("D2I",0,100,&D2I_Angle,10);
//  gAddMovingGraph("FSR - D2",0,1000,&D2_Voltage,10);
//  gAddSlider(600,2400,"D2 Servo Pos",&digit[2].pos);
// 
//  gAddColumn();
//  gAddLabel("MIDDLE",1);
//  gAddSpacer(1);
//  gAddMovingGraph("D3P",0,100,&D3P_Angle,10);
//  gAddMovingGraph("D3I",0,100,&D3I_Angle,10);
//  gAddMovingGraph("FSR - D3",0,1000,&D3_Voltage,10); 
//  gAddSlider(600,2400,"D3 Servo Pos",&digit[3].pos);
//  
//  gAddColumn();
//  gAddLabel("RING",1);
//  gAddSpacer(1);
//  gAddMovingGraph("D4P",0,100,&D4P_Angle,10);
//  gAddMovingGraph("D4I",0,100,&D4I_Angle,10);
//  gAddMovingGraph("FSR - D4",0,1000,&D4_Voltage,10);
//  gAddSlider(600,2400,"D4 Servo Pos",&digit[4].pos);
//  
//  gAddColumn();
//  gAddLabel("PINKY",1);
//  gAddSpacer(1);
//  gAddMovingGraph("D5P",0,100,&D5P_Angle,10);
//  gAddMovingGraph("D5I",0,100,&D5I_Angle,10);
//  gAddMovingGraph("FSR - D5",0,1000,&D5_Voltage,10);
//  gAddSlider(600,2400,"D5 Servo Pos",&digit[5].pos);
  gAddSlider(-1,7,"SERVO ID",&servoID);
  
  gSetColor(120,120,120);
  
}

// Method called everytime a button has been pressed in the interface.
void gButtonPressed(int id)
{
 
}

void gItemUpdated(int id)
{

}

