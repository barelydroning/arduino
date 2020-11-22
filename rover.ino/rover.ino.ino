

//  #####################################
//  #   Using the ArduinoJson library   #
//  #####################################
//
// https://arduinojson.org/
// Installed through the built in library manager
//
// Commands as json blobs (I don't think linebreaks are acceptable)
// Each command may contain one or more of
//  servo: target_angle
//  L: turn speed (with direction given as sign)
//  R: turn speed (with direction given as sign)
//
// Example commands
//  {} -- maintain current state
//  {"A": 0.2, "B": -0.2} -- Left speed 0.2 and right speed backwards 0.2
//  {"A": 0, "B": 0.5} -- Right speed 0 and left speed 0.5
// where speed +-1 is max speed. -- perhaps change this
//

#include <ArduinoJson.h>

#define enA 2
#define enB 7

#define in1 3
#define in2 4
#define in3 5
#define in4 6

#define MAX_SPEED 255
//#define min_speed 50

struct STATE {
  int A;
  int B;
  String message;
  int json_depth;
};

STATE state = {
  0,
  0,
  "", 
  0,
};


void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  StaticJsonDocument<60> doc;
  doc["message"] = "Arduino ready";
  serializeJsonPretty(doc, Serial);
}

void loop() {
  update_state();
  keep_moving();
}

void update_state() {
  if (Serial.available() > 0) {
    int new_byte = Serial.read();

    switch (new_byte) {
      case '{':
        state.json_depth++;
        break;
      case '}':
        state.json_depth--;
        break;
    }
    state.message += (char)new_byte;

    if (state.json_depth == 0) {
      if (state.message.length() > 1) {
        DynamicJsonDocument received(256);
        DeserializationError error = deserializeJson(received, state.message);
  
        if (error) {
          Serial.println("Failed to parse json");
          Serial.print("  with error: ");
          Serial.println(error.c_str());
          Serial.print("  on message len=");
          Serial.println(state.message.length());
          Serial.println(state.message);
        } else {
          char key[] = "A";
          if (received.containsKey(key)) {
            float rotational_speed = received[key];
            rotational_speed *= MAX_SPEED;
            state.A = (int)rotational_speed;
            Serial.println("Setting A speed");
          }
  
          strcpy(key, "B");
          if (received.containsKey(key)) {
            float rotational_speed = received[key];
            rotational_speed *= MAX_SPEED;
            state.B = (int)rotational_speed;
            Serial.println("Setting B speed");
          }
        }
        state.message = "";
      }
    }
  }
}

void keep_moving(){
  if (state.A < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  if (state.B < 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  
  analogWrite(enA, abs(state.A)); // Send PWM signal to motor A
  analogWrite(enB, abs(state.B)); // Send PWM signal to motor A
}
