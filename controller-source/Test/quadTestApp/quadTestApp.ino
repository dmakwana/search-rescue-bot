#include <stdint.h>
#include <Servo.h> 

int rollNeutral = 1500;
#define throttleMax 2000
int yawNeutral = 1500;
int pitchNeutral = 1500;
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
#define SerialTx 2
#define SerialRx 4
#define battery 1

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
#define bluetooth Serial1
#define negTr -2
#define posTr 2

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
char cmd[100];
int cmdIndex;
String st;
int tr;
char c;

int getVal() {
  
  int val = 0;
       for(int i=2; cmd[i]!=0; i++) { // number begins at cmd[6]
         val = val*10 + (cmd[i]-'0');
       }
       // if cmd is "speed 100", val will be 100        
       //analogWrite(motorSpeedPin, val);
       return val; 
}

//SoftwareSerial Serial(SerialTx, SerialRx);

void setNeutral(){
  yaw = yawNeutral;
  pitch = pitchNeutral;
  roll = rollNeutral;
  throttle = throttleMin;
  flight = flightMode1;
}

//void SerialConfig(){
//}

void setup() {
  setNeutral();
  
  changed = true;
  rollPWM.attach(rollPin);
  pitchPWM.attach(pitchPin);
  throttlePWM.attach(throttlePin);
  yawPWM.attach(yawPin);
  flightPWM.attach(flightPin);
  
  delay(1000);  // Short delay, wait for the Mate to send back CMD
  Serial.begin(115200);
  bluetooth.begin(115200);
  cmdIndex = 0; 
}

void loop() {
  if (bluetooth.available() > 0)
  {
    c = (char)bluetooth.read();
    if(c=='\n') {
      cmd[cmdIndex] = 0;
      
      if (cmd[0] == (char)unarm){
        throttle = throttleMin;
        roll = rollMax;
        changed = true;
        Serial.println("UN-ARM!");
      }
      else if (cmd[0] == (char)arm){
        throttle = throttleMin;
        roll = rollMin;
        changed = true;
        Serial.println("ARM!");
      }
      else if (cmd[0] == (char)reset){
        setNeutral();
        changed = true;
        Serial.println("RESET!");
      }
      else if (cmd[0] == (char)keyS){
        throttle = getVal();
        Serial.print("Throttle:");
        Serial.println(throttle);
        changed = true;
      }
      else if (cmd[0] == 'p'){
        if (cmd[1] == '+'){
          tr=posTr;
        }
        else if (cmd[1] == '-'){
          tr=negTr;
        }
        pitchNeutral+=tr;
        st = (String)"pt " + pitchNeutral + "\n";
        bluetooth.print(st);
        Serial.print("pitch trim: ");
        Serial.println(pitchNeutral);  
      }
      else if (cmd[0] == 't'){
        if (cmd[1] == '+'){
          tr=posTr;
        }
        else if (cmd[1] == '-'){
          tr=negTr;
        }
        rollNeutral+=tr;
        st = (String)"rt " + rollNeutral + "\n";
        bluetooth.print(st);
        Serial.print("roll trim: ");
        Serial.println(rollNeutral);  
      }
      else if (cmd[0] == 'y'){
        if (cmd[1] == '+'){
          tr=posTr;
        }
        else if (cmd[1] == '-'){
          tr=negTr;
        }
        yawNeutral+=tr;
        st = (String)"yt " + yawNeutral + "\n";
        bluetooth.print(st);
        Serial.print("yaw trim: ");
        Serial.println(yawNeutral);  
      }
      
      cmdIndex = 0; // reset the cmdIndex
      
    } else {      
      cmd[cmdIndex] = c;
      if(cmdIndex<99) cmdIndex++;
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
  float batteryLevel=(analogRead(battery)*15.0)/1024;
  bluetooth.print("bl ");
  bluetooth.print(batteryLevel);
  bluetooth.print("\n");
  Serial.print("bl ");
  Serial.print(batteryLevel);
  Serial.print("\n");
}


