#include <Arduino.h>

void Microswitch();

void setup(){
 Serial.begin(115200);
 Serial.println("Testing all components \nPlease wait:");
 pinMode(23, INPUT_PULLUP);
}

void loop(){
  Microswitch();
  delay(7000);
}

void Microswitch(){
  Serial.println("Testing microswitch (normal open):\n");
  delay(500);
  Serial.println("Stap 1: \nSluit pin 1 van de microswitch aan op pin 23 van de ESP\n");
  delay(500);
  Serial.println("Stap 2: \nSluit pin 2 van de microswitch aan op een 10K resistor en de resistor op GND\n");
  delay(500);
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de knop indrukken als je er klaar voor bent:");

  while (digitalRead(23)){}
  Serial.println("Als je de knop nu hebt ingedrukt is de test geslaagd");
  delay(500);
  Serial.println("Als je de knop niet in hebt gedrukt is de test gefaald");
  delay(500);
  Serial.println("Dit was de microswitch test");
  while (digitalRead(23) == true){}  
}