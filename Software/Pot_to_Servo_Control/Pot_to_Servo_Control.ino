/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald <http://www.arduino.cc/en/Tutorial/Knob>

 modified on 1 Aug 2017
 by Dylan Brenneis <https://blinclab.ca/about/dylan-brenneis-undergraduate/>
*/

#include <Servo.h>

Servo D0;          // create servo object to control servo
int Analog0 = 0;   // analog pin used to connect the potentiometer
int posD0;         // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  D0.attach(3);    // attaches the servo on pin 3 to the servo object
}

void loop() {
  posD0 = analogRead(Analog0);            // reads the value of the potentiometers (value between 0 and 1023)
  posD0 = map(posD0, 0, 1023, 0, 180);    // scale it to use it with the servo (value between 0 and 180)
  D0.write(posD0);                        // sets the servo position according to the scaled value
  
  
  Serial.print(posD0);
  Serial.print('  ');
  Serial.println(Analog0); 
  delay(15);                              // waits 15 ms for the servo to get there
}
