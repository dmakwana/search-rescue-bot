#include <stdint.h>
#include <Servo.h>
#include <SoftwareSerial.h>  

#define rollNeutral 1500
#define throttleMax 2000
#define yawNeutral 1500
#define pitchNeutral 1500
#define rollMin 1000 
#define yawMin 1000
#define pitchMin 1000
#define rollMax 2000
#define throttleMin 1000
#define yawMax 2000
#define pitchMax 2000
#define flightMode1 1100
#define flightMode2 1500
#define flightMode3 1900

#define rollPin 3
#define pitchPin 5
#define throttlePin 6
#define yawPin 9
#define flightPin 10
#define bluetoothTx 2
#define bluetoothRx 4

#define sensitivity 25
#define keyA 97 // A
#define keyS 115 // S
#define keyD 100 // D
#define keyW 119 // W
#define keySpace 32 // " "
#define keyC 99 // C
#define keyN 110 // N
#define keyM 109 // M
#define keyP 112 // P
#define key2 50
#define key3 51
#define arm 48 // 0
#define unarm 49 // 1
#define throttle0 56 // 8
#define reset 114 // r

int serialData = 0;
Servo pitchPWM;
Servo rollPWM;
Servo yawPWM;
Servo throttlePWM;
Servo flightPWM;
int pitch;
int roll;
int throttle;
int yaw;
int flight;
bool changed = false;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setNeutral(){
  yaw = yawNeutral;
  pitch = pitchNeutral;
  roll = rollNeutral;
  throttle = throttleMin;
  flight = flightMode1;
}

//void bluetoothConfig(){
//}

void setup() {
  setNeutral();
  
  changed = true;
  rollPWM.attach(rollPin);
  pitchPWM.attach(pitchPin);
  throttlePWM.attach(throttlePin);
  yawPWM.attach(yawPin);
  flightPWM.attach(flightPin);
  
  Serial.begin(9600);
  //bluetoothConfig();
  bluetooth.begin(115200);  // The Serial Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start Serial serial at 9600
  bluetooth.print("start");
}

void loop() {
  if (bluetooth.available() > 0)
  {
    serialData = bluetooth.read();
    if (serialData == throttle0){
      throttle = throttleMin;
      changed = true;
    }
    else if (serialData == unarm){
      throttle = throttleMin;
      roll = rollMax;
      changed = true;
    }
    else if (serialData == arm){
      throttle = throttleMin;
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
      else{
        roll=rollMin;
      }
    }
    else if (serialData == keyD){
      if (roll+sensitivity<=rollMax){
        roll += sensitivity;
        changed = true;
      }
      else{
        roll=rollMax;
      }
    }
    else if (serialData == keyS){
      if (pitch-sensitivity>=pitchMin){
        pitch -= sensitivity;
        changed = true;
      }
      else{
        pitch=pitchMin;
      }
    }
    else if (serialData == keyW){
      if (pitch+sensitivity<=pitchMax){
        pitch += sensitivity;
        changed = true;
      }
      else{
        pitch=pitchMax;
      }
    }
    else if (serialData == keyC){
      if (throttle-sensitivity>=throttleMin){
        throttle -= sensitivity;
        changed = true;
      }
      else{
        throttle=throttleMin;
      }
    }
    else if (serialData == keySpace){
      if (throttle+sensitivity<=throttleMax){
        throttle += sensitivity;
        changed = true;
      }
      else{
        throttle=throttleMax;
      }
    }
    else if (serialData == keyN){
      if (yaw-sensitivity>=yawMin){
        yaw -= sensitivity;
        changed = true;
      }
      else{
        yaw=yawMin;
      }
    }
    else if (serialData == keyM){
      if (yaw+sensitivity<=yawMax){
        yaw += sensitivity;
        changed = true;
      }
      else{
        yaw=yawMax;
      }
    }
    else if (serialData == key2){
      if (flight-25>=flightMode1){
        flight -= 25;
        changed = true;
      }
    }
    else if (serialData == key3){
      if (flight+25<=flightMode2){
        flight += 25;
        changed = true;
      }
    }
    else if (serialData == keyP){
      bluetooth.print("Pitch: \t");
      bluetooth.println(pitch);
      bluetooth.print("Roll: \t");
      bluetooth.println(roll);
      bluetooth.print("throttle: ");
      bluetooth.println(throttle);
      bluetooth.print("Yaw: \t");
      bluetooth.println(yaw);
      bluetooth.println("\n");
      bluetooth.print("Flight: \t");
      bluetooth.println(flight);
      bluetooth.println("\n");
    }
  }
  if (changed == true){
    
    rollPWM.writeMicroseconds(roll);
    pitchPWM.writeMicroseconds(pitch);
    throttlePWM.writeMicroseconds(throttle);
    yawPWM.writeMicroseconds(yaw);
    flightPWM.writeMicroseconds(flight);
    changed = false;
  }
}


