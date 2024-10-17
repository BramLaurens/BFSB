#include <Arduino.h>
#include <RemoteXY.h>
unsigned long huidigetijd = 0;
unsigned long vorigeTijd = 0;
int fakedelay = 0;
int drempelwaarde = 1000;
const int trigPin = 13;
const int echoPin = 12;
unsigned long duration;
unsigned long distance;
int telop = 0;
int hamerTimeout = 1000;
int hammerTime = 0;
int hammerTimeDifference = 0;
void leeshamer();
void tel();




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
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print("  Score: ");
  Serial.println(telop);
  // Serial.println("\t");

  hammerTimeDifference = millis() - hammerTime;

  if((distance <= 5) && (distance >= 1) && hammerTimeDifference > hamerTimeout){
    telop = telop -3;
    hammerTime = millis();
  }

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
