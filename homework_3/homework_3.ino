#include <Servo.h>
#include <MsTimer2.h>

#define SENSOR_LEFT 0
#define SENSOR_RIGHT 1
#define LOOP_TIME 50
#define SERVO 9
#define LOWER 0
#define UPPER 180
#define MIN_TO_MOVE 2
#define GAIN 0.055

Servo myservo;
int leftSensor,rightSensor,newVal,angle,error,change,leftPrev,rightPrev,leftAve,rightAve;

void setup() {
  leftSensor,rightSensor,newVal,angle,error,change,leftPrev,rightPrev = 0;
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  myservo.attach(SERVO);
  myservo.write(90);//init to center
  MsTimer2::set(LOOP_TIME, readWrite);
  MsTimer2::start();
}

void readWrite() {
  leftSensor = constrain(analogRead(SENSOR_LEFT),1,1023);//constrain to avoid divide by zero error
  rightSensor = constrain(analogRead(SENSOR_RIGHT),1,1023);
  leftAve = ((leftSensor+leftPrev)/2);//simple running average with minimal history
  rightAve = ((rightSensor+rightPrev)/2);
  leftPrev = leftSensor;//save for next iteration
  rightPrev = rightSensor;
  angle = myservo.read();//get current position
  error = leftAve - rightAve;//calculate error
  change = error*GAIN;//apply gain
  newVal = constrain((angle + change),LOWER,UPPER);//constrain the new value so it doesn't try to move past it's limits
  if(abs(change) > MIN_TO_MOVE) {//if the change is below the threhold don't move
    myservo.write(newVal);
  }
}

void loop() {
}
