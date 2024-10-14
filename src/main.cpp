#include <Arduino.h>
#include <DabbleESP32.h>

// put function declarations here:

#define motorL_FWD 16
#define motorL_REV 0
#define motorR_FWD 2
#define motorR_REV 15

#define motorPWMres 8
#define motorPWMfreq 200

void forward(void);
void brake(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Dabble.begin("ESP32_Bram");

  ledcAttachPin(motorL_FWD, 0);
  ledcSetup(0, motorPWMfreq, motorPWMres);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  Dabble.processInput();
  
  if(GamePad.isCirclePressed()){
    forward();
  }
  */
  ledcWrite(motorL_FWD, 100);
  Serial.println("Test");
}

void forward(void){
  analogWrite(motorL_FWD, 100);
  digitalWrite(motorL_REV, LOW);
  analogWrite(motorR_FWD, 100);
  digitalWrite(motorR_REV, LOW);
}

void brake(void){
  analogWrite(motorL_FWD, 0);
  digitalWrite(motorL_REV, LOW);
  analogWrite(motorR_FWD, 0);
  digitalWrite(motorR_REV, LOW);
}
