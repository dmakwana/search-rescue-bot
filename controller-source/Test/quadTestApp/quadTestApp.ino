#include <stdint.h>
#include <Servo.h> 
#include <NewPing.h>
#include <Wire.h>

int limit=1400;
int rollNeutral = 1480;
#define throttleMax 2000
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
#define throttlePin 11
#define yawPin 9
#define flightPin 10
#define echoPin 51
#define trigPin 53
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
#define negTr -1
#define posTr 1
#define nlimTr -5
#define limTr 5
#define k 5 
#define RP_change 15


// ------- ALTITUDE CONTROL SYSTEM
//double Setpoint, Input, Output;
//PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);


//I2C Address
#define SLAVE_ADDRESS 0x04

int yawNeutral = 1500;
int pitchNeutral = 1480;
int serialData = 0;
Servo pitchPWM;
Servo rollPWM;
Servo yawPWM;
Servo throttlePWM;
Servo flightPWM;
int pitch;
int roll;
int throttle;
int throttleByte;
int yaw;
int flight;
bool changed = false;
bool requireReset = true;
bool sendThrottle = true;
char cmd[100];
int cmdIndex;
String st;
int tr;
char c;
int change=1;
long duration;
long distance;
bool hold=false;
bool takeoff = false;
bool land=false;
bool height=false;
long holdVal=100;
long throttleVal=0;
int error=0;
double average;
double sonarVal;
int new_limit;

//I2C Variables
int number=0;
int currentHeight = 0;
int state = 0;

int getVal() {
  
  int val = 0;
       for(int i=2; cmd[i]!=0; i++) { // currentHeight begins at cmd[6]
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

NewPing sonar(trigPin, echoPin, 200);

int getDistance(){
  float alpha = 0.2; // factor to tune
  sonarVal = sonar.ping_cm();
  //Serial.print("Height: ");
  //Serial.println(sonarVal);
  if (abs(sonarVal-average)<20){
    average = alpha * sonarVal + (1-alpha) * average;
    //Serial.println("*******************************************");
  }
  //Serial.print("Average: ");
  //Serial.println(average);
  return average; 
}
  

//void SerialConfig(){
//}

void servo_attach(){
  rollPWM.attach(rollPin);
  pitchPWM.attach(pitchPin);
  throttlePWM.attach(throttlePin);
  yawPWM.attach(yawPin);
  flightPWM.attach(flightPin);
  rollPWM.writeMicroseconds(rollNeutral);
  pitchPWM.writeMicroseconds(pitchNeutral);
  throttlePWM.writeMicroseconds(throttleMin);
  yawPWM.writeMicroseconds(yawNeutral);
  flightPWM.writeMicroseconds(flightMode1);
}

void setup() {
  setNeutral();
  changed = true;
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //throttlePWM.writeMicroseconds(1000);
  
  // define callbacks for i2c communication
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  delay(1000);  // Short delay, wait for the Mate to send back CMD
  Serial.begin(9600);
  bluetooth.begin(115200);
  cmdIndex = 0; 
}

void loop() {
  currentHeight=getDistance();

  //Serial.print("Hold: ");
  //Serial.println(hold);
//  Serial.print("Throttle: ");
//  Serial.println(throttle);
  if (bluetooth.available() > 0)
  {
    c = (char)bluetooth.read();
    if(c=='\n') {
      hold=false;
      land=false;
      takeoff=false;
      height=false;
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
        requireReset = true;
        Serial.println("ARM!");
      }
      else if (cmd[0] == (char)reset){
        setNeutral();
        changed = true;
        requireReset = false;
        Serial.println("RESET!");
      }
      else if (cmd[0] == (char)keyS){
        if (requireReset == false){
          throttle = getVal();
          changed = true;
        }
        //Serial.print("Throttle:");
        //Serial.println(throttle);
      }
      else if (cmd[0] == 'a'){
        if (requireReset == false){
          takeoff=true;
          Serial.println("auto");
        }
      }
      else if (cmd[0] == 'n'){
        servo_attach();
      }
      else if (cmd[0] == 'l'){
        land=true;
        Serial.println("land");
      }
//      else if (cmd[0] == 'h'){
//        hold=true;
//        holdVal=getDistance();
//        throttleVal=throttle;
//      }
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
      else if (cmd[0] == 'f'){
        changed=true;
        Serial.print("hi");
        if (cmd[1] == '+'){
          pitch+=change;
        }
        else if (cmd[1] == '-'){
          pitch-=change;
        }
        st = (String)"fl " + pitch + "\n";
        bluetooth.print(st);
        Serial.print("pitch level: ");
        Serial.println(pitch);  
      }
      else if (cmd[0] == 'c'){
        if (cmd[1] == '+'){
          change++;
        }
        else if (cmd[1] == '-'){
          change--;
        }
        st = (String)"cl " + change + "\n";
        bluetooth.print(st);
        Serial.print("change amount: ");
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
          tr=limTr;
        }
        else if (cmd[1] == '-'){
          tr=nlimTr;
        }
        limit+=tr;
        st = (String)"yt " + limit + "\n";
        bluetooth.print(st);
        Serial.print("yaw trim: ");
        Serial.println(limit);  
      }
      cmdIndex = 0; // reset the cmdIndex  
    } else {      
      cmd[cmdIndex] = c;
      if(cmdIndex<99) cmdIndex++;
    }    
  }
  if (changed == true){
      //Serial.println(throttle);
      rollPWM.writeMicroseconds(roll);
      pitchPWM.writeMicroseconds(pitch);
      throttlePWM.writeMicroseconds(throttle);
      yawPWM.writeMicroseconds(yaw);
      flightPWM.writeMicroseconds(flight);
      changed = false;
  }
  if(takeoff==true){
    if (throttle<limit){
      throttle+=1;
      changed=true;
      delay(1);
      st = (String)"cl " + throttle + "\n";
      bluetooth.print(st);
    }
    else{
      //hold=true;
      takeoff=false;
    }
  }
  if(land==true){
    if (throttle>1100){
      throttle-=1;
      changed=true;
      delay(15);
    }
    else{
      land=false;
    }
  }
  if (hold){
    //Serial.print("Height: ");
    //Serial.println(currentHeight);
    if(height){
      //Serial.println("hold");
      error=holdVal-currentHeight;
      //Serial.println(limit+error*k); 
      if ((new_limit+error*k)<(new_limit+200) && (new_limit+error*k)>(new_limit-200)){
        
        throttle=new_limit+error*k;
        //Serial.println(throttle);
        st = (String)"cl " + throttle + "\n";
        bluetooth.print(st);
      }
      
      changed=true;
      //Hovering over a square
      if (number == 0){
      //everything neutral
        pitch = pitchNeutral;
        roll = rollNeutral;
        //Serial.println("Neutral");
      }
      else if (number == 2) {
      //up
        pitch = pitchNeutral-RP_change;
        //Serial.println("Up");
      }
      else if (number == 1) {
        pitch = pitchNeutral+RP_change;
        //Serial.println("Down");
      }
      else if (number == 4) {
      //left
        roll = rollNeutral-RP_change;
        //Serial.println("Left");
      }
      else if (number == 3) {
      //right
        roll = rollNeutral+RP_change;
        //Serial.println("Right");
      }
    }
    else if (currentHeight>holdVal){
      height=true;
      new_limit=limit;
    }
    //Serial.print("Height");
    //Serial.println(currentHeight);
  }
}

// callback for received data
void receiveData(int byteCount){

    while(Wire.available()) {
        number = Wire.read();
     }
}

// callback for sending data
void sendData(){
  if (sendThrottle){
    throttleByte = throttle/10;
    Wire.write(throttleByte);
    sendThrottle = false;
  }
  else{
    Wire.write(currentHeight);
    sendThrottle = true;
  }
}


