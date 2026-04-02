#include <Servo.h>
#include <math.h>

// Pins
const int joystickX_PIN = 28;
const int joystickY_PIN = 27;
const int button_PIN = 0;
const int pwmInputPan_PIN = 5;
const int pwmInputTilt_PIN = 6;
const int pwmOutputPan_PIN = 13;
const int pwmOutputTilt_PIN = 11;

const int LOOP_HZ = 200;
const float MAX_DEG_PER_SEC = 100.0;
const float DEG_PER_LOOP = MAX_DEG_PER_SEC / LOOP_HZ;

const int xDEADZONE = 35;
const int yDEADZONE = 35;

const float JOYSTICK_MOVE_SPEED = 0.001; // degrees per loop in joystick mode

const float MIN_ANGLE = 0;
const float MAX_ANGLE = 270;

volatile unsigned long pwmPanStartTime = 0;
volatile unsigned long pwmPanInputValue = 0;
volatile unsigned long pwmTiltStartTime = 0;
volatile unsigned long pwmTiltInputValue = 0;

Servo servoPan;
Servo servoTilt;
int buttonWait = 0;
int midX;
int midY;

float deltaTheta = 0;
float deltaPhi = 0;
float joyX;
float joyY;
float filteredX = 0;
float filteredY = 0;

float joystickTheta = 0;
float joystickPhi = 0;
bool joystickMode = false;

// ================================================
// JOYSTICK CALIBRATION
// ================================================

void joycal() {
  long sumX = 0;
  long sumY = 0;
  for (int i = 0; i <= 20; i++) {
    sumX += analogRead(joystickX_PIN);
    sumY += analogRead(joystickY_PIN);
    delay(5);
  }
  midX = sumX / 20;
  midY = sumY / 20;
}

// ================================================
// SERVO WRITING
// ================================================

void writeservoPan(float angle) {
  angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
  int pulse = angle * (2000 / 270.0) + 500;
  servoPan.writeMicroseconds(pulse);
}

void writeservoTilt(float angle) {
  angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
  int pulse = angle * (2000 / 270.0) + 500;
  servoTilt.writeMicroseconds(pulse);
}

// ================================================
// JOYSTICK READING
// ================================================

void delta() {
  filteredX = 0.8 * filteredX + 0.2 * analogRead(joystickX_PIN);
  filteredY = 0.8 * filteredY + 0.2 * analogRead(joystickY_PIN);

  joyX = filteredX;
  if (joyX >= midX) {
    deltaTheta = ((joyX - midX) / (float)(1024 - midX));
  } else {
    deltaTheta = ((joyX - midX) / (float)(midX));
  }

  joyY = filteredY;
  if (joyY >= midY) {
    deltaPhi = ((joyY - midY) / (float)(1024 - midY));
  } else {
    deltaPhi = ((joyY - midY) / (float)(midY));
  }
}

// ================================================
// PWM INPUT INTERRUPTS
// ================================================

void readPWMPan() {
  if (digitalRead(pwmInputPan_PIN) == HIGH) {
    pwmPanStartTime = micros();
  } else {
    pwmPanInputValue = micros() - pwmPanStartTime;
  }
}

void readPWMTilt() {
  if (digitalRead(pwmInputTilt_PIN) == HIGH) {
    pwmTiltStartTime = micros();
  } else {
    pwmTiltInputValue = micros() - pwmTiltStartTime;
  }
}

// ================================================
// SETUP
// ================================================

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(button_PIN, INPUT_PULLUP);

  joycal();

  pinMode(pwmInputPan_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(pwmInputPan_PIN), readPWMPan, CHANGE);
  pinMode(pwmInputTilt_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(pwmInputTilt_PIN), readPWMTilt, CHANGE);

  servoPan.attach(pwmOutputPan_PIN, 500, 2500);
  servoTilt.attach(pwmOutputTilt_PIN, 500, 2500);
}

// ================================================
// LOOP
// ================================================

void loop() {
  noInterrupts();
  unsigned long pan = (pwmPanInputValue / 5) * 5;
  unsigned long tilt = (pwmTiltInputValue / 5) * 5;
  interrupts();

  if (digitalRead(button_PIN) == LOW && buttonWait < 0) {
    if (!joystickMode) {
      joystickTheta = (pan - 500) * (270.0 / 2000.0);
      joystickPhi = (tilt - 500) * (270.0 / 2000.0);
      joystickMode = true;
    } else {
      joystickMode = false;
    }
    buttonWait = 100;
  }
  if (buttonWait > -10)
    buttonWait--;

  if (joystickMode) {
    int rawX = analogRead(joystickX_PIN);
    int rawY = analogRead(joystickY_PIN);

    if (rawX - midX > xDEADZONE)
      joystickTheta += JOYSTICK_MOVE_SPEED;
    if (rawX - midX < -xDEADZONE)
      joystickTheta -= JOYSTICK_MOVE_SPEED;
    if (rawY - midY > yDEADZONE)
      joystickPhi += JOYSTICK_MOVE_SPEED;
    if (rawY - midY < -yDEADZONE)
      joystickPhi -= JOYSTICK_MOVE_SPEED;

    joystickTheta = constrain(joystickTheta, MIN_ANGLE, MAX_ANGLE);
    joystickPhi = constrain(joystickPhi, MIN_ANGLE, MAX_ANGLE);

    writeservoPan(joystickTheta);
    writeservoTilt(joystickPhi);
  } else {
    servoPan.writeMicroseconds(pan);
    servoTilt.writeMicroseconds(tilt);
  }
}