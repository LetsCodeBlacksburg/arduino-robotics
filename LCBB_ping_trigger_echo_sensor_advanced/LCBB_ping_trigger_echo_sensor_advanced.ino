//  Let's Code Blacksburg
//  Ping trigger/echo ultrasonic distance sensor code
//       (version .1 -tweeks )
//  
//  This code gives a more advanced example of a dual-pin echo sensor
//  in conjunction with a panning servo "neck" to be interleved with 
//  a moving robot or vehicle with the purpose of collision avoidance.
//  There are distance thresholds collisionwarn and collisiondist to
//  use for things like slow vs fast motor driven response.
//  Be sure to watch the serial montior to see how all these things are
//  used together.

#include <Arduino.h>

// ============== Using Pins =================
// 13 - Eyes (Ultrasonic Sensor) Echo
// 12 - Eyes (Ultrasonic Sensor) Trigger
//  9 - Eyes neck-servo 

//Configure IR receiver

//Servo for eyes "neck"
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos = 0;    // variable to store the servo position 

//Configure "Eyes" Ping Sensor
const int pingPin = 12;
const int echoPin = 13;
const long collisionwarn = 10;
const long collisiondist = 6;
long distance = -1;
 
//================== SETUP ========================
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);  // for outputting t-shooting codes
} 
 

//========================= pingsweep() ===========================
long pingsweep() 
{ 
  long x = -1;

  //Pan Ping Sensor Eyes Left to Right
  for(pos = 35; pos < 145; pos += 5)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 5 degrees 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 5ms for the servo to reach the position 
    // Every 15-20 degrees capture IR-remote data and let out a ping, 
    // and return value if > -1 && <collitiondist (inches)
    if(pos == 35 || pos == 55 || pos == 75 || pos == 90 || pos == 105 || pos == 125 || pos == 145) 
      {
     //getIR();                        // While servo is moving, get the look for IR-remote data 
      x = pingdist();
      if(x > -1 && x < collisionwarn ){
        myservo.write(90);             // if object detected, snap to 90 degrees (useful)
        Serial.println("!! PROXIMITY WARNING !!");        
        return(x);
       }
     }
  } 
  
  //Pan Ping Sensor Eyes Right to Left
  for(pos = 145; pos>=35; pos-=5)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);             // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 5ms for the servo to reach the position 
    //Every 15-20 degrees let out a ping, and return value if > -1 && <collitiondist (inches)
    if(pos == 35 || pos == 55 || pos == 75 || pos == 90 || pos == 105 || pos == 125 || pos == 145) 
      {
      //getIR();                        // While servo is moving, get the look for IR-remote data 
      x = pingdist();
      if(x > -1 && x < collisionwarn ){
        myservo.write(90);
        Serial.println("!! PROXIMITY WARNING !!");        
        return(x);
       }
     }
  }
  
  myservo.write(90);
  return(x);  
} 


//======================== pingdist() ==============================
long pingdist()
{
  // Returns eyes ping distance in inches
  long duration=0, inches, cm;
  int x=0;
  
  // Quickly take three pings and average them...
  for( x=0 ; x<3 ; x++){
    // The eyes "ping" is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);  // sets up a clean ping
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH); // form a 2uS ping
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);  // end the ping
    // In this Chinese ping module, we send the ping on one pin (pingPin) and 
    // read the signal from echoPin.  A HIGH pulse whose duration is the 
    // time (in microseconds) from the sending of the ping to the reception 
    // of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = duration + pulseIn(echoPin, HIGH);  // do this and sum it three times      
  }
  // Average the three pulses
  duration = duration / 3;

  // convert the ping time into a distance
  inches = duration / 74 / 2;
  Serial.print(inches);
  Serial.println("in, ");
  
  delay(100);
  return(inches);
}



void loop() {
  //getIR();          //grabs IR-remote control data (if used)
  
  //Check out for possible collisions (collisionwarn = warning, collisiondist = stop! )
  distance = pingsweep();
    //if close to colliding, all stop
  if(distance != -1 && distance < collisiondist ){
    Serial.print("distance=");
    Serial.print(distance);
    Serial.print(" !! COLLISION IMMINENT !!");
    //movestop();              // stops all formward movement
    Serial.println("    All Stop & wait..");
    //getIR();
    delay(2000);
  }
    //otherwise, go ahead and proceed
  else{
  //moveforward();  
  Serial.println("    Looks good.. move forward");
  }
  
}





