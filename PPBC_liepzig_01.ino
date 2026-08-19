/* NAME         : DARIAN
   SURNAME      : VERGOTINE
   STUDENT NO   : 162 502 118
   ISAT TYPE    : IROB ISAT PPBC*/

#include <Servo.h>

// ==== PIN DEFINTIONS ==== //
const int IR_LEFT = 10;
const int IR_RIGHT = 11;

// ==== LEFT MOTOR ==== //
const int ENA = 3;
const int IN1 = 6;
const int IN2 = 4;

// ==== RIGHT MOTOR ==== //
const int ENB = 5;
const int IN3 = 8;
const int IN4 = 7;

// ==== SERVO MOTOR ==== //
const int SERVO_PIN = 9;

// ==== DEFAULT STRING ==== //
const bool IR_HIGH_ON_WHITE = false;

const int MOTOR_SPEED = 200; // 0-255
const int SERVO_OPEN = 0;    // release angle
const int SERVO_CLOSED = 90; // capture angle

const unsigned long STATUS_INTERVAL_MS = 1000;
const unsigned long CLEAR_AFTER_RELEASE_MS = 400; // move forward after release to clear line
const unsigned long CAPTURE_PAUSE_MS = 3000; // pause after capture before driving again

Servo arm;
unsigned long lastStatusPrint = 0;

void driveForward(int speed){
  analogWrite(ENA, speed); analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void stopMotors(){
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

bool isWhite(int pin){
  int v = digitalRead(pin);
  return IR_HIGH_ON_WHITE ? (v == HIGH) : (v == LOW);
}
bool isBlack(int pin){ return !isWhite(pin); }

void printSensorStatusIfDue(){
  if (millis() - lastStatusPrint > STATUS_INTERVAL_MS){
    Serial.print("L:"); Serial.print(digitalRead(IR_LEFT));
    Serial.print(" R:"); Serial.println(digitalRead(IR_RIGHT));
    lastStatusPrint = millis();
  }
}

void capture(){
  Serial.println("Servo: closing to capture");
  arm.write(SERVO_CLOSED);
  delay(600);
  Serial.println("Captured");
}

void releaseBall(){
  Serial.println("Servo: opening to release");
  arm.write(SERVO_OPEN);
  delay(400);
  Serial.println("Released");
}

void waitForBothWhite(){
  Serial.println("Waiting for BOTH sensors WHITE...");
  while (!(isWhite(IR_LEFT) && isWhite(IR_RIGHT))){
    stopMotors();
    printSensorStatusIfDue();
    delay(40);
  }
  Serial.println("Both WHITE detected");
}

bool waitForLineDebounced(unsigned long timeout = 15000){
  unsigned long start = millis();
  int stable = 0;
  while (millis() - start < timeout){
    if (isBlack(IR_LEFT) || isBlack(IR_RIGHT)){
      if (++stable >= 5) return true; // ~50 ms stable
    } else {
      stable = 0;
    }
    printSensorStatusIfDue();
    delay(10);
  }
  return false;
}

void setup(){
  pinMode(IR_LEFT, INPUT); pinMode(IR_RIGHT, INPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  delay(50);
  Serial.println("Ping Pong Catcher starting...");
  arm.attach(SERVO_PIN);
  arm.write(SERVO_OPEN);
  Serial.println("Servo set to OPEN");
  delay(300);
}

void loop(){
  // Wait until robot is placed on WHITE to begin
  waitForBothWhite();

  // Phase 1: drive to first line
  Serial.println("Driving to first line");
  driveForward(MOTOR_SPEED);
  if (waitForLineDebounced()){
    stopMotors();
    Serial.println("First black line detected");
    delay(100);
    capture();
  } else {
    stopMotors();
    Serial.println("First-line timeout, restarting wait");
    return;
  }

  // Pause after capture even if WHITE not seen
  Serial.print("Pausing after capture for "); Serial.print(CAPTURE_PAUSE_MS); Serial.println(" ms");
  stopMotors();
  delay(CAPTURE_PAUSE_MS);

  // Clear first line (short drive) and proceed to second line without waiting for WHITE
  Serial.println("Clearing first line (short drive) and continuing to second line");
  driveForward(MOTOR_SPEED);
  delay(300);
  stopMotors();

  // Phase 2: drive to second line
  Serial.println("Driving to second line");
  driveForward(MOTOR_SPEED);
  if (waitForLineDebounced()){
    stopMotors();
    Serial.println("Second black line detected");
    delay(50);
    releaseBall();
  } else {
    stopMotors();
    Serial.println("Second-line timeout, restarting wait");
    return;
  }

  // Move forward a bit to clear the release area, then stop and wait for WHITE to auto-restart
  Serial.println("Clearing after release to find WHITE for next run");
  driveForward(MOTOR_SPEED);
  delay(CLEAR_AFTER_RELEASE_MS);
  stopMotors();

  // Now wait for BOTH sensors to see WHITE again; when they do loop() repeats automatically
  waitForBothWhite();
  Serial.println("Ready for next cycle");
  delay(200); // small debounce before next cycle
}