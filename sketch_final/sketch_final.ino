#include <Servo.h>
#include <math.h>

bool loop_is_delta = true ;

const int bottonpin = 28;
int bottonwait = 0 ;

const int joystickX = 26;  // GP26 (ADC0) connected to VRx
const int joystickY = 27;  // GP27 (ADC1) connected to VRy

const int passThoughtphi = 4; // atual pin 6
const int passThoughttheta = 5; // atual pin 6

const int hz = 144 ;// loops per second
const float wait= 1000/hz ;

const int deg_per_sec = 100; //in degress per secon  this asmmes all code takes no time to exicte 100 is fors testing in percente values
float deg_per_run = (deg_per_sec/hz);

float deltatheta = 0;
float deltaphi = 0;

int midx = 0;
int midy = 0;

float yjoy = 0;
float xjoy = 0;

int pintheta = 16;
int pinphi = 17;

Servo servotheta;
Servo servophi;

void writeanglephi(float w) {
  int rotation = (int)(w * (2500 - 500) / 270.0 + 500); 
  //Serial.println(rotation);
  servophi.writeMicroseconds(rotation);
}

void writeangletheta(float w) {
  int rotation = (int)(w * (2500 - 500) / 270.0 + 500);
  //Serial.println(rotation);
  servotheta.writeMicroseconds(rotation);
}

void delta() {
  xjoy = analogRead(joystickX);

  if (xjoy >= midx) {
    deltatheta = (((xjoy - midx) / (float)(1024 - midx)) * loop_is_delta);
  }
  if (xjoy <= midx) {
    deltatheta = (((xjoy - midx) / (float)(midx)) * loop_is_delta);
  }

  yjoy = analogRead(joystickY);

  if (yjoy >= midy) {
    deltaphi = (((yjoy - midy) / (float)(1024 - midy)) * loop_is_delta);
  }
  if (yjoy <= midy) {
    deltaphi = (((yjoy - midy) / (float)(
      midy)) * loop_is_delta);
  }
}

void pass_thought() { 
   // chancge to just close some swithchens fo any kind
  deltatheta = digitalRead(passThoughttheta); 
  deltaphi = digitalRead(passThoughtphi);
}

void joycal() {
  // store fist values 
  midx = analogRead(joystickX);  
  midy = analogRead(joystickY);
}

void setup() {
  joycal();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  servotheta.attach(pintheta);
  servophi.attach(pinphi);
  Serial.begin(9600);
}

void loop() {
  if (loop_is_delta == true) {
  delta();
  xjoy = analogRead(joystickX);
  yjoy = analogRead(joystickY);

  Serial.print("X: ");
  Serial.print(xjoy);
  Serial.print(" | Y: ");
  Serial.println(yjoy);

  Serial.print("dX: ");
  Serial.print(deltatheta);
  Serial.print(" |d Y: ");
  Serial.println(deltaphi);
  }
  else {
    //pass_thought(); // might not want to read and hust pass it thought
    Serial.println("got in");
    //loop_is_delta = false ;
  } 
  writeanglephi(deltaphi);
  writeanglephi(deltatheta);

  if ((analogRead(bottonpin) > 700) and (bottonwait < 0) and loop_is_delta == true ) {
    loop_is_delta = false;
    bottonwait = 100;
  }
  if ((analogRead(bottonpin) > 700) and (bottonwait < 0) and loop_is_delta == false ) { 
    loop_is_delta = true;
    bottonwait = 100;
  }
  Serial.print("change");
  Serial.println(loop_is_delta);
  Serial.print("wait");
  Serial.println(bottonwait);
  Serial.print("botton");
  Serial.println(analogRead(bottonpin));
  bottonwait = bottonwait - 1;
  delay(wait);
}