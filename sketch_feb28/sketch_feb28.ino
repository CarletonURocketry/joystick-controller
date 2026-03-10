#include <Servo.h>
#include <math.h>

const int joystickX = 26;  // GP26 (ADC0) connected to VRx
const int joystickY = 27;  // GP27 (ADC1) connected to VRy

const int maxdeg = 10;

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
  Serial.println(rotation);
  servophi.writeMicroseconds(rotation);
}

void writeangletheta(float w) {
  int rotation = (int)(w * (2500 - 500) / 270.0 + 500);
  Serial.println(rotation);
  servotheta.writeMicroseconds(rotation);
}

void delta() {
  xjoy = analogRead(joystickX);
  if (xjoy >= midx) {
    deltatheta = (((xjoy - midx) / (float)(1024 - midx)) * 100);
  }
  if (xjoy <= midx) {
    deltatheta = (((xjoy - midx) / (float)(midx)) * 100);
  }

  yjoy = analogRead(joystickY);
  if (yjoy >= midy) {
    deltaphi = (((yjoy - midy) / (float)(1024 - midy)) * 100);
  }
  if (yjoy <= midy) {
    deltaphi = (((yjoy - midy) / (float)(midy)) * 100);
  }
}

void joycal() {
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
  delta();
  xjoy = analogRead(joystickX);
  yjoy = analogRead(joystickY);

  Serial.print("X: ");
  Serial.print(xjoy);
  Serial.print(" | Y: ");
  Serial.println(yjoy);


  Serial.print("X: ");
  Serial.print(deltatheta);
  Serial.print(" | Y: ");
  Serial.println(deltaphi);


  delay(250);
}