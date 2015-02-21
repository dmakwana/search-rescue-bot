#include <stdint.h>

#define keyA 97
#define keyS 115
#define keyD 100
#define keyW 119
#define keySpace 32
#define keyC 99
#define keyP 112
#define sensitivity 1
#define rollPin 3
#define thrustPin 5
#define elevationPin 6

int serialData = 0;
uint8_t pitch;
uint8_t roll;
uint8_t elevation;


void setup() {
  pitch = 128;
  roll = 128;
  elevation = 0;
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    serialData = Serial.read();
    if (serialData == keyA){
      if (roll>0){
        roll -= sensitivity;
      }
    }
    if (serialData == keyD){
      if (roll<255){
        roll += sensitivity;
      }
    }
    if (serialData == keyS){
      if (pitch>0){
        pitch -= sensitivity;
      }
    }
    if (serialData == keyW){
      if (pitch<255){
        pitch += sensitivity;
      }
    }
    if (serialData == keyC){
      if (elevation>0){
        elevation -= sensitivity;
      }
    }
    if (serialData == keySpace){
      if (elevation<255){
        elevation += sensitivity;
      }
    }
    if (serialData == keyP){
      Serial.println(pitch);
      Serial.println(roll);
      Serial.println(elevation);
    }
  }
  analogWrite(rollPin, roll);
  analogWrite(pitch, pitch);
  analogWrite(elevationPin, elevation);
}
