#include <Servo.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>

// ==============================================
// PIN DEFINITIONS
// ==============================================
const int joystickX = 26;  // GP26 (ADC0) connected to VRx
const int joystickY = 27;  // GP27 (ADC1) connected to VRy

const int buttonPin = 28;

int pinTheta = 16; // Servo output theta
int pinPhi = 17; // Servo output phi

// no idea what these do
const int passThoughtphi = 4; // atual pin 6
const int passThoughttheta = 5; // atual pin 6

int buttonWait = 0;

// ===============================================
// PARAMETERS
// ===============================================
const int LOOP_HZ = 300;// loops per second (hz)
const float LOOP_DELAY = 1000/LOOP_HZ ; // Delay

const float MAX_DEG_PER_SEC = 100.0;
const float DEG_PER_LOOP = MAX_DEG_PER_SEC / LOOP_HZ;

const int xDEADZONE = 35; 
const int yDEADZONE = 35;

const float MIN_ANGLE = 0;
const float MAX_ANGLE = 270;
// ================================================
// GLOBAL VARIABLES
// ================================================
bool deltaMode = true;

Servo servoTheta;
Servo servoPhi;

int midX;
int midY;

float deltaTheta = 0;
float deltaPhi = 0;

float theta = 0;
float phi = 135;

float joyX; //Store current joystick ADC readings
float joyY;

bool active = false;

float deadzonevalueTheta = 0;
float deadzonevaluePhi = 0;

float filteredX = 0;
float filteredY = 0;

#include <iostream>
#include <fstream>
#include <string>

// ================================================
// new file
// ================================================
int file_start() {
    std::ofstream file("test.csv");
    if (file.is_open()) {
        file << ("X,Y,dX,dy,θ,φ,active,DeadzoneX,DeadzoneY,wait,button");
    } else {
        std::cerr << "Unable to open file\n";
    }
    return 0;
}
// ================================================
// JOYSTICK CALIBRATION
// ================================================
int file_wright() {
    std::ofstream file("test.csv");
    if (file.is_open()) {
        file << (joyX);
        file << (",");
        file << (joyY);
        file << (",");
        file << (deltaTheta);
        file << (",");
        file << (deltaPhi);
        file << (",");
        file << (theta);
        file << (",");
        file << (phi);
        file << (",");
        file << (active);
        file << (",");
        file << (deadzonevalueTheta);
        file << (",");
        file << (deadzonevaluePhi);
        file << (",");
        file << (buttonWait);
        file << (",");
        file << (analogRead(buttonPin));
        file << ("\n");
        file.close();
    } else {
        std::cerr << "Unable to open file\n";
    }
    return 0;
}

// ================================================
// JOYSTICK CALIBRATION
// ================================================

void joycal() {
  //Sets servo to start at angle 0

  long sumX = 0;
  long sumY = 0;

//Averaging initial readings to calibrate
  for(int i=0;i<=20;i++)
  {
    sumX += analogRead(joystickX);
    sumY += analogRead(joystickY);
    delay(5);
  }
  midX = sumX/20;
  midY = sumY/20;
  
}

// ================================================
// JOYSTICK NORMALIZATION
// ================================================

void joystickactive() {
  if(abs(joyX-midX) > xDEADZONE || abs(joyY-midY) > yDEADZONE)
  {
    active = true;
  }
  
  else
  {
    active = false;
  }

  //for debugging purposes
  deadzonevalueTheta = abs(joyX-midX);
  deadzonevaluePhi = abs(joyY-midY);
}

// ================================================
// SERVO WRITING
// ================================================

//Converts angle to PWM pulse width

void writeServoPhi(float angle) {
  angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
  int pulse = angle * (2000/270.0) + 500;
  //Serial.println(rotation);
  if(active == true){
  servoPhi.writeMicroseconds(pulse);
  }
}

void writeServoTheta(float angle) {
  angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
  int pulse = angle * (2000/270.0) + 500;

  if(active == true){
  servoTheta.writeMicroseconds(pulse);
  }
}
// ================================================
// JOYSTICK READING
// ================================================

void delta() {
  filteredX = 0.8 * filteredX + 0.2 * analogRead(joystickX);
  filteredY = 0.8 * filteredY + 0.2 * analogRead(joystickY);

  joyX = filteredX;
  if (joyX >= midX) {
    deltaTheta = (((joyX - midX) / (float)(1024 - midX)) * deltaMode);
  }
  else if (joyX <= midX) {
    deltaTheta = (((joyX - midX) / (float)(midX)) * deltaMode);
  }

  joyY = filteredY;
  if (joyY >= midY) {
    deltaPhi = (((joyY - midY) / (float)(1024 - midY)) * deltaMode);
  }
  else if(joyY <= midY) {
    deltaPhi = (((joyY - midY) / (float)(midY)) * deltaMode);
  }
}
// ================================================
// test printing and loging 
// ================================================

void testprint(){
  Serial.print("X: ");
  Serial.print(joyX);
  Serial.print(" | Y: ");
  Serial.println(joyY);

  Serial.print("dX: ");
  Serial.print(deltaTheta);
  Serial.print(" |d Y: ");
  Serial.println(deltaPhi);

  Serial.print("θ: ");
  Serial.print(theta);
  Serial.print(" |φ: ");
  Serial.println(phi);

  Serial.print("Active: ");
  Serial.println(active);
  Serial.print("DeadzoneX: ");
  Serial.println(deadzonevalueTheta);
  Serial.print(" | DeadzoneY: ");
  Serial.println(deadzonevaluePhi);

  Serial.print("wait ");
  Serial.println(buttonWait);

  Serial.print("button ");
  Serial.println(analogRead(buttonPin));
}

// ================================================
// SETUP
// ================================================
void setup() {
  Serial.begin(9600);
  joycal();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  servoTheta.attach(pinTheta);
  servoPhi.attach(pinPhi);
  //file_start(); look at next year
}

// ================================================
// MAIN LOOP
// ================================================

  void loop() {
  if (deltaMode == true) {
  joystickactive();
  delta();

  if (abs(deltaTheta) > 0.06)
  {
    theta += deltaTheta * DEG_PER_LOOP;
  }

  if(abs(deltaPhi) > 0.06)
  {
    phi += deltaPhi * DEG_PER_LOOP;
  }
  }
  else {
    //pass_thought(); // might not want to read and hust pass it thought
    Serial.println("got in");
    deltaMode = false ;
  } 

  theta = constrain(theta, MIN_ANGLE, MAX_ANGLE);
  phi = constrain(phi, MIN_ANGLE, MAX_ANGLE);
  
  writeServoPhi(phi);
  writeServoTheta(theta);
  

  
  if ((analogRead(buttonPin) < 5) and (buttonWait < 0) and deltaMode == true ) {
   deltaMode = false;
   buttonWait = 100;
  }
  if ((analogRead(buttonPin) < 5) and (buttonWait < 0) and deltaMode == false ) { 
   deltaMode = true;
    buttonWait = 100;
  }
  // Serial.print("change");
  // Serial.println(deltaMode);
  // Serial.print("wait");
  // Serial.println(buttonWait);
  // Serial.print("button");
  // Serial.println(analogRead(buttonPin));
  if  (buttonWait>-10){
  buttonWait = buttonWait - 1;
  }
  //file_wright(); doe not work look at next year
  testprint();
  delay(LOOP_DELAY);
  }