/*
  NC(V) INTRODUCTION TO ROBOTICS LEVEL 3

  ISAT SUBTASK 9
  PING PONG BALL ROBOT - PPBR

  Student Name: DARIAN VERGOTINE

  Student Number: 162 502 118

  Date: 21 AUGUST 2026
*/

#include <Servo.h>

// =====================================================
// 1. SERVO SETUP
// =====================================================
const int SERVO_PIN = 9;
const int SERVO_UP = 0;               // frame lifted (open / release)
const int SERVO_DOWN = 90;            // frame dropped (capture)
const unsigned long SERVO_MOVE_MS = 500;

// =====================================================
// 2. MOTOR DRIVER CONFIGURATION
// =====================================================
const int ENA = 5;        // left motor enable (L298N ENA)
const int IN1 = 4;        // left motor
const int IN2 = 6;
const int ENB = 3;        // right motor enable (L298N ENB)
const int IN3 = 8;        // right motor
const int IN4 = 7;

// =====================================================
// 3. IR SENSOR CONFIGURATION
// =====================================================
const int IR_LEFT = 10;
const int IR_RIGHT = 11;

// =====================================================
// BOOLEAN STATEMENT
// =====================================================
const bool IR_HIGH_ON_WHITE = false;

// =====================================================
// DELAY CONFIGURATION
// =====================================================
const unsigned long START_DELAY_MS = 1000;
const unsigned long CAPTURE_WAIT_MS = 3000;
const int LINE_DEBOUNCE_COUNT = 5;    // consecutive samples (~50 ms)

Servo frame;

// =====================================================
// BOOLEAN EXPRESSIONS ON THE IR SENSORS
// =====================================================
bool isBlack(int pin) {
  int v = digitalRead(pin);
  if (IR_HIGH_ON_WHITE) {
    return v == LOW;
  }
  return v == HIGH;
}

bool lineDetected() {
  return isBlack(IR_LEFT) || isBlack(IR_RIGHT);
}

bool onWhite() {
  return !isBlack(IR_LEFT) && !isBlack(IR_RIGHT);
}

// =====================================================
// MOTOR CONTROL CONFIGURATOR
// =====================================================
void motorsEnable(bool on) {
  digitalWrite(ENA, on ? HIGH : LOW);
  digitalWrite(ENB, on ? HIGH : LOW);
}

// =====================================================
// MOVE FORWARD FUNCTION
// =====================================================
void driveForward() {
  motorsEnable(true);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// =====================================================
// STOP MOTOR FUNCTION
// =====================================================
void stopMotors() {
  motorsEnable(false);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =====================================================
// WAIT UNTIL LINE IS DETECTED
// =====================================================
void waitUntilLine() {
  int stable = 0;
  while (stable < LINE_DEBOUNCE_COUNT) {
    if (lineDetected()) {
      stable++;
    } else {
      stable = 0;
    }
    delay(10);
  }
}

// =====================================================
// IS ROBOT ON WHITE SURFACE?
// =====================================================
void waitUntilWhite() {
  int stable = 0;
  while (stable < LINE_DEBOUNCE_COUNT) {
    if (onWhite()) {
      stable++;
    } else {
      stable = 0;
    }
    delay(10);
  }
}

// =====================================================
// LOW FRAME FUNCTION
// =====================================================
void dropFrame() {
  frame.write(SERVO_DOWN);
  delay(SERVO_MOVE_MS);
}

// =====================================================
// HIGH FRAME FUNCTION
// =====================================================
void liftFrame() {
  frame.write(SERVO_UP);
  delay(SERVO_MOVE_MS);
}

// =====================================================
// SETUP CONFIGURATION
// =====================================================
void setup() {
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
  delay(START_DELAY_MS);                // let the 5 V rail settle after power-on

  frame.attach(SERVO_PIN);
  liftFrame();                          // start with frame up so the ball can roll in
  delay(300);
}

// =====================================================
// LOOP COUNTER
// =====================================================
void loop() {
  // First black line: stop and capture
  driveForward();
  waitUntilLine();
  stopMotors();
  dropFrame();

  delay(CAPTURE_WAIT_MS);

  // Leave the first line, then stop on the second line and release
  driveForward();
  waitUntilWhite();
  waitUntilLine();
  stopMotors();
  liftFrame();

  while (true) {                        // one run per power cycle
    stopMotors();
    delay(100);
  }
}
