#include <Arduino.h>

//  Let's Code Blacksburg
//  IR sensor test code
//       (version .2 -tweeks )
//  
//  This code reads the three IR line follower sensors 
//  (Left, Middle and Right) on pins analog pins A0, A1 
//   and A2,  and outputs L for black line or _ for 
//   light surface (no line) on the serial monitor.

int sensorLeft=0;
int sensorMiddle=0;
int sensorRight=0;

void setup()
{
    Serial.begin(9600);  // for outputting t-shooting codes
}

void loop()
{
  // Sensor readings give you a 1 if it sees the black line
  // and a 0 if it sees white.
  
  sensorLeft=digitalRead(A0);    // Left uses analog A0
  sensorMiddle=digitalRead(A1);  // Middle uses analog A1
  sensorRight=digitalRead(A2);   // Right uses analog A2
                               
  if (sensorLeft==1) {        
    Serial.print(" L ");
  }
  else {
    Serial.print(" _ ");
  }
 
  if (sensorMiddle==1) {        
    Serial.print(" L ");
  }
  else {
    Serial.print(" _ ");   
  }
  
  if (sensorRight==1) {        
    Serial.println(" L ");
  }
  else {
    Serial.println(" _ ");   
  }
  
}
