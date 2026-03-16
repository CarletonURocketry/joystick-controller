#include <Servo.h>
#include <math.h>

bool loop_is_delta = false ;

const int bottonpin = 28;
int bottonwait = 0 ;

const int joystickX = 26;  // GP26 (ADC0) connected to VRx
const int joystickY = 27;  // GP27 (ADC1) connected to VRy

const int passThoughtphi = 4; // atual pin 6
const int passThoughttheta = 5; // atual pin 6

const int hz = 10 ;// loops per second
const float wait= 1000/hz ;

const int deg_per_sec = 25; //in degress per secon  this asmmes all code takes no time to exicte 100 is fors testing in percente values
float deg_per_run = (deg_per_sec/hz);

float deltatheta = 0;
float deltaphi = 0;

float phi = 145;
float theta = 145;

int midx = 0;
int midy = 0;

float yjoy = 0;
float xjoy = 0;

int pintheta = 16;
int pinphi = 17;

float deadzone = 1; // in deg /sec
float deadzone_per_run = deadzone/hz; //for use

Servo servotheta;
Servo servophi;

float filter (float value) {
  if ((value < deadzone_per_run) and (value > -deadzone_per_run)){
    int value = 0 ;
  }
  return value ;
}

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
    deltatheta = (((xjoy - midx) / (float)(1024 - midx)) * deg_per_run);
  }
  if (xjoy <= midx) {
    deltatheta = (((xjoy - midx) / (float)(midx)) * deg_per_run);
  }

  yjoy = analogRead(joystickY);

  if (yjoy >= midy) {
    deltaphi = (((yjoy - midy) / (float)(1024 - midy)) * deg_per_run);
  }
  if (yjoy <= midy) {
    deltaphi = (((yjoy - midy) / (float)(1024 - midy)) * deg_per_run);
  }
  deltatheta = filter(deltatheta);
  deltaphi = filter(deltaphi);
  
}

void pass_thought() { 
   // chancge to just close some swithchens of any kind
  deltatheta = digitalRead(passThoughttheta); 
  deltaphi = digitalRead(passThoughtphi);
}

void joycal() {
  // store fist values 
  midx = analogRead(joystickX);  
  midy = analogRead(joystickY);
}
void testprint(){
  Serial.print("X: ");
  Serial.print(xjoy);
  Serial.print(" | Y: ");
  Serial.println(yjoy);

  Serial.print("dX: ");
  Serial.print(deltatheta);
  Serial.print(" |dY: ");
  Serial.println(deltaphi);

  Serial.print("θ: ");
  Serial.print(theta);
  Serial.print(" |φ: ");
  Serial.println(phi);

  Serial.print("change ");
  Serial.println(loop_is_delta);

  Serial.print("wait ");
  Serial.println(bottonwait);

  Serial.print("botton ");
  Serial.println(analogRead(bottonpin));

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
  xjoy = analogRead(joystickX);
  yjoy = analogRead(joystickY);
  delta();


  }
  else {
    //pass_thought(); // might not want to read and hust pass it thought
    Serial.println("got in");
    //loop_is_delta = false ;
  }
  if ((phi <= 270) and (deltaphi > 0)){ 
  phi = phi + deltaphi;
  }
  else if ((phi >= 0) and (deltaphi < 0)){
  phi = phi + deltaphi;
  }
  if ((theta <= 270) and (deltatheta > 0)){ 
  theta = theta + deltatheta;
  }
  else if ((theta >= 0) and (deltatheta < 0)){
  theta = theta + deltatheta;
  }
  writeanglephi(phi);
  writeanglephi(theta);

  if ((analogRead(bottonpin) > 700) and (bottonwait < 0) and loop_is_delta == true ) {
    loop_is_delta = false;
    bottonwait = hz*2;
  }
  if ((analogRead(bottonpin) > 700) and (bottonwait < 0) and loop_is_delta == false ) { 
    loop_is_delta = true;
    bottonwait = hz*2;
  }
    if (bottonwait > -5){
  bottonwait = bottonwait - 1;
  }


  delay(wait);
  testprint();
}
