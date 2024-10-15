#include <Arduino.h>
#include <DabbleESP32.h>
#include <WiFi.h>
#include <RemoteXY.h>


/*
   -- BFBS_V1 --
   
   This part of the source code of the graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 61 bytes
  { 255,4,0,0,0,54,0,19,0,0,0,0,31,2,106,200,200,84,1,1,
  3,0,1,58,64,57,57,147,8,24,24,0,2,31,0,1,55,112,57,57,
  147,47,24,24,0,2,31,0,5,208,17,143,143,14,7,60,60,0,2,26,
  31 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_01; // =1 if button pressed, else =0
  uint8_t button_02; // =1 if button pressed, else =0
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100

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
void motorSpeedcontrol(float padSpeed);

/*Variables inits*/

float pad_xAxis = 0;

float speedL = 0;
float speedR = 0;
float basespeedL = 50;
float basespeedR = 50;

CRemoteXY *remotexy;

void setup(){
  remotexy = new CRemoteXY (
    RemoteXY_CONF_PROGMEM, 
    &RemoteXY, 
    new CRemoteXYConnectionServer (
      new CRemoteXYComm_WiFiPoint (
        "myRemoteXY",       // REMOTEXY_WIFI_SSID
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