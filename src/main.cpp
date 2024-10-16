#include <Arduino.h>

void Microswitch();
void CNY70();
void Ultrasoonsensor();

float duration_us;
float distance_cm;


void setup(){
 Serial.begin(115200);
 Serial.println("Testing all components \nPlease wait:");
 pinMode(23, INPUT_PULLUP);
 pinMode(34, INPUT);
  pinMode(23, OUTPUT);
  pinMode(22, INPUT);
}

void loop(){
  Microswitch();
  delay(1000);
  CNY70();
  delay(1000);
  Ultrasoonsensor();
  delay(1000);
}

void Microswitch(){
  Serial.println("Testing microswitch (normal open):\n");
  Serial.println("Stap 1: \nSluit pin 1 van de microswitch aan op pin 23 van de ESP\n");
  Serial.println("Stap 2: \nSluit pin 2 van de microswitch aan op een 10K resistor en de resistor op GND\n");
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de knop indrukken als je er klaar voor bent:");

  while (digitalRead(23)){}
  Serial.println("Als je de knop nu hebt ingedrukt is de test geslaagd");
  delay(500);
  Serial.println("Als je de knop niet in hebt gedrukt is de test gefaald");
  delay(500);
  Serial.println("Dit was de microswitch test");
  while (digitalRead(23) == true){}  
}

void CNY70(){
  Serial.println("Testing CNY70 sensor:\n");
  Serial.println("Stap 1: \nSluit de Vcc aan op 5V en sluit de GND aan\n");
  Serial.println("Stap 2: \nSluit de input aan op pin 34\n");
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de test beginnen");
  Serial.println("Zet de sensor boven een zwart oppervlak");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();

  if (analogRead(34) >= 2000)
  {
    Serial.println("Als je sensor nu boven een zwart oppervlak ligt klopt het!\n");
  } else
  {
    Serial.println("De sensor heeft GEEN zwart oppervlak gemeten\n");
  }
  
  delay(1000);

  Serial.println("Zet de sensor boven een wit oppervlak");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();

  if (analogRead(34) <= 2000)
  {
    Serial.println("Als je sensor nu boven een wit oppervlak ligt klopt het!");
  } else
  {
    Serial.println("De sensor heeft GEEN wit oppervlak gemeten\n");
  }
  Serial.println("Dit was de microswitch test");
}

void  Ultrasoonsensor(){
  pinMode(23, OUTPUT);
  pinMode(22, INPUT);
  Serial.println("Testing ultrasoon sensor:\n");
  Serial.println("Stap 1: \nSluit de Vcc aan op 5V en sluit de GND aan\n");
  Serial.println("Stap 2: \nSluit de Trig_RX aan op pin 23\n");
  Serial.println("Stap 3: \nSluit de Echo_TX aan op pin 22\n");
  Serial.println("Stap 4: \nAls je alles hebt aangesloten mag je de test beginnen\n");
  Serial.println("Straks wordt er 10X een afstand gemeten, jij moet kijken of de afstand klopt");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();

  for (char i = 0; i < 10; i++){
    digitalWrite(23, HIGH);
    delayMicroseconds(1000);
    digitalWrite(23, LOW);

    duration_us = pulseIn(22, HIGH);
    distance_cm = 0.017 * duration_us;

    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    delay(500);
  }
  Serial.println("Zijn de gemeten waarden correct?\nDan kan je door naar de volgende test");
}