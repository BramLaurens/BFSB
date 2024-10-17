#include <Arduino.h>
#include <RemoteXY.h>
unsigned long huidigetijd = 0;
unsigned long vorigeTijd = 0;
int fakedelay = 0;
int drempelwaarde = 1000;

// put function declarations here:


void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  digitalWrite(13, LOW);
  delayMicroseconds(2);
}


void loop() {
  huidigetijd = millis();
  if (huidigetijd - vorigeTijd  >= 1000) {
    vorigeTijd = huidigetijd;
    fakedelay = fakedelay +1;
    Serial.println(fakedelay);
  switch (fakedelay) {
  case 2:
  digitalWrite(13, HIGH);
  delayMicroseconds(10);
  Serial.println(digitalRead(12));
    break;
  case 10:
    break;
}
}
}