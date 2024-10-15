#include <Arduino.h>
#include <DabbleESP32.h>
#include <ESP32Servo.h>
Servo myservo;

void setup() {
  //ledcAttachPin(4, 0);
  //ledcSetup(0, 50, 8);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo
	myservo.attach(4, 500, 2500); 
}

void loop() {
  myservo.write(0);
  delay(500);
  myservo.write(180);
  delay(500);
}
