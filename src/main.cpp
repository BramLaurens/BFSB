#include <Arduino.h>
#include <WiFi.h>
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

TaskHandle_t Task1;

////////////////////////////Display LIB/////////////////////////////////
void Display(int InvoerDisplay);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define BLUE 0x03

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//////////////////////////////////////////////////////////////////////

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


/*Servo declarations*/
#define Servo_Pin 32
#define Servo_Max_Degrees 90
#define Servo_Min_Degrees 0
#define Servo_Lowtime 1000
#define Servo_Timeout 5000
Servo myservo;


/*Microssitch declarations*/
#define Microswitch_Pin 23
#define Microswitch_Timeout 1500


/*Ultrasoon declarations*/
#define Strafpunt 3
#define Ultrasoon_Trig_Pin 26
#define Ultrasoon_Echo_Pin 27
#define Ultrasoon_Timeout 4000
#define Ultrasoon_Measure_Delay 100
#define Strafpunt_Drempelwaarde_cm 7


/*CNY70 declarations*/
#define Drempelwaarde_CNY70 2000
#define CNY70_Pin 34


/*Display declarations*/
//Vcc = 5V / 3.3V
//SDA = pin 21
//SCL = pin 22


/*Function prototypes*/
void forward();
void brake();
void reverse();
void servo();
void Display(int InvoerDisplay);
void motorSpeedcontrolFWD(float padSpeed);
void motorSpeedcontrolREV(float padSpeed);
void Task1code(void * pvParameters);
void motorSpeedlimiter();
void remoteMotorcontrol();
void microswitch();
bool CNY70();
void ultrasoon();

/*Motor Variables*/
float pad_xAxis = 0;
float padxSpeed = 0;
float padFactor = 0.8;

float speedL = 0;
float speedR = 0;
float basespeedL = 150;
float basespeedR = 150;
int maxSpeedL = 255;
int maxSpeedR = 255;

int motorLoffset = 0;
int motorRoffset = 0;


/*Servo Variables*/
unsigned long Servo_Timer = 0;


/*Microswitch Variables*/
unsigned long Microswitch_Timer = 0;


/*Ultrasoon Variables*/
unsigned long Ultrasoon_Timer = 0;
int duration_us = 0;
int distance_cm = 0;
bool Ultrasoon_Timer_Toggle = true;


int Score = 0;

CRemoteXY *remotexy;

void setup(){
  Serial.begin(115200);

  //Wifi
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


  //Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setRotation(2); //Voer een waarde tussen 1 en 3 in of comment deze line; 1 draait het beeld met 90 graden, 2 draait het beeld met 180 graden, 3 draait het beeld met 270 graden
  display.setTextSize(9);
  display.setTextColor(WHITE);//De kleur kan je als het goed is niet veranderen


  //Motor
  digitalWrite(ch_motorL_REV, LOW);
  digitalWrite(ch_motorR_REV, LOW);
  digitalWrite(ch_motorL_FWD, LOW);
  digitalWrite(ch_motorR_FWD, LOW);

  ledcAttachPin(motorL_FWD, 0);
  ledcAttachPin(motorL_REV, 1);
  ledcAttachPin(motorR_FWD, 2);
  ledcAttachPin(motorR_REV, 3);

  ledcSetup(0, motorPWMfreq, motorPWMres);
  ledcSetup(1, motorPWMfreq, motorPWMres);
  ledcSetup(2, motorPWMfreq, motorPWMres);
  ledcSetup(3, motorPWMfreq, motorPWMres);

  
  //Servo
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); 
	myservo.attach(Servo_Pin, 500, 2500); 
  myservo.write(Servo_Min_Degrees);

  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());  


  //Microswitch
  pinMode(Microswitch_Pin, INPUT_PULLUP);  


  //CNY70
  pinMode(CNY70_Pin, INPUT);


  //Ultrasoon
  pinMode(Ultrasoon_Trig_Pin, OUTPUT);
  pinMode(Ultrasoon_Echo_Pin, INPUT);
}

void Task1code(void *pvParameters){
  for(;;){
    //Serial.print("RemoteXY handler running on core ");
    //Serial.println(xPortGetCoreID());
    RemoteXY_delay(1);          
    remotexy->handler ();
  } 
}

void loop() {
  ultrasoon();
  remoteMotorcontrol();
  servo();
  microswitch();
  Display(Score);

  // if (CNY70 == true){
  //   ledcWrite(ch_motorL_FWD, 100);
  //   digitalWrite(motorL_REV, LOW);
  //   ledcWrite(ch_motorR_REV, 100)
  //   digitalWrite(motorR_FWD, LOW);
  //   delay(100);
  //   ledcWrite(ch_motorL_FWD, 100);
  //   digitalWrite(motorL_REV, LOW);
  //   ledcWrite(ch_motorR_FWD, 100);
  //   digitalWrite(motorR_REV, LOW);
  //   delay(500);
  // }
}

void remoteMotorcontrol(){
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
  speedL = basespeedL + motorLoffset + padSpeed;
  speedR = basespeedR + motorRoffset - padSpeed;
  forward();
}

void motorSpeedcontrolREV(float padSpeed){
  speedL = basespeedL + motorLoffset + padSpeed;
  speedR = basespeedR + motorRoffset - padSpeed;
  reverse();
}

void motorSpeedlimiter(){
  if(speedL > maxSpeedL){
    speedL = maxSpeedL;
  }
  if(speedR > maxSpeedR){
    speedR = maxSpeedR;
  }
  if(speedL < 0){
    speedL = 0;
  }
  if(speedR < 0){
    speedR = 0;
  }
}

void forward(){
  motorSpeedlimiter();
  
  Serial.print(speedL);
  Serial.print("  ");
  Serial.println(speedR);
  ledcWrite(ch_motorL_FWD, speedL);
  digitalWrite(motorL_REV, LOW);
  ledcWrite(ch_motorR_FWD, speedR);
  digitalWrite(motorR_REV, LOW);
}

void brake(){
  ledcWrite(ch_motorL_FWD, 0);
  ledcWrite(ch_motorL_REV, 0);
  ledcWrite(ch_motorR_FWD, 0);
  ledcWrite(ch_motorR_REV, 0);
}

void reverse(){
  motorSpeedlimiter();
  digitalWrite(motorL_FWD, LOW);
  digitalWrite(motorR_FWD, LOW);
  ledcWrite(ch_motorL_REV, speedL);
  ledcWrite(ch_motorR_REV, speedR);
}

void Display(int InvoerDisplay) {
  display.clearDisplay();
  if (InvoerDisplay >= 10 || InvoerDisplay < 0){
    display.setCursor(10, 0);
  } else {
    display.setCursor(40, 0);
  }
  Serial.println(Score);
  display.println(InvoerDisplay); //invoer wat wordt uitgebeeld op display
  display.display(); 
}

void servo(){
  if (RemoteXY.button_03 == 1 && millis() - Servo_Timer > Servo_Timeout){
    myservo.write(Servo_Max_Degrees);
    // Serial.println("Pressed");
    Servo_Timer = millis();
  } else if (millis() - Servo_Timer > Servo_Lowtime){
    myservo.write(Servo_Min_Degrees);
    // Serial.println("Not pressed");
  }
}

void microswitch(){
  if (digitalRead(Microswitch_Pin) == LOW && (millis() - Microswitch_Timer) > Microswitch_Timeout){
    Score++;
    // Serial.println(Score);
    Microswitch_Timer = millis();
  } 
}

bool CNY70(){
  if (analogRead(CNY70_Pin) >= Drempelwaarde_CNY70){
    return(false);
    // Serial.println("Zwart");
  }

  if (analogRead(CNY70_Pin) <= Drempelwaarde_CNY70){
    Serial.println("Wit");
    return(true);
  }
}

void ultrasoon(){
  if (millis() - Ultrasoon_Timer > Ultrasoon_Measure_Delay && Ultrasoon_Timer_Toggle == true){
  digitalWrite(Ultrasoon_Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasoon_Trig_Pin, LOW);

  duration_us = pulseIn(Ultrasoon_Echo_Pin, HIGH);
  distance_cm = 0.017 * duration_us;

  // Serial.print("distance: ");
  // Serial.print(distance_cm);
  // Serial.println(" cm");
  
  Ultrasoon_Timer = millis();
  }

  if (distance_cm < Strafpunt_Drempelwaarde_cm && distance_cm != 0){
    Score = Score - Strafpunt;
    // Serial.println(Score);
    distance_cm = Strafpunt_Drempelwaarde_cm;
    Ultrasoon_Timer_Toggle = false;
  }

  if (Ultrasoon_Timer_Toggle == false && millis() - Ultrasoon_Timer > Ultrasoon_Timeout){
    Ultrasoon_Timer_Toggle = true;
  }
  
}