#define ULTRASOUND_PING_PIN 8
#define ULTRASOUND_ECHO_PIN 9

void setup() {
  Serial.begin(115200); // Starting Serial Terminal
  while (!Serial) continue;

  pinMode(ULTRASOUND_PING_PIN, OUTPUT);
  pinMode(ULTRASOUND_ECHO_PIN, INPUT);

}

void loop() {
  long cm = get_centimeters_to_obstacle();
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
}

long get_centimeters_to_obstacle() {
  digitalWrite(ULTRASOUND_PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASOUND_PING_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND_PING_PIN, LOW);

  long duration = pulseIn(ULTRASOUND_ECHO_PIN, HIGH);
  return microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}