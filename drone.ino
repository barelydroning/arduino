
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

PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, 60, 400, 10, P_ON_E, DIRECT);

double rollSetpoint, rollInput, rollOutput;

PID rollPID(&rollInput, &rollOutput, &rollSetpoint, 60, 400, 10, P_ON_E, DIRECT);

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
  rollPID.SetOutputLimits(-1000.0, 1000.0);

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

int BASE_SPEED = 1400;

void loop() {
  while (Serial.available()) {

    String str = Serial.readStringUntil('T');


    int pitchEndIndex = str.indexOf('P');
    int rollEndIndex = str.indexOf('R');

    String pitchString = str.substring(0, pitchEndIndex);
    String rollString = str.substring(pitchEndIndex + 1, rollEndIndex);
    String motorSpeedString = str.substring(rollEndIndex + 1, str.length());

    float pitch = str.substring(0, pitchEndIndex).toFloat();
    float roll = str.substring(pitchEndIndex + 1, rollEndIndex).toFloat();
    int motorSpeed = str.substring(rollEndIndex + 1, str.length()).toInt();

    //Serial.println("Pitch: " + pitchString + ": %.3f", &pitch );
    //Serial.println("Roll: " + rollString);
    //Serial.println("Motor speed: " + motorSpeedString);

    pitchInput = pitch;
    rollInput = roll;

    pitchPID.Compute();
    rollPID.Compute();

    //pitchOutput = 100*(0.0 - pitch);
    //pitchOutput = 0;

    motorOneSpeed = pitchOutput;
    motorFiveSpeed = pitchOutput;
    motorTwoSpeed = -pitchOutput;
    motorFourSpeed = -pitchOutput;

    motorOneSpeed -= rollOutput;
    motorTwoSpeed -= rollOutput;
    motorFourSpeed += rollOutput;
    motorFiveSpeed += rollOutput;

    motorOneSpeed += BASE_SPEED;
    motorTwoSpeed += BASE_SPEED;
    motorFourSpeed += BASE_SPEED;
    motorFiveSpeed += BASE_SPEED;

    motorThreeSpeed = BASE_SPEED;
    motorSixSpeed = BASE_SPEED;

    motorOneSpeed = minmaxSpeed(motorOneSpeed);
    motorTwoSpeed = minmaxSpeed(motorTwoSpeed);
    motorThreeSpeed = minmaxSpeed(motorThreeSpeed);
    motorFourSpeed = minmaxSpeed(motorFourSpeed);

    // Serial.println("MOTOR_ONE: " + String(motorOneSpeed));
    // Serial.println("MOTOR_TWO: " + String(motorTwoSpeed));
    // Serial.println("MOTOR_THREE: " + String(motorThreeSpeed));
    // Serial.println("MOTOR_FOUR: " + String(motorFourSpeed));

    setSpeed(motorOneSpeed, MOTOR_ONE);
    setSpeed(motorTwoSpeed, MOTOR_TWO);
    setSpeed(motorThreeSpeed, MOTOR_THREE);
    setSpeed(motorFourSpeed, MOTOR_FOUR);
    setSpeed(motorFiveSpeed, MOTOR_FIVE);
    setSpeed(motorSixSpeed, MOTOR_SIX);

  }
}
