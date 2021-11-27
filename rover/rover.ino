

//  #####################################
//  #   Using the ArduinoJson library   #
//  #####################################
//
// https://arduinojson.org/
// Installed through the built in library manager
//
// Commands as json blobs
// Each command may contain one or more of
//  L: turn speed (with direction given as sign)
//  R: turn speed (with direction given as sign)
//  prettify_output: bool -- toggle pretty json output from arduino (inserts line breaks)
//
// Example commands
//  {} -- maintain current state
//  {"A": 0.2, "B": -0.2} -- Left speed 0.2 and right speed backwards 0.2
//  {"A": 0, "B": 0.5} -- Right speed 0 and left speed 0.5
// where speed +-1 is max speed. -- perhaps change this
//

#include <ArduinoJson.h>

#define enA 10
#define enB 11

#define in1 2
#define in2 4
#define in3 5
#define in4 7

#define FRONT_ULTRASOUND_PING_PIN 8
#define FRONT_ULTRASOUND_ECHO_PIN 9

#define default_prettify_output 0
#define baud 115200

#define MAX_SPEED 255
//#define min_speed 50

struct STATE {
  int A;
  bool updateA;
  int B;
  bool updateB;
  String message;
  int json_depth;
  bool prettify_output;
  int centimeters;
};

STATE state = {
  0,
  true,
  0,
  false,
  "", 
  0,
  default_prettify_output,
  0,
};


void setup() {
  Serial.begin(baud);
  while (!Serial) continue;

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(FRONT_ULTRASOUND_PING_PIN, OUTPUT);
  pinMode(FRONT_ULTRASOUND_ECHO_PIN, INPUT);


  StaticJsonDocument<60> doc;
  doc["message"] = "Arduino ready";
  serialize_json(&doc);
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

    if (state.json_depth <= 0) {
      if (state.message.length() > 1) {
        DynamicJsonDocument received(256);
        DeserializationError error = deserializeJson(received, state.message);
        
        DynamicJsonDocument response(512);
        
        if (error) {
          response["type"] = "parse-error";
          JsonObject data = response.createNestedObject("data");
          
          data["error"] = error.c_str();
          data["input_message"] = state.message;
          data["input_length"] = state.message.length();
          
        } else {
          response["type"] = "state-update";
          JsonArray data = response.createNestedArray("data");
          
          char key[] = "A";
          if (received.containsKey(key)) {
            float rotational_speed = received[key];
            rotational_speed *= MAX_SPEED;
            int new_speed = (int)rotational_speed;
            
            state.A = new_speed;
            state.updateA = true;
            
            JsonObject update_info = data.createNestedObject();
            update_info["device"] = "A";
            update_info["command"] = state.A; 
          }
  
          strcpy(key, "B");
          if (received.containsKey(key)) {
            float rotational_speed = received[key];
            rotational_speed *= MAX_SPEED;
            int new_speed = (int)rotational_speed;

            state.B = new_speed;
            state.updateB = true;
            
            JsonObject update_info = data.createNestedObject();
            update_info["device"] = "B";
            update_info["command"] = state.B; 
          } 

          // Set distance in centimeters to return

          long cm = get_centimeters_to_obstacle(FRONT_ULTRASOUND_PING_PIN, FRONT_ULTRASOUND_ECHO_PIN);
          JsonObject distance_info = data.createNestedObject();
          distance_info["centimeters"] = cm;
        }
        
        state.message = "";
        serialize_json(&response);
      }
    }
  }
}

void serialize_json(const JsonDocument *obj) {
  if (state.prettify_output) {
    serializeJsonPretty(*obj, Serial);
  } else {
    serializeJson(*obj, Serial);
    Serial.println("");
  }
}

void update_engine(int motor_pin, int pin1, int pin2, int new_speed) {
  if (new_speed < 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  } else {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  }
  int abs_speed = abs(new_speed);
  abs_speed = abs_speed > MAX_SPEED ? MAX_SPEED : abs_speed;
  
  analogWrite(motor_pin, abs_speed);
}
  
void keep_moving(){
  if (state.updateA) {
    state.updateA = false;
    update_engine(enA, in1, in2, state.A);
  }
  
  if (state.updateB) {
    state.updateB = false;
    update_engine(enB, in3, in4, state.B);
  }
}

// Ultrasound helpers

long get_centimeters_to_obstacle(int pingPin, int echoPin) {
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

