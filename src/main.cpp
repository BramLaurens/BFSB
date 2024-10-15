/*
Testprogramma voor de microswitch NO
Sluit aan op pin 23 en GND met 10K resistor 
Indrukken = aan
Anders = uit
*/

#include <Arduino.h>
#include <DabbleESP32.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  pinMode(23, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(23) == 0)
  {
    Serial.println("Aan");
  } else
  {
    Serial.println("Uit");
  }
  delay(50);
}