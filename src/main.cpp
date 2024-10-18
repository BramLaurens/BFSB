#include <Arduino.h>
#include <Wifi.h>
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

TaskHandle_t Task1;

#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 84 bytes
  { 255,5,0,0,0,77,0,19,0,0,0,0,31,2,106,200,200,84,1,1,
  4,0,1,58,64,57,57,154,7,24,24,2,137,31,70,87,68,0,1,55,
  112,57,57,154,46,24,24,2,2,31,82,69,86,0,5,208,17,143,143,14,
  7,60,60,0,2,26,31,1,37,72,57,57,118,27,24,24,1,37,31,66,
  79,78,75,0 };
  
// this structure defines all the variables and events of the control interface 
struct {

    // input variables
  uint8_t button_01; // =1 if button pressed, else =0
  uint8_t button_02; // =1 if button pressed, else =0
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100
  uint8_t button_03; // =1 if button pressed, else =0

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)


#define Servo_Max_Degrees 90
#define Servo_Min_Degrees 0
#define Servo_Lowtime 1000
#define Servo_Timeout 5000
Servo myservo;

int Servo_Timer = 0;

void Task1code(void * pvParameters);
void Servo();

CRemoteXY *remotexy;

void setup(){
  Serial.begin(115200);

  remotexy = new CRemoteXY (
    RemoteXY_CONF_PROGMEM, 
    &RemoteXY, 
    new CRemoteXYConnectionServer (
      new CRemoteXYComm_WiFiPoint (
        "BFSB_ESP32_Matthias",       // REMOTEXY_WIFI_SSID
        "12345678"),        // REMOTEXY_WIFI_PASSWORD
      6377                  // REMOTEXY_SERVER_PORT
    )
  );

  xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    10000,
    NULL,
    1,
    &Task1,
    0
  );

//Dit moet in de setup voor de servo functie
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); 
	myservo.attach(4, 500, 2500); 
  myservo.write(Min_Degrees);
}

void Task1code(void *pvParameters){
  for(;;){
    // Serial.print("RemoteXY handler running on core ");
    // Serial.println(xPortGetCoreID());
    RemoteXY_delay(1);
    remotexy->handler ();
  } 
}

void loop(){
  Servo();
}


///////////////////////////////////////////////////
//                  Servo Functie                //
///////////////////////////////////////////////////
void Servo(){
  if (RemoteXY.button_03 == 1 && millis() - Servo_Timer > Bonk_Timeout){
    myservo.write(Max_Degrees);
    Serial.println("Pressed");
    Servo_Timer = millis();
  } else if (millis() - Servo_Timer > Bonk_Lowtime){
    myservo.write(Min_Degrees);
    Serial.println("Not pressed");
  }
}