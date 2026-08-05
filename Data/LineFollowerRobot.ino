// Name       : Darian
// Surname    : Vergotine
// Student No : 162 502 118
// Task type  : IROB ISAT FINAL  

#include "config.h"

// Motor control pins (L298N)
const int motorA_IN1 = 6;    // Motor A input 1
const int motorA_IN2 = 7;    // Motor A input 2
const int motorA_EN = 5;    // Motor A enable (PWM)

const int motorB_IN3 = 9;   // Motor B input 3
const int motorB_IN4 = 8;   // Motor B input 4
const int motorB_EN = 10;     // Motor B enable (PWM)

// Sensor pins
const int leftSensor = A0;
const int rightSensor = A1;

// Variables for sensor readings
int leftValue = 0;
int rightValue = 0;
bool leftDetected = false;
bool rightDetected = false;

void setup() {
  // Initialize motor pins as outputs
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorA_EN, OUTPUT);
  
  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);
  pinMode(motorB_EN, OUTPUT);
  
  // Initialize sensor pins as inputs
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  
  // Serial communication for debugging
  Serial.begin(9600);
  Serial.println("Line Follower Robot Initialized");
  Serial.println("================================");
}

void loop() {
  // Read sensor values
  readSensors();
  
  // Determine line position and control motors
  followLine();
  
  // Debug output (can be disabled)
  if (DEBUG_MODE) {
    printDebugInfo();
  }
}

/*
 * Reads both IR sensors and determines if they detect the black line.
 * Sensor values range from 0-1023 (0 = dark/on line, 1023 = light/off line)
 */
void readSensors() {
  leftValue = analogRead(leftSensor);
  rightValue = analogRead(rightSensor);
  
  // Detect line based on threshold
  leftDetected = (leftValue < SENSOR_THRESHOLD);
  rightDetected = (rightValue < SENSOR_THRESHOLD);
}

/*
 * Main line-following logic using PD control
 * 
 * Sensor states:
 * - Left ON, Right ON   -> Move forward (line centered)
 * - Left ON, Right OFF  -> Turn left
 * - Left OFF, Right ON  -> Turn right
 * - Left OFF, Right OFF -> Emergency stop (lost line)
 */
void followLine() {
  if (leftDetected && rightDetected) {
    // Line is centered - move forward
    moveForward(BASE_SPEED);
  }
  else if (leftDetected && !rightDetected) {
    // Robot drifted right - turn left
    turnLeft(BASE_SPEED);
  }
  else if (!leftDetected && rightDetected) {
    // Robot drifted left - turn right
    turnRight(BASE_SPEED);
  }
  else {
    // Line lost - stop and wait
    stopMotors();
    if (DEBUG_MODE) {
      Serial.println("WARNING: Line lost!");
    }
  }
}

/*
 * Move robot forward at specified speed (0-255)
 */
void moveForward(int speed) {
  // Motor A - forward
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  analogWrite(motorA_EN, speed);
  
  // Motor B - forward
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
  analogWrite(motorB_EN, speed);
}

/*
 * Move robot backward at specified speed (0-255)
 */
void moveBackward(int speed) {
  // Motor A - backward
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  analogWrite(motorA_EN, speed);
  
  // Motor B - backward
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
  analogWrite(motorB_EN, speed);
}

/*
 * Turn left by reducing left motor speed
 */
void turnLeft(int speed) {
  // Motor A - slower or stopped
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  analogWrite(motorA_EN, speed * TURN_FACTOR);
  
  // Motor B - full speed
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
  analogWrite(motorB_EN, speed);
}

/*
 * Turn right by reducing right motor speed
 */
void turnRight(int speed) {
  // Motor A - full speed
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  analogWrite(motorA_EN, speed);
  
  // Motor B - slower or stopped
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
  analogWrite(motorB_EN, speed * TURN_FACTOR);
}

/*
 * Stop all motors
 */
void stopMotors() {
  analogWrite(motorA_EN, 0);
  analogWrite(motorB_EN, 0);
}

/*
 * Print debug information to serial monitor
 */
void printDebugInfo() {
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {  // Print every 500ms
    Serial.print("L:");
    Serial.print(leftValue);
    Serial.print(" R:");
    Serial.print(rightValue);
    Serial.print(" | ");
    Serial.print(leftDetected ? "L_ON " : "L_OFF ");
    Serial.println(rightDetected ? "R_ON" : "R_OFF");
    lastPrint = millis();
  }
}
