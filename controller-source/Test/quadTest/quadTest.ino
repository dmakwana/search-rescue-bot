#include <stdint.h>
#include <Servo.h>

#define keyA 97
#define keyS 115
#define keyD 100
#define keyW 119
#define keySpace 32
#define keyC 99
#define keyN 110
#define keyM 109
#define keyP 112
#define sensitivity 5
#define ailPin 3
#define elePin 5
#define thrustPin 6
#define rudderPin 9

int serialData = 0;
Servo pitchPPM;
Servo rollPPM;
Servo yawPPM;
Servo thrustPPM;
uint8_t pitch;
uint8_t roll;
uint8_t elevation;
uint8_t yaw;
bool changed = false;

//void setPwmFrequency(int pin, int divisor) {
//  byte mode;
//  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
//    switch(divisor) {
//      case 1: mode = 0x01; break;
//      case 8: mode = 0x02; break;
//      case 64: mode = 0x03; break;
//      case 256: mode = 0x04; break;
//      case 1024: mode = 0x05; break;
//      default: return;
//    }
//    if(pin == 5 || pin == 6) {
//      TCCR0B = TCCR0B & 0b11111000 | mode;
//    } else {
//      TCCR1B = TCCR1B & 0b11111000 | mode;
//    }
//  } else if(pin == 3 || pin == 11) {
//    switch(divisor) {
//      case 1: mode = 0x01; break;
//      case 8: mode = 0x02; break;
//      case 32: mode = 0x03; break;
//      case 64: mode = 0x04; break;
//      case 128: mode = 0x05; break;
//      case 256: mode = 0x06; break;
//      case 1024: mode = 0x7; break;
//      default: return;
//    }
//    TCCR2B = TCCR2B & 0b11111000 | mode;
//  }
//}

void setup() {
  pitch = 128;
  roll = 128;
  elevation = 255;
  yaw = 128;
  rollPPM.attach(ailPin);
  pitchPPM.attach(elePin);
  thrustPPM.attach(thrustPin);
  yawPPM.attach(rudderPin);
//setPwmFrequency(thrustPin,128);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    serialData = Serial.read();
    if (serialData == keyA){
      if (roll-sensitivity>0){
        roll -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keyD){
      if (roll+sensitivity<=255){
        roll += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyS){
      if (pitch-sensitivity>=0){
        pitch -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keyW){
      if (pitch+sensitivity<=255){
        pitch += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyC){
      if (elevation-sensitivity>=0){
        elevation -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keySpace){
      if (elevation+sensitivity<=255){
        elevation += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyN){
      if (yaw-sensitivity>=0){
        yaw -= sensitivity;
        changed = true;
      }
    }
    if (serialData == keyM){
      if (yaw+sensitivity<=255){
        yaw += sensitivity;
        changed = true;
      }
    }
    if (serialData == keyP){
      Serial.print("Pitch: \t");
      Serial.println(pitch);
      Serial.print("Roll: \t");
      Serial.println(roll);
      Serial.print("Thrust: ");
      Serial.println(elevation);
      Serial.print("Yaw: \t");
      Serial.println(yaw);
    }
  }
  if (changed == true){
    
    rollPPM.write(map(roll, 0, 255, 0, 179));
    pitchPPM.write(map(pitch, 0, 255, 0, 179));
    thrustPPM.write(map(elevation, 0, 255, 0, 179));
    yawPPM.write(map(yaw, 0, 255, 0, 179));
    changed = false;
  }
}
