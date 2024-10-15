#include <Arduino.h>
#include <DabbleESP32.h>

// put function declarations here:

#define motorL_FWD 16
#define motorL_REV 0
#define motorR_FWD 2
#define motorR_REV 15

#define motorPWMres 8
#define motorPWMfreq 200

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Dabble.begin("ESP32_Bram");
}
void loop() {

}

