#include <Servo.h>
#include <MsTimer2.h>

#define SENSOR_LEFT 0
#define SENSOR_RIGHT 1
#define LOOP_TIME 50
#define POT 3
#define SERVO 9
#define LOWER 0
#define UPPER 180
#define MIN_TO_MOVE 2
#define GAIN 0.055
#define NUM_READINGS 2

Servo myservo;
int leftSensor,rightSensor,newVal,angle,error,change;
int leftIndex,rightIndex,leftTotal,rightTotal,leftAverage,rightAverage;
int leftAve[NUM_READINGS];      // the readings from the analog input
int rightAve[NUM_READINGS];      // the readings from the analog input

void setup() {
  leftSensor,rightSensor,newVal,angle,error,change = 0;
  leftIndex,leftTotal,leftAverage = 0;
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  myservo.attach(SERVO);
  myservo.write(90);
  MsTimer2::set(LOOP_TIME, readWrite);
  MsTimer2::start();
}

void readWrite() {
  leftSensor = analogRead(SENSOR_LEFT);
  rightSensor = analogRead(SENSOR_RIGHT);
  angle = myservo.read();
  error = leftAverage - rightAverage;//calculate error
  change = error*GAIN;
  newVal = constrain((angle + change),LOWER,UPPER);//constrain the new value so it doesn't try to move past it's limits
  if(abs(change) > MIN_TO_MOVE) {//if the servro is at the limit of it's travel or the request is below the threhold don't move
    myservo.write(newVal);
  }
//============= running ave for left side
  leftTotal -= leftAve[leftIndex];// subtract the last reading  
  leftAve[leftIndex] = leftSensor;
  leftTotal += leftAve[leftIndex];// add the reading to the total
  leftIndex += 1;// advance to the next position in the array
  
  if (leftIndex >= NUM_READINGS) {// if we're at the end of the array...
    leftIndex = 0;// ...wrap around to the beginning
  }

  leftAverage = leftTotal / NUM_READINGS;// calculate the average
//============= running ave for right side
  rightTotal -= rightAve[rightIndex];// subtract the last reading  
  rightAve[rightIndex] = rightSensor;
  rightTotal += rightAve[rightIndex];// add the reading to the total
  rightIndex += 1;// advance to the next position in the array
  
  if (rightIndex >= NUM_READINGS) {// if we're at the end of the array...
    rightIndex = 0;// ...wrap around to the beginning
  }

  rightAverage = rightTotal / NUM_READINGS;// calculate the average
//=============
}

void loop() {
}
