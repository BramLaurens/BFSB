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
void brake();
void motorSpeedcontrol(float padSpeed);

float pad_xAxis = 0;

float speedL = 0;
float speedR = 0;
float basespeedL = 50;
float basespeedR = 50;

void setup(){
  digitalWrite(ch_motorL_REV, LOW);
  digitalWrite(ch_motorR_REV, LOW);
  digitalWrite(ch_motorL_FWD, LOW);
  digitalWrite(ch_motorR_FWD, LOW);

  Serial.begin(115200);
  Dabble.begin("AA_ESP32_Bram");

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
  pad_xAxis = GamePad.getXaxisData();

  Serial.print(GamePad.isCirclePressed());
  Serial.print("  ");
  Serial.println(GamePad.getXaxisData());

  
  if(GamePad.isCirclePressed()){
    motorSpeedcontrol(pad_xAxis);
  }
  else{
    brake();
  }

  delay(100);
  

}

void motorSpeedcontrol(float padSpeed){
  speedL = basespeedL + padSpeed;
  speedR = basespeedR - padSpeed;
  forward();
  /*Serial.print(padSpeed);
  Serial.print("  ");
  Serial.print(speedL);
  Serial.println(speedR);*/
}

void forward(){
  ledcWrite(ch_motorL_FWD, speedL);
  digitalWrite(ch_motorL_REV, LOW);
  ledcWrite(ch_motorR_FWD, speedR);
  digitalWrite(ch_motorR_REV, LOW);
}

void brake(){
  ledcWrite(ch_motorL_FWD, 0);
  digitalWrite(ch_motorL_REV, LOW);
  ledcWrite(ch_motorR_FWD, 0);
  digitalWrite(ch_motorR_REV, LOW);
}