#include <Arduino.h>
#include <DabbleESP32.h>

// put function declarations here:

#define motorL_FWD 4
#define motorL_REV 0
#define motorR_FWD 2
#define motorR_REV 15

#define ch_motorL_FWD 0
#define ch_motorL_REV 1
#define ch_motorR_FWD 2
#define ch_motorR_REV 3

#define motorPWMres 8
#define motorPWMfreq 200

void forward();

void setup(){
  Serial.begin(115200);
  Dabble.begin("ESP32_Bram");

  ledcAttachPin(motorL_FWD, 0);
  ledcAttachPin(motorL_REV, 1);
  ledcAttachPin(motorR_FWD, 2);
  ledcAttachPin(motorR_REV, 3);

  ledcSetup(0, motorPWMfreq, motorPWMres);
  ledcSetup(1, motorPWMfreq, motorPWMres);
  ledcSetup(2, motorPWMfreq, motorPWMres);
  ledcSetup(3, motorPWMfreq, motorPWMres);
}

void loop() {
  Dabble.processInput();
  if(GamePad.isCirclePressed()){
  }
  
  forward();
}

void forward(){
  ledcWrite(ch_motorL_FWD, 50);
  digitalWrite(ch_motorL_REV, LOW);
  ledcWrite(ch_motorR_FWD, 50);
  digitalWrite(ch_motorR_REV, LOW);
}

void brake(){
  ledcWrite(ch_motorL_FWD, 0);
  digitalWrite(ch_motorL_REV, LOW);
  ledcWrite(ch_motorR_FWD, 0);
  digitalWrite(ch_motorR_REV, LOW);
}