#include <Servo.h>
#include <MsTimer2.h>

#define SENSOR_LEFT 0
#define SENSOR_RIGHT 1
//#define SENSOR_CENTER 2
#define POT 3
#define SERVO 9
#define HUNDO 10
#define TEN 50
#define SEC 1000
#define LOWER 0
#define UPPER 180
#define MIN_TO_MOVE 2
//#define GAIN 0.1
#define DEBUG true
//#define DEBUG false

Servo myservo;
unsigned long hundredhzTimer,tenhzTimer,onehzTimer;
int leftSensor,rightSensor,newVal,angle,error,change;
float gain;
bool maxed;

void setup() {
  hundredhzTimer,tenhzTimer,onehzTimer = 0;
  leftSensor,rightSensor,newVal,angle,error,change = 0;
  gain = 0.0;
  maxed = false;
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  myservo.attach(SERVO);
  myservo.write(90);
  Serial.begin(115200);
  Serial.println(F("Begin"));
}

void loop() {
  if(onehzTimer < millis() && DEBUG) {//this whole loop is debug code
    Serial.println(F("================================="));
    Serial.print(F("Servo angle"));Serial.println(myservo.read());
    Serial.print(F("Left Sensor: "));Serial.println(leftSensor);
    Serial.print(F("Right Sensor: "));Serial.println(rightSensor);
    Serial.print(F("Error: "));Serial.println(error);
    Serial.print(F("New val: "));Serial.println(newVal);
    Serial.print(F("Trim pot: "));Serial.println(analogRead(POT));
    Serial.print(F("Gain: "));Serial.println(gain);
    Serial.print(F("Change: "));Serial.println(change);
    if(maxed){
      Serial.println(F("MAXED, no move"));
    }else {
      Serial.println(F("MOVING"));
    }    
    onehzTimer += SEC;
  }  

  if(tenhzTimer < millis()) {//sample sensors at 10hz
    leftSensor = analogRead(SENSOR_LEFT);
    rightSensor = analogRead(SENSOR_RIGHT);
    gain = (map(analogRead(POT),0,1023,0,100)*.001);
    angle = myservo.read();
    error = rightSensor - leftSensor;//calculate error
    change = error*gain;
    newVal = constrain((angle + change),LOWER,UPPER);//constrain the new value so it doesn't try to move past it's limits
    if(abs(change) > MIN_TO_MOVE) {// && angle < UPPER && angle > LOWER){//if the servro is at the limit of it's travel or the request is below the threhold don't move
      myservo.write(newVal);
      maxed = false;
    } else {
      maxed = true;
    }
    tenhzTimer += TEN;
  }

  if(hundredhzTimer < millis()) {

    hundredhzTimer += HUNDO;
  }

}
