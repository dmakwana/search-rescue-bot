#include <stdint.h>
#include <Servo.h>
#include <SoftwareSerial.h>  

#define rollNeutral 1484
#define throttleMin 1880
#define yawNeutral 1464
#define pitchNeutral 1452
#define rollMin 1086 
#define yawMin 1052
#define pitchMin 1048
#define rollMax 1896
#define throttleMax 1056
#define yawMax 1868
#define pitchMax 1856

#define rollPin 3
#define pitchPin 5
#define throttlePin 6
#define yawPin 9
#define bluetoothTx = 2;
#define bluetoothRx = 4;

#define sensitivity 5
#define keyA 97 // A
#define keyS 115 // S
#define keyD 100 // D
#define keyW 119 // W
#define keySpace 32 // " "
#define keyC 99 // C
#define keyN 110 // N
#define keyM 109 // M
#define keyP 112 // P
#define arm 48 // 0
#define unarm 49 // 1
#define throttle0 56 // 8
#define reset 114 // r

int serialData = 0;
Servo pitchPWM;
Servo rollPWM;
Servo yawPWM;
Servo throttlePWM;
int pitch;
int roll;
int throttle;
int yaw;
bool changed = false;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setNeutral(){
  yaw = yawNeutral;
  pitch = pitchNeutral;
  roll = rollNeutral;
  throttle = throttleMin;
}

void setup() {
  setNeutral();
  changed = true;
  rollPWM.attach(rollPin);
  pitchPWM.attach(pitchPin);
  throttlePWM.attach(throttlePin);
  yawPWM.attach(yawPin);
//setPwmFrequency(throttlePin,128);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    serialData = Serial.read();
    if (serialData == throttle0){
      throttle = 0;
      changed = true;
    }
    else if (serialData == unarm){
      pitch = pitchMin;
      roll = rollMax;
      changed = true;
    }
    else if (serialData == arm){
      pitch = pitchMin;
      roll = rollMin;
      changed = true;
    }
    else if (serialData == reset){
      setNeutral();
      changed = true;
    }
    else if (serialData == keyA){
      if (roll-sensitivity>rollMin){
        roll -= sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyD){
      if (roll+sensitivity<=rollMax){
        roll += sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyS){
      if (pitch-sensitivity>=pitchMin){
        pitch -= sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyW){
      if (pitch+sensitivity<=pitchMax){
        pitch += sensitivity;
        changed = true;
      }
    }
    else if (serialData == keySpace){
      if (throttle-sensitivity<=throttleMin){
        throttle -= sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyC){
      if (throttle+sensitivity>=throttleMax){
        throttle += sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyN){
      if (yaw-sensitivity>=yawMin){
        yaw -= sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyM){
      if (yaw+sensitivity<=yawMax){
        yaw += sensitivity;
        changed = true;
      }
    }
    else if (serialData == keyP){
      Serial.print("Pitch: \t");
      Serial.println(pitch);
      Serial.print("Roll: \t");
      Serial.println(roll);
      Serial.print("throttle: ");
      Serial.println(throttle);
      Serial.print("Yaw: \t");
      Serial.println(yaw);
      Serial.println("\n");
    }
  }
  if (changed == true){
    
    rollPWM.write(roll);
    pitchPWM.write(pitch);
    throttlePWM.write(throttle);
    yawPWM.write(yaw);
    changed = false;
  }
}


