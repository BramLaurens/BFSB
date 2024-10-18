#include <Arduino.h>
#include <Wifi.h>
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Max_Degrees 180
#define Min_Degrees 90
#define Servo_Timeout 1000
Servo myservo;

unsigned long Servo_Timer = 0;

void Servo();

void setup(){
  Serial.begin(115200);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); 
	myservo.attach(4, 500, 2500); 
  myservo.write(Max_Degrees);
}

void loop(){
  Servo();
}

void Servo(){
  if (millis() - Servo_Timer >= 0 && millis() - Servo_Timer <= 1000){
    myservo.write(Min_Degrees);
  } 
  else if (millis() - Servo_Timer > 1000 && millis() - Servo_Timer <= 2000){
    myservo.write(Max_Degrees);
  } 
  else if (millis() - Servo_Timer > 2000 && millis() - Servo_Timer <= 3000){
    Servo_Timer = millis();
  }
}