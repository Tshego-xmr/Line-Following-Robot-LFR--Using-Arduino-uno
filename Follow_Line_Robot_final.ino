/* NAME         : UNLOCALIZED
   SURNAME      : UNLOCALIZED
   STUDENT NO   : UNLOCALIZED
   ISAT TYPE    : UNLOCALIZED*/

// --- Pin configuration ---
const uint8_t LEFT_SENSOR_PIN  = 11;  // digital input
const uint8_t RIGHT_SENSOR_PIN = 10;  // digital input

// L298N motor pins
const uint8_t L_IN1 = 5;
const uint8_t L_IN2 = 6;
const uint8_t L_ENA = 3; // PWM

const uint8_t R_IN3 = 8;
const uint8_t R_IN4 = 7;
const uint8_t R_ENB = 9; // PWM

// --- Behavior tuning ---
const int baseSpeed = 100;         // base PWM speed (0-255)
const float turnSpeedFactor = 0.2; // reduction factor for inner wheel when turning
const int SEARCH_SPEED = 150;      // (unused when stopping on no line)
const unsigned long SEARCH_TIMEOUT = 5000; // ms (unused when stopping on no line)

// If your IR outputs LOW on detecting black set true, otherwise set false
const bool LINE_IS_BLACK_IS_LOW = true;

// Debounce / smoothing
const uint8_t SAMPLE_COUNT = 3;    // number of quick samples per read to stabilize sensors

// Tracking last known direction for recovery
enum LastDir { DIR_NONE = 0, DIR_LEFT = -1, DIR_RIGHT = 1 };
volatile LastDir lastKnownDir = DIR_RIGHT;

void setup() {
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(L_ENA, OUTPUT);

  pinMode(R_IN3, OUTPUT);
  pinMode(R_IN4, OUTPUT);
  pinMode(R_ENB, OUTPUT);

  // Start stopped briefly, then start moving
  stopMotors();
  delay(50);

  Serial.begin(9600);
  Serial.println("Line follower starting (will STOP when no line detected)...");
}

// Read sensor with small smoothing
bool readSensor(uint8_t pin) {
  uint8_t positive = 0;
  for (uint8_t i = 0; i < SAMPLE_COUNT; ++i) {
    int v = digitalRead(pin);
    if (v == HIGH) positive++;
    delay(2);
  }
  bool high = (positive >= (SAMPLE_COUNT/2 + 1));
  // If sensor returns LOW on black, invert output so function returns true when ON_LINE
  return LINE_IS_BLACK_IS_LOW ? !high : high;
}

// Motor helper: speed range -255..255, negative = reverse
void setLeftMotor(int speed) {
  if (speed > 0) {
    digitalWrite(L_IN1, HIGH);
    digitalWrite(L_IN2, LOW);
    analogWrite(L_ENA, constrain(speed, 0, 255));
  } else if (speed < 0) {
    digitalWrite(L_IN1, LOW);
    digitalWrite(L_IN2, HIGH);
    analogWrite(L_ENA, constrain(-speed, 0, 255));
  } else {
    digitalWrite(L_IN1, LOW);
    digitalWrite(L_IN2, LOW);
    analogWrite(L_ENA, 0);
  }
}

void setRightMotor(int speed) {
  if (speed > 0) {
    digitalWrite(R_IN3, HIGH);
    digitalWrite(R_IN4, LOW);
    analogWrite(R_ENB, constrain(speed, 0, 255));
  } else if (speed < 0) {
    digitalWrite(R_IN3, LOW);
    digitalWrite(R_IN4, HIGH);
    analogWrite(R_ENB, constrain(-speed, 0, 255));
  } else {
    digitalWrite(R_IN3, LOW);
    digitalWrite(R_IN4, LOW);
    analogWrite(R_ENB, 0);
  }
}

void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}

// Drive differential: positive = forward
void drive(int leftSpeed, int rightSpeed) {
  setLeftMotor(leftSpeed);
  setRightMotor(rightSpeed);
}

unsigned long searchStart = 0;

void loop() {
  bool leftOnLine  = readSensor(LEFT_SENSOR_PIN);
  bool rightOnLine = readSensor(RIGHT_SENSOR_PIN);

  // For debugging
  //Serial.print("L:"); Serial.print(leftOnLine); Serial.print(" R:"); Serial.println(rightOnLine);

  if (leftOnLine && rightOnLine) {
    // On line (both sensors): drive straight
    searchStart = 0;
    int leftSpeed = baseSpeed;
    int rightSpeed = baseSpeed;
    drive(leftSpeed, rightSpeed);
    lastKnownDir = DIR_NONE; // We're centered
  } else if (leftOnLine && !rightOnLine) {
    // Line seen on left only -> turn left to recenter
    searchStart = 0;
    lastKnownDir = DIR_LEFT;
    int leftSpeed = int(baseSpeed * turnSpeedFactor);  // slow inner wheel
    int rightSpeed = baseSpeed;                        // outer wheel faster
    drive(leftSpeed, rightSpeed);
  } else if (!leftOnLine && rightOnLine) {
    // Line seen on right only -> turn right to recenter
    searchStart = 0;
    lastKnownDir = DIR_RIGHT;
    int leftSpeed = baseSpeed;                        // outer wheel faster
    int rightSpeed = int(baseSpeed * turnSpeedFactor); // slow inner wheel
    drive(leftSpeed, rightSpeed);
  } else {
    // Both sensors off-line: stop motors
    searchStart = 0;
    lastKnownDir = DIR_NONE;
    stopMotors();
    // Uncomment to debug
    // Serial.println("No line detected - motors stopped");
  }

  // Small delay to let motors react and avoid excessive loop speed
  delay(10);
}