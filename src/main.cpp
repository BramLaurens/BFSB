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

void setup() {
  digitalWrite(ch_motorL_REV, LOW);
  digitalWrite(ch_motorR_REV, LOW);
  digitalWrite(ch_motorL_FWD, LOW);
  digitalWrite(ch_motorR_FWD, LOW);

  Serial.begin(115200);

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
  digitalWrite(motorL_FWD, LOW);
  digitalWrite(motorR_REV, LOW);
  ledcWrite(ch_motorL_FWD, 100);
  ledcWrite(ch_motorR_FWD, 100);
  Serial.println("Voor");
  delay(500);

  ledcWrite(ch_motorL_FWD, 0);
  ledcWrite(ch_motorL_REV, 0);
  ledcWrite(ch_motorR_FWD, 0);
  ledcWrite(ch_motorR_REV, 0);
  Serial.println("Stop");
  delay(500);

  digitalWrite(motorL_FWD, LOW);
  digitalWrite(motorR_REV, LOW);
  ledcWrite(ch_motorL_REV, 100);
  ledcWrite(ch_motorR_REV, 100);
  Serial.println("Achter");
  delay(500);

  ledcWrite(ch_motorL_FWD, 0);
  ledcWrite(ch_motorL_REV, 0);
  ledcWrite(ch_motorR_FWD, 0);
  ledcWrite(ch_motorR_REV, 0);
  Serial.println("Stop");
  delay(500);
}

