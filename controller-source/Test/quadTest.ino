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
int pitch;
int roll;
int elevation;


void setup() {
  pitch = 0;
  roll = 0;
  elevation = 0;
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    serialData = Serial.read();
    if (serialData == keyA){
      roll -= sensitivity;
    }
    if (serialData == keyD){
      roll += sensitivity;
    }
    if (serialData == keyS){
      pitch -= sensitivity;
    }
    if (serialData == keyW){
      pitch += sensitivity;
    }
    if (serialData == keyC){
      elevation -= sensitivity;
    }
    if (serialData == keySpace){
      elevation += sensitivity;
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
