#include <Arduino.h>
#include <Wifi.h>
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Drempelwaarde_CNY70 2000

int CNY70();

void setup(){
  Serial.begin(115200);
  pinMode(34, INPUT);
}

void loop(){
  CNY70();
}

int CNY70(){
  if (analogRead(34) >= Drempelwaarde_CNY70){
    return(0);
    // Serial.println("Zwart");
  }

  if (analogRead(34) <= Drempelwaarde_CNY70){
    // Serial.println("Wit");
    return(1);
  }
}