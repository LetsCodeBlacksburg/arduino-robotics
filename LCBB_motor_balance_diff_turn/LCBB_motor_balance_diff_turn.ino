#include <Arduino.h>

//Configure Keyes L298 Motor H-Bridge Board for the following wiring setup:
    //Line   Function       A.Pin
    //===============================
    //ENB    motor-b enable D3  PWM
    //IN4    motor-b +      D2  (invert 3/4 the change direction)
    //IN3    motor-b -      D4
    //IN2    motor-a -      D5  (invert 1/2 the change direction)
    //IN1    motor-a +      D7
    //ENA    motor-a enable D6  PWM

// Right motor control
int R_EN =  6;  // ENA
int R_IN1 = 7;  // IN1
int R_IN2 = 5;  // IN2

// Left motor control
int L_EN =  3;  // ENB
int L_IN1 = 4;  // IN3
int L_IN2 = 2;  // IN4

// Current speed
int L_SPEED = 0;
int R_SPEED = 0;

// Use this to compensate for an imbalance between the wheels
// 1) Set it to 0.0 to start
// 2) Send your bot in a straight line
// 3) If it drifts to the left, try a small negative value (e.g. -0.1) to slow down the right wheel
// 4) If it drifts to the right, try a small positive value (e.g. 0.1) to slow down the left wheel
// 5) Try the straight line test again and adjust values as needed
float BALANCE = -0.1;

void setup()
{
  Serial.begin(9600);
  pinMode(R_IN1,OUTPUT);  //Setup pins for motor-a 
  pinMode(R_IN2,OUTPUT);  //Setup pins for motor-a
  pinMode(L_IN1,OUTPUT);  //Setup pins for motor-b 
  pinMode(L_IN2,OUTPUT);  //Setup pins for motor-b
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
}

void speedLeft(byte speed)
{
  L_SPEED = speed;
  analogWrite(L_EN, speed);
}

void speedRight(byte speed)
{
  R_SPEED = speed;
  analogWrite(R_EN, speed);
}

void forwardRight()
{
  digitalWrite(R_IN1, 0);
  digitalWrite(R_IN2, 1);
}

void reverseRight()
{
  digitalWrite(R_IN1, 1);
  digitalWrite(R_IN2, 0);
}

void forwardLeft()
{
  digitalWrite(L_IN1, 0);
  digitalWrite(L_IN2, 1);
}

void reverseLeft()
{
  digitalWrite(L_IN1, 1);
  digitalWrite(L_IN2, 0);
}

// Given a speed and a differential percentage (0.0 - 1.0)
// calculate the speed for the other wheel
int getDifferentialSpeed(int s, float diffPct)
{
  int totDiff = 2 * s;
  int diff = totDiff * diffPct;
  int otherSpeed = s - diff;
  
  return otherSpeed;
}

// Turn right using the given turn rate
// Turn rate is between 0.0 and 1.0
// 1.0 is maximum differential - right wheel will run max reverse
// 0.5 will stop the right wheel during the turn
// < 0.5 the right wheel continues forward but proportionally slower than the left
void turnRight(float turnRate)
{
  int r_spd = getDifferentialSpeed(L_SPEED, turnRate);
  if(r_spd < 0)
  {
    reverseRight();
    speedRight(-r_spd);
  }
  else
  {
    speedRight(r_spd);
  }
}

// Turn right using the given turn rate
// Turn rate is between 0.0 and 1.0
// 1.0 is maximum differential - left wheel will run max reverse
// 0.5 will stop the left wheel during the turn
// < 0.5 the left wheel continues forward but proportionally slower than the right
void turnLeft(float turnRate)
{
  int l_spd = getDifferentialSpeed(R_SPEED, turnRate);
  if(l_spd < 0)
  {
    reverseLeft();
    speedLeft(-l_spd);
  }
  else
  {
    speedLeft(l_spd);
  }
}

// Set the speed for both motors with imbalance compensation
// See the BALANCE variable above for details
void setSpeed(int speed)
{
  int lspeed = (BALANCE <= 0) ? speed : speed * (1 - BALANCE);
  int rspeed = (BALANCE >= 0) ? speed : speed * (1 + BALANCE);
  speedRight(rspeed);
  speedLeft(lspeed);
}

void forward(int speed)
{
  forwardRight();
  forwardLeft();

  setSpeed(speed);
}

void reverse(int speed)
{
  reverseRight();
  reverseLeft();

  setSpeed(speed);
}

void loop()
{
  forward(255);
  delay(1000);
  turnRight(0.5);
  delay(1000);
}
