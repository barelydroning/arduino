
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

void loop() {
  while (Serial.available()) {

    String str = Serial.readStringUntil('T');

    int motorOneSpeed = str.substring(0, str.indexOf('A')).toInt();
    int motorTwoSpeed = str.substring(str.indexOf('A') + 1, str.indexOf('B')).toInt();
    int motorThreeSpeed = str.substring(str.indexOf('B') + 1, str.indexOf('C')).toInt();
    int motorFourSpeed = str.substring(str.indexOf('C') + 1, str.indexOf('D')).toInt();
    int motorFiveSpeed = str.substring(str.indexOf('D') + 1, str.indexOf('E')).toInt();
    int motorSixSpeed = str.substring(str.indexOf('E') + 1, str.indexOf('F')).toInt();

    setSpeed(motorOneSpeed, MOTOR_ONE);
    setSpeed(motorTwoSpeed, MOTOR_TWO);
    setSpeed(motorThreeSpeed, MOTOR_THREE);
    setSpeed(motorFourSpeed, MOTOR_FOUR);
    setSpeed(motorFiveSpeed, MOTOR_FIVE);
    setSpeed(motorSixSpeed, MOTOR_SIX);

  }
}