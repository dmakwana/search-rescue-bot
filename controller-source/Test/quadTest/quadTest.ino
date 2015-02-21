#include <stdint.h>

#define keyA 97
#define keyS 115
#define keyD 100
#define keyW 119
#define keySpace 32
#define keyC 99
#define keyP 112
#define sensitivity 1
#define ailPin 3
#define elePin 5
#define thrustPin 6

int serialData = 0;
uint8_t pitch;
uint8_t roll;
uint8_t elevation;
bool changed = false;


void setup() {
  pitch = 128;
  roll = 128;
  elevation = 0;
  pinMode(ailPin,OUTPUT);
  pinMode(elePin,OUTPUT);
  pinMode(thrustPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    serialData = Serial.read();
    if (serialData == keyA){
      if (roll>0){
        roll -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keyD){
      if (roll<255){
        roll += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyS){
      if (pitch>0){
        pitch -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keyW){
      if (pitch<255){
        pitch += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyC){
      if (elevation>0){
        elevation -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keySpace){
      if (elevation<255){
        elevation += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyP){
      Serial.println(pitch);
      Serial.println(roll);
      Serial.println(elevation);
    }
  }
  if (changed == true){
    analogWrite(ailPin, roll);
    analogWrite(elePin, pitch);
    analogWrite(thrustPin, elevation);
    changed = false;
  }
}
