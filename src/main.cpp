/*
Voor integreren in main code:

//////////////////////////////////////
Gedefinieerde waardes:
//////////////////////////////////////

#define Microswitch_Pin 23            //Pin voor demicroswitch
#define Microswitch_Timeout 1500      //Tijd voordat de microswitch opnieuw kan worden ingedrukt



//////////////////////////////////////
Intergers:
//////////////////////////////////////

int Score = 0;                        //Globale variabele om de score bij te houden
int Microswitch_Timer = 0;            //Globale variabele voor de timeout van de microswitch



//////////////////////////////////////
Prototypes:
//////////////////////////////////////

void Microswitch();



///////////////////////////////////////
In de void setup():
///////////////////////////////////////

pinMode(23, INPUT_PULLUP);             //Pin voor de microswitch



////////////////////////////////////////
Microswitch functie:
////////////////////////////////////////

void Microswitch(){
  if (digitalRead(Microswitch_Pin) == LOW && (millis() - Microswitch_Timer) > Microswitch_Timeout){
    Score++;
    // Serial.println(Score);
    Microswitch_Timer = millis();
  } 
}
*/

#include <Arduino.h>
#include <Wifi.h>
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Microswitch_Pin 23
#define Microswitch_Timeout 1500

int Score = 0;
int Microswitch_Timer = 0;

void Microswitch();

void setup(){
  Serial.begin(115200);
  pinMode(23, INPUT_PULLUP);
}

void loop(){
  Microswitch();
}

void Microswitch(){
  if (digitalRead(Microswitch_Pin) == LOW && (millis() - Microswitch_Timer) > Microswitch_Timeout){
    Score++;
    // Serial.println(Score);
    Microswitch_Timer = millis();
  } 
}