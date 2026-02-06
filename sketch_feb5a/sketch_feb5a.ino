#include <Servo.h>
#include <math.h>

Servo servotop;
Servo servobot;

const int joystickX = 26;  // GP26 (ADC0) connected to VRx
const int joystickY = 27;  // GP27 (ADC1) connected to VRy

int angle = 0;
int servomax = 2500;
int servomin = 500;

// int postop = 0;
// int posbot = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  servotop.attach(16, 500, 2500);

}

void loop() {
  // put your main code here, to run repeatedly:

  // for(int i = 0; i < 7; i++){
  //   writeangle(i*45);
  // }

  int xValue = analogRead(joystickX);
  int yValue = analogRead(joystickY);

  Serial.print("X: ");
  Serial.println(xValue);
  Serial.print("Y: ");
  Serial.println(yValue);

  // angle = angle + (xValue-180)/1000;
  // writeangle(angle);
  // delay(5);

}


void writeangle(float w){
  int r = w*(servomax-servomin)/270 + servomin;
  Serial.println(r);
  servotop.writeMicroseconds(r);
  delay(5);

}