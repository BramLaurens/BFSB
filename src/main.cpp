#include <Arduino.h>
#include <Wifi.h>
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Servo myservo;

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
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define Drempelwaarde_CNY70 2000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void Microswitch();
void CNY70();
void Ultrasoonsensor();
void Display(int InvoerDisplay);
void Motor();
void Servo();
void Task1code(void * pvParameters);

float duration_us;
float distance_cm;

CRemoteXY *remotexy;

void setup(){
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);
  display.clearDisplay();


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

  ledcAttachPin(motorL_FWD, 0);
  ledcAttachPin(motorL_REV, 1);
  ledcAttachPin(motorR_FWD, 2);
  ledcAttachPin(motorR_REV, 3);

  ledcSetup(0, motorPWMfreq, motorPWMres);
  ledcSetup(1, motorPWMfreq, motorPWMres);
  ledcSetup(2, motorPWMfreq, motorPWMres);
  ledcSetup(3, motorPWMfreq, motorPWMres);

    xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    10000,
    NULL,
    1,
    &Task1,
    0
  );

  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());  
  Serial.println("\n\nTesting all components \nPlease wait:");
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
  Serial.println("Kies het getal van de test die je wilt doen:");
  Serial.println("1 = Microswitch test");
  Serial.println("2 = CNY70 test");
  Serial.println("3 = Ultrasoon test");
  Serial.println("4 = OLED test");
  Serial.println("5 = Motor test");
  Serial.println("6 = Servo test");
  Serial.read();
  while (Serial.available() == 0){}

  switch (Serial.read()){
  case '1':
    Serial.print("Je koos voor: ");
    Serial.print("Microswitch test\n");
    Microswitch();
    break;
  
  case '2':
    Serial.print("Je koos voor: ");
    Serial.print("CNY70 test\n");
    CNY70();
    break;
  
  case '3':
    Serial.print("Je koos voor: ");
    Serial.print("Ultrasoon test\n");
    Ultrasoonsensor();
    break;
  
  case '4':
    Serial.print("Je koos voor: ");
    Serial.print("OLED test\n");
    Display(0);
    break;

case '5':
    Serial.print("Je koos voor: ");
    Serial.print("Motor test\n");
    Motor();
    break;

case '6':
    Serial.print("Je koos voor: ");
    Serial.print("Servo test\n");
    Serial.read();
    Servo();
    break;

  default:
    break;
  }
  Serial.read();
}

void Microswitch(){
  pinMode(23, INPUT_PULLUP);
  Serial.println("\n\nTesting microswitch (normal open):\n");
  Serial.println("Stap 1: \nSluit pin 1 van de microswitch aan op pin 23 van de ESP\n");
  Serial.println("Stap 2: \nSluit pin 2 van de microswitch aan op een 10K resistor en de resistor op GND\n");
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de knop indrukken als je er klaar voor bent:\n");

  while (digitalRead(23)){}
  Serial.println("Als je de knop nu hebt ingedrukt is de test geslaagd");
  Serial.println("Als je de knop niet in hebt gedrukt is de test gefaald");
  Serial.println("Dit was de microswitch test\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();
}

void CNY70(){
  pinMode(34, INPUT);
  Serial.println("\n\nTesting CNY70 sensor:\n");
  Serial.println("Stap 1: \nSluit de Vcc aan op 5V en sluit de GND aan\n");
  Serial.println("Stap 2: \nSluit de input aan op pin 34\n");
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de test beginnen");
  Serial.println("Zet de sensor boven een zwart oppervlak");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();

  if (analogRead(34) >= Drempelwaarde_CNY70)
  {
    Serial.println("Als je sensor nu boven een zwart oppervlak ligt klopt het!\n");
  } else {
    Serial.println("FOUT! De sensor heeft GEEN zwart oppervlak gemeten\n");
  }
  

  Serial.println("Zet de sensor boven een wit oppervlak");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();

  if (analogRead(34) <= Drempelwaarde_CNY70)
  {
    Serial.println("Als je sensor nu boven een wit oppervlak ligt klopt het!");
  } else
  {
    Serial.println("FOUT! De sensor heeft GEEN wit oppervlak gemeten\n");
  }
  Serial.println("Dit was de CNY70 test\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();
}

void  Ultrasoonsensor(){
  pinMode(23, OUTPUT);
  pinMode(22, INPUT);
  Serial.println("\n\nTesting ultrasoon sensor:\n");
  Serial.println("Stap 1: \nSluit de Vcc aan op 3.3V en sluit de GND aan\n");
  Serial.println("Stap 2: \nSluit de Trig_RX aan op pin 23\n");
  Serial.println("Stap 3: \nSluit de Echo_TX aan op pin 22\n");
  Serial.println("Stap 4: \nAls je alles hebt aangesloten mag je de test beginnen\n");
  Serial.println("Straks wordt er 10X een afstand gemeten, jij moet kijken of de afstand klopt");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();

  for (char i = 0; i < 10; i++){
    digitalWrite(23, HIGH);
    delayMicroseconds(1000);
    digitalWrite(23, LOW);

    duration_us = pulseIn(22, HIGH);
    distance_cm = 0.017 * duration_us;

    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    delay(500);
  }
  Serial.println("Zijn de gemeten waarden correct?\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();
}

void Display(int InvoerDisplay){
  Serial.println("\n\nTesting van het OLED scherm:\n");
  Serial.println("Controleer of de OLED juist is aangesloten");
  Serial.println("Als alles klopt zal het beeldscherm het getal 1 tot 10 weergeven");
  Serial.println("Druk enter om te beginnen\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();

  for (int i = 1; i <= 10; i++)
  {
    display.clearDisplay();
    //display.setRotation(1); //Voer een waarde tussen 1 en 3 in of comment deze line; 1 draait het beeld met 90 graden, 2 draait het beeld met 180 graden, 3 draait het beeld 
    display.setTextSize(9);
    //Grootte tekst in pixels (max 6)
    if (i==10){
      display.setCursor(10, 0);// int x, int y; waardes om in te stellen hoe ver opzij of hoe hoog de invoer wordt uitgebeeld. (Hoe hoger de y, en hoe hoger de x, )
    } else {
      display.setCursor(40, 0);// int x, int y; waardes om in te stellen hoe ver opzij of hoe hoog de invoer wordt uitgebeeld. (Hoe hoger de y, en hoe hoger de x, )
    }

    display.setTextColor(WHITE);//De kleur kan je als het goed is niet veranderen
    // Display static text
    display.println(InvoerDisplay+i); //invoer wat wordt uitgebeeld op display
    display.display(); 
    Serial.println(i);
    delay(700);
  }
  
  Serial.println("\nWerkt het scherm correct?\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();
}

void Motor(){
  Serial.println("\n\nTesting van motoren:\n");
  Serial.println("Controleer of de motoren juist zijn aangesloten");
  Serial.println("De + van de linkermotor op pin 4");
  Serial.println("De - van de linkermotor op pin 0");
  Serial.println("De + van de rechtermotor op pin 2");
  Serial.println("De - van de rechtermotor op pin 15\n");
  Serial.println("Als je alles hebt aangesloten mag je de test beginnen\n");
  Serial.println("Als alles klopt gaan bijde motoren eerst vooruit, dan stil, en dan achteruit");
  Serial.println("Druk enter om te beginnen\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();

  digitalWrite(motorL_FWD, LOW);
  digitalWrite(motorR_REV, LOW);
  ledcWrite(ch_motorL_FWD, 100);
  ledcWrite(ch_motorR_FWD, 100);
  Serial.println("Voor\n");
  delay(1000);

  ledcWrite(ch_motorL_FWD, 0);
  ledcWrite(ch_motorL_REV, 0);
  ledcWrite(ch_motorR_FWD, 0);
  ledcWrite(ch_motorR_REV, 0);
  Serial.println("Stop\n");
  delay(1000);

  digitalWrite(motorL_FWD, LOW);
  digitalWrite(motorR_REV, LOW);
  ledcWrite(ch_motorL_REV, 100);
  ledcWrite(ch_motorR_REV, 100);
  Serial.println("Achter\n");
  delay(1000);

  ledcWrite(ch_motorL_FWD, 0);
  ledcWrite(ch_motorL_REV, 0);
  ledcWrite(ch_motorR_FWD, 0);
  ledcWrite(ch_motorR_REV, 0);
  Serial.println("Stop\n");

  Serial.println("Werken de motoren correct?\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 1){}
  Serial.read();
}

void Servo(){
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); 
	myservo.attach(4, 500, 2500); 
  Serial.println("\n\nTesting van de servo:\n");
  Serial.println("Controleer of de servo goed is aangesloten:");
  Serial.println("De bruine draad moet op GND");
  Serial.println("De rode draad moet op de 5V pin");
  Serial.println("De orange draad moet op pin 4");
  Serial.println("Als je alles hebt aangesloten mag je de test beginnen\n");
  Serial.println("Als alles klopt beweegt de servo straks 2x 180 graden heen en terug");
  Serial.println("Jij moet kijken of alles klopt");
  Serial.println("Druk op enter om te beginnen:");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();

  myservo.write(0);
  Serial.println("0 graden");
  delay(1000);
  myservo.write(180);
  Serial.println("180 graden");
  delay(1000);
  myservo.write(0);
  Serial.println("0 graden");
  delay(1000);
  myservo.write(180);
  Serial.println("180 graden\n");

  Serial.println("Werkt de servo correct?\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();
}