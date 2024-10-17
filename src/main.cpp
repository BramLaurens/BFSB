#include <Arduino.h>
#include <RemoteXY.h>
unsigned long huidigetijd = 0;
unsigned long vorigeTijd = 0;
int fakedelay = 0;
int drempelwaarde = 1000;
const int trigPin = 23;
const int echoPin = 22;
unsigned long duration;
unsigned long distance;
int telop = 0;
void leeshamer();

// put function declarations here:


void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(13, LOW);
  delayMicroseconds(2);
}


void loop() {
  leeshamer();
  delay(10);
  Serial.println(distance);
  // Serial.println("\t");
  if(distance <= 5 && distance >= 4 ){
    telop = telop -3;
    Serial.println(telop, DEC);
    delay(1000);
  }
  // else{
  // Serial.println(distance);
  // }
}


void leeshamer(){
  huidigetijd = millis();
  if (huidigetijd - vorigeTijd  >= 1) {
    vorigeTijd = huidigetijd;
    fakedelay = fakedelay +1;

  switch (fakedelay) {
  case 1:
  digitalWrite(trigPin, LOW);
  break;
  case 3:
  digitalWrite(trigPin, HIGH);
  break;
  case 14:
  digitalWrite(trigPin, LOW);
  duration = pulseInLong(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  fakedelay = 0;
  break;
  }
  }

}