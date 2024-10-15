/*
Testcode voor de CNY70 sensor
VCC = 5V
Weerstand = 330 ohm
*/

#include <Arduino.h>
#include <DabbleESP32.h>
#define Drempelwaarde 500

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  pinMode(34, INPUT);
}

void loop() {
  // if (analogRead(34) > Drempelwaarde)
  // {
  //   Serial.println("Licht");
  // } else
  // {
  //   Serial.println("Donker");
  // }
  Serial.println(analogRead(34));
}

