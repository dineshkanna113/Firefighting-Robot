#include <DabbleESP32.h> 
#include <ESP32Servo.h>

// Motor pins
const int enableRightMotor = 22;
const int rightMotorPin1 = 16;
const int rightMotorPin2 = 17;

const int enableLeftMotor = 23;
const int leftMotorPin1 = 18;
const int leftMotorPin2 = 19;

// Motor speed constants
#define MAX_MOTOR_SPEED 150
const int PWMFreq = 1000; // 1 KHz
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 4;

// Flame sensor pins
#define FLAME_SENSOR1_PIN 34
#define FLAME_SENSOR2_PIN 35
#define FLAME_SENSOR3_PIN 36

// Water pump and servo motor
#define WATER_PUMP_PIN 14
#define SERVO_PIN 15
Servo waterServo;

// Flame sensor readings
int flame1, flame2, flame3;

// Servo position
int servoAngle = 90; // Default center position
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

// Initialize pins and motor settings
void initializePins() {
  // Motor pins
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Flame sensor pins
  pinMode(FLAME_SENSOR1_PIN, INPUT);
  pinMode(FLAME_SENSOR2_PIN, INPUT);
  pinMode(FLAME_SENSOR3_PIN, INPUT);

  // Water pump
  pinMode(WATER_PUMP_PIN, OUTPUT);
  digitalWrite(WATER_PUMP_PIN, LOW); // Ensure pump is off initially

  // PWM setup
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel);

  // Servo setup
  waterServo.attach(SERVO_PIN);
  waterServo.write(servoAngle); // Start at default position
}

// Motor control function
void controlMotors(int rightMotorSpeed, int leftMotorSpeed) {
  // Right motor direction
  if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  // Left motor direction
  if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  // Set motor speeds
  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));
}

// Move towards fire based on flame sensor input
void moveTowardFire() {
  if (flame1 == LOW) { 
    controlMotors(MAX_MOTOR_SPEED, MAX_MOTOR_SPEED); // Move forward
  } else if (flame2 == LOW) { 
    controlMotors(MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED); // Turn left
  } else if (flame3 == LOW) { 
    controlMotors(-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED); // Turn right
  } else {
    controlMotors(0, 0); // Stop
  }
}

// Activate water pump and control servo direction
void activateWaterPump() {
  digitalWrite(WATER_PUMP_PIN, HIGH);

  // Automatic servo control based on flame sensors
  if (flame1 == LOW) {
    servoAngle = 90; // Center (front-facing) direction
    Serial.println("Spraying forward");
  } else if (flame2 == LOW) {
    servoAngle = 45; // Turn left
    Serial.println("Spraying left");
  } else if (flame3 == LOW) {
    servoAngle = 135; // Turn right
    Serial.println("Spraying right");
  }
  waterServo.write(servoAngle);
}

// Stop water pump
void stopWaterPump() {
  digitalWrite(WATER_PUMP_PIN, LOW);
}

// Main setup function
void setup() {
  Serial.begin(115200);
  initializePins();
  controlMotors(0, 0); // Ensure motors are stopped
  Dabble.begin("FireFighterRobot");
}

// Main loop function
void loop() {
  // Read flame sensor values
  flame1 = digitalRead(FLAME_SENSOR1_PIN);
  flame2 = digitalRead(FLAME_SENSOR2_PIN);
  flame3 = digitalRead(FLAME_SENSOR3_PIN);

  // Debug flame sensor readings
  Serial.print("Flame1: "); Serial.print(flame1);
  Serial.print(", Flame2: "); Serial.print(flame2);
  Serial.print(", Flame3: "); Serial.println(flame3);

  // Manual control via Dabble
  int rightMotorSpeed = 0, leftMotorSpeed = 0;
  Dabble.processInput();

  // Manual motor control
  if (GamePad.isUpPressed()) {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  } else if (GamePad.isDownPressed()) {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  } else if (GamePad.isLeftPressed()) {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  } else if (GamePad.isRightPressed()) {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  // Manual servo control
  if (GamePad.isCirclePressed()) {
    servoAngle += 5; // Move servo right
    if (servoAngle > SERVO_MAX_ANGLE) servoAngle = SERVO_MAX_ANGLE;
    waterServo.write(servoAngle);
    Serial.print("Manual Servo Angle: "); Serial.println(servoAngle);
  } else if (GamePad.isSquarePressed()) {
    servoAngle -= 5; // Move servo left
    if (servoAngle < SERVO_MIN_ANGLE) servoAngle = SERVO_MIN_ANGLE;
    waterServo.write(servoAngle);
    Serial.print("Manual Servo Angle: "); Serial.println(servoAngle);
  }

  // Automatic control if no manual input
  if (rightMotorSpeed == 0 && leftMotorSpeed == 0) {
    if (flame1 == LOW || flame2 == LOW || flame3 == LOW) {
      moveTowardFire();
      activateWaterPump();
    } else {
      stopWaterPump();
      controlMotors(0, 0); // Stop motors
    }
  } else {
    controlMotors(rightMotorSpeed, leftMotorSpeed); // Manual control
  }
}
