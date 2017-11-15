
#include <Servo.h>
#include <PID_v1.h>

Servo MOTOR_ONE;
Servo MOTOR_TWO;
Servo MOTOR_THREE;
Servo MOTOR_FOUR;
Servo MOTOR_FIVE;
Servo MOTOR_SIX;



int PIN_MOTOR_ONE = 2;
int PIN_MOTOR_TWO = 11;
int PIN_MOTOR_THREE = 10;
int PIN_MOTOR_FOUR = 13;
int PIN_MOTOR_FIVE = 6;
int PIN_MOTOR_SIX = 12;

double pitchSetpoint, pitchInput, pitchOutput;

PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, 100, 0, 0, DIRECT);

double rollSetpoint, rollInput, rollOutput;

PID rollPID(&rollInput, &rollOutput, &rollSetpoint, 100, 0, 0, DIRECT);

void arm(Servo motor) {

  setSpeed(1000, motor);

}

void setSpeed(int speed, Servo motor) {

  //int angle = map(speed, 0, 100, 0, 180); //Sets servo positions to different speeds
  //motor.write(speed);
  motor.writeMicroseconds(speed);

}

String inputString = "";
boolean stringComplete = false;

void setup() {

  // Engines
  MOTOR_ONE.attach(PIN_MOTOR_ONE); //Adds ESC to certain pin.
  MOTOR_TWO.attach(PIN_MOTOR_TWO);
  MOTOR_THREE.attach(PIN_MOTOR_THREE);
  MOTOR_FOUR.attach(PIN_MOTOR_FOUR);
  MOTOR_FIVE.attach(PIN_MOTOR_FIVE);
  MOTOR_SIX.attach(PIN_MOTOR_SIX);
  

  arm(MOTOR_ONE);
  arm(MOTOR_TWO);
  arm(MOTOR_THREE);
  arm(MOTOR_FOUR);
  arm(MOTOR_FIVE),
  arm(MOTOR_SIX);

  // pids
  pitchSetpoint = 0;
  pitchInput = 0;
  pitchOutput = 0;

  pitchPID.SetMode(AUTOMATIC);
  pitchPID.SetOutputLimits(-1000.0, 1000.0);

  rollSetpoint = 0;
  rollInput = 0;
  //rollOutput = 0;

  rollPID.SetMode(AUTOMATIC);
  rollPID.SetOutputLimits(10000.0, 1000.0);

  delay(3000);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

int motorOneSpeed = 1000;
int motorTwoSpeed = 1000;
int motorThreeSpeed = 1000;
int motorFourSpeed = 1000;
int motorFiveSpeed = 1000;
int motorSixSpeed = 1000;

int MOTOR_INCREMENT = 25;

float minmaxSpeed(float val) {
  return min(max(val, 1000), 2000);
}

int TEST_SPEED = 1100;

void loop() {
  while (Serial.available()) {

    String str = Serial.readStringUntil('T');

    int motorSpeed = str.substring(1, str.length()).toInt();

    String motor = str.substring(0, 1);

    //Serial.println("Motor: " + motor + " speed: " + String(motorSpeed));

    if (motor.equals("A")) {
      setSpeed(motorSpeed, MOTOR_ONE);
      Serial.println("Motor A: " + String(motorSpeed));
    } else if (motor.equals("B")) {
      setSpeed(motorSpeed, MOTOR_TWO);
      Serial.println("Motor B: " + String(motorSpeed));
    } else if (motor.equals("C")) {
      setSpeed(motorSpeed, MOTOR_THREE);
      Serial.println("Motor C: " + String(motorSpeed));
    } else if (motor.equals("D")) {
      setSpeed(motorSpeed, MOTOR_FOUR);
      Serial.println("Motor D: " + String(motorSpeed));
    } else if (motor.equals("E")) {
      setSpeed(motorSpeed, MOTOR_FIVE);
      Serial.println("Motor E: " + String(motorSpeed));
    } else if (motor.equals("F")) {
      setSpeed(motorSpeed, MOTOR_SIX);
      Serial.println("Motor F: " + String(motorSpeed));
    } else if (motor.equals("Q")) {
      setSpeed(motorSpeed, MOTOR_ONE);
      setSpeed(motorSpeed, MOTOR_TWO);
      setSpeed(motorSpeed, MOTOR_THREE);
      setSpeed(motorSpeed, MOTOR_FOUR);
      setSpeed(motorSpeed, MOTOR_FIVE);
      setSpeed(motorSpeed, MOTOR_SIX);

      Serial.println("All motors: " + String(motorSpeed));
    }

  }
}