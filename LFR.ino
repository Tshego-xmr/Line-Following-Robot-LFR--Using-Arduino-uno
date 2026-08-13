// 1/header comment 
// project Infor:SENSOR LINE FOLLOWING ROBOT
// Arduino UNO + L298N + 2 IR Sensors

//2libraries

//3 Pin declaration
// -------- IR SENSOR PINS --------
const int RIGHT_SENSOR = 3; //pwm
const int LEFT_SENSOR  = 11; //pwm

//L298N PINS
// Left motor
const int ENA = 5;   // PWM - Left motor speed
const int IN1 = 6;   // Left motor direction
const int IN2 = 7;   // Left motor direction

// Right motor
const int IN3 = 8;   // Right motor direction
const int IN4 = 9;   // Right motor direction
const int ENB = 10;  // PWM - Right motor speed

//  MOTOR SPEED 
int motorSpeed = 300;

void setup()
{
  // IR sensors
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);

  // L298N
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop()
{
  int rightSensor = digitalRead(RIGHT_SENSOR);
  int leftSensor  = digitalRead(LEFT_SENSOR);

  // --------------------------------
  // BOTH SENSORS ON WHITE
  // Move forward
  // --------------------------------
  if (leftSensor == HIGH && rightSensor == HIGH)
  {
    moveForward();
  }

  
  // LEFT SENSOR DETECTS BLACK
  // Turn LEFT
  
  else if (leftSensor == HIGH && rightSensor == LOW)
  {
    turnLeft();
  }

  
  // RIGHT SENSOR DETECTS BLACK
  // Turn RIGHT
  
  else if (leftSensor == LOW && rightSensor == HIGH)
  {
    turnRight();
  }

  
  // BOTH SENSORS DETECT BLACK
  // Stop
  
  else
  {
    stopMotors();
  }
}


// MOVE FORWARD

void moveForward()
{
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}


// TURN LEFT

void turnLeft()
{
  // Left motor slower
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 60);
  analogWrite(ENB, motorSpeed);
}


// TURN RIGHT

void turnRight()
{
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor slower
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, 60);
}


// STOP
void stopMotors()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
