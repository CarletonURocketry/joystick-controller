#include <Servo.h>
#include <math.h>

const int joystickX = 26;  // GP26 (ADC0) connected to VRx
const int joystickY = 27;  // GP27 (ADC1) connected to VRy

const int maxdeg = 10;

float deltatheta = 0;
float deltaphi = 0;

int midx =0;
int midy =0;

float yjoy =0;
float xjoy =0;

int pintheta = 16;
int pinphi = 17;

Servo servotheta;
Servo servophi ;


void writeanglephi(float w){
  int rotation = w(2500-500)/270 + 500;
  Serial.println(rotation);
  servophi.writeMicroseconds(rotation);
  //delay(2000);

}
void writeangletheta(float w){
  int rotation = w(2500-500)/270 + 500;
  Serial.println(rotation);
  servotheta.writeMicroseconds(rotation);
  //delay(2000);

}
void delta(){

  xjoy = analogRead(joystickX);
  if (xjoy >= midx) {
    deltatheta = (((xjoy-midx)/(1024-midx))100);
  }
  if  (xjoy <= midx) {
    deltatheta = (((xjoy-midx)/(midx))100);
  }
  yjoy = analogRead(joystickY);
  if (yjoy >= midx) {
    deltaphi = (((yjoy-midx)/(1024-midy))100);
  }
  if  (xjoy <= midx) {
    deltaphi = (((yjoy-midx)/(midy))100);
  }

}
void joycal(){
  midx=analogRead(joystickX);
  midy=analogRead(joystickY);
  }
// int theta = 145 ;
// int phi = 145 ;

void setup(){
  joycal();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  servotheta.attach(pintheta); // actual pin 21
  servophi.attach(pinphi); // actual pin 22
  Serial.begin(9600);
  // int theta = 0;
  // int phi = 0;
  // servotheta.write(0);
  //midx = analogRead(joystickX);
 // midy = analogRead(joystickY);
 // if x >= 0 {
//    deltatheta=((theta-midx)/minx);
 //   theta=maxdeg*deltatheta
 // } 
 }
void loop(){




  int theta = analogRead(joystickX);
  int phi = analogRead(joystickY);
  delta();

  Serial.print("X: ");
  Serial.print(deltatheta);
  Serial.print(" | Y: ");
  Serial.print(deltaphi);
  servotheta.write(theta);
  servophi.write(phi);
  // phi = phi + 45 ;
  // theta = theta + 45 ;



  delay(1000);
}