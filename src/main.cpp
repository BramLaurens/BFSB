#include <Arduino.h>
#include <WiFi.h>
#include <RemoteXY.h>

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY GUI configuration  
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
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////




/*Motor declarations*/
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

/*Function prototypes*/

void forward();
void brake();
void reverse();
void motorSpeedcontrolFWD(float padSpeed);
void motorSpeedcontrolREV(float padSpeed);

/*Variables inits*/

float pad_xAxis = 0;
float padxSpeed = 0;
float padFactor = 0.8;

float speedL = 0;
float speedR = 0;
float basespeedL = 150;
float basespeedR = 150;

int motorLoffset = 30;
int motorRoffset = 0;

CRemoteXY *remotexy;

void setup(){
  remotexy = new CRemoteXY (
    RemoteXY_CONF_PROGMEM, 
    &RemoteXY, 
    new CRemoteXYConnectionServer (
      new CRemoteXYComm_WiFiPoint (
        "BFSB_ESP32_Bram",       // REMOTEXY_WIFI_SSID
        "12345678"),        // REMOTEXY_WIFI_PASSWORD
      6377                  // REMOTEXY_SERVER_PORT
    )
  );  

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
  remotexy->handler ();

  pad_xAxis = RemoteXY.joystick_01_x;
  padxSpeed = pad_xAxis * padFactor;

  if(RemoteXY.button_01 == 1){
    motorSpeedcontrolFWD(padxSpeed);
  }
  else{
    if(RemoteXY.button_02 == 1){
      motorSpeedcontrolREV(padxSpeed);
    }
    else{
      brake();
    }
  }
}

void motorSpeedcontrolFWD(float padSpeed){
  speedL = basespeedL + padSpeed;
  speedR = basespeedR - padSpeed;
  forward();
}

void motorSpeedcontrolREV(float padSpeed){
  speedL = basespeedL + padSpeed;
  speedR = basespeedR - padSpeed;
  reverse();
}

void forward(){
  ledcWrite(ch_motorL_FWD, speedL+motorLoffset);
  digitalWrite(ch_motorL_REV, LOW);
  ledcWrite(ch_motorR_FWD, speedR+motorRoffset);
  digitalWrite(ch_motorR_REV, LOW);
}

void brake(){
  ledcWrite(ch_motorL_FWD, 0);
  ledcWrite(ch_motorL_REV, 0);
  ledcWrite(ch_motorR_FWD, 0);
  ledcWrite(ch_motorR_REV, 0);
}

void reverse(){
  digitalWrite(ch_motorL_FWD, LOW);
  digitalWrite(ch_motorR_REV, LOW);
  ledcWrite(ch_motorL_REV, speedL + motorLoffset);
  ledcWrite(ch_motorR_REV, speedR + motorRoffset);
}