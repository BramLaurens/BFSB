#include <Arduino.h>
#include <RemoteXY.h>
#include <Wifi.h>
#include <ESP32Servo.h>
Servo myservo;

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

void Microswitch();
void CNY70();
void Ultrasoonsensor();
void OLED();
void Motor();
void Servo();

float duration_us;
float distance_cm;


void setup(){
  Serial.begin(115200);
  Serial.println("Testing all components \nPlease wait:");

  ledcAttachPin(motorL_FWD, 0);
  ledcAttachPin(motorL_REV, 1);
  ledcAttachPin(motorR_FWD, 2);
  ledcAttachPin(motorR_REV, 3);

  ledcSetup(0, motorPWMfreq, motorPWMres);
  ledcSetup(1, motorPWMfreq, motorPWMres);
  ledcSetup(2, motorPWMfreq, motorPWMres);
  ledcSetup(3, motorPWMfreq, motorPWMres);
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
    OLED();
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
  Serial.println("Testing microswitch (normal open):\n");
  Serial.println("Stap 1: \nSluit pin 1 van de microswitch aan op pin 23 van de ESP\n");
  Serial.println("Stap 2: \nSluit pin 2 van de microswitch aan op een 10K resistor en de resistor op GND\n");
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de knop indrukken als je er klaar voor bent:");

  while (digitalRead(23)){}
  Serial.println("Als je de knop nu hebt ingedrukt is de test geslaagd");
  Serial.println("Als je de knop niet in hebt gedrukt is de test gefaald");
  Serial.println("Dit was de microswitch test\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();
}

void CNY70(){
  pinMode(34, INPUT);
  Serial.println("Testing CNY70 sensor:\n");
  Serial.println("Stap 1: \nSluit de Vcc aan op 5V en sluit de GND aan\n");
  Serial.println("Stap 2: \nSluit de input aan op pin 34\n");
  Serial.println("Stap 3: \nAls je alles hebt aangesloten mag je de test beginnen");
  Serial.println("Zet de sensor boven een zwart oppervlak");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();

  if (analogRead(34) >= 2000)
  {
    Serial.println("Als je sensor nu boven een zwart oppervlak ligt klopt het!\n");
  } else
  {
    Serial.println("FOUT! De sensor heeft GEEN zwart oppervlak gemeten\n");
  }
  

  Serial.println("Zet de sensor boven een wit oppervlak");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();

  if (analogRead(34) <= 2000)
  {
    Serial.println("Als je sensor nu boven een wit oppervlak ligt klopt het!");
  } else
  {
    Serial.println("FOUT! De sensor heeft GEEN wit oppervlak gemeten\n");
  }
  Serial.println("Dit was de CNY70 test\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();
}

void  Ultrasoonsensor(){
  pinMode(23, OUTPUT);
  pinMode(22, INPUT);
  Serial.println("Testing ultrasoon sensor:\n");
  Serial.println("Stap 1: \nSluit de Vcc aan op 5V en sluit de GND aan\n");
  Serial.println("Stap 2: \nSluit de Trig_RX aan op pin 23\n");
  Serial.println("Stap 3: \nSluit de Echo_TX aan op pin 22\n");
  Serial.println("Stap 4: \nAls je alles hebt aangesloten mag je de test beginnen\n");
  Serial.println("Straks wordt er 10X een afstand gemeten, jij moet kijken of de afstand klopt");
  Serial.println("Druk enter om te meten");
  Serial.read();
  while (Serial.read() == 0){}
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
  while (Serial.available() == 0){}
  Serial.read();
}

void OLED(){

}

void Motor(){
  Serial.println("Testing van motoren:\n");
  Serial.println("Controleer of de motoren juist zijn aangesloten");
  Serial.println("De + van de linkermotor op pin 4");
  Serial.println("De - van de linkermotor op pin 0");
  Serial.println("De + van de rechtermotor op pin 2");
  Serial.println("De - van de rechtermotor op pin 15\n");
  Serial.println("Als je alles hebt aangesloten mag je de test beginnen\n");
  Serial.println("Als alles klopt gaan bijde motoren eerst vooruit, dan stil, en dan achteruit");
  Serial.println("Druk enter om te beginnen\n");
  Serial.read();
  while (Serial.available() == 0){}
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

  Serial.println("Werken de motoren correct?\nDruk dan op enter om naar de volgende test te gaan:\n");
  Serial.read();
  while (Serial.available() == 0){}
  Serial.read();
}

void Servo(){
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); 
	myservo.attach(4, 500, 2500); 
  Serial.println("Testing van de servo:\n");
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
  delay(500);
  myservo.write(180);
  delay(500);
  myservo.write(0);
  delay(500);
  myservo.write(180);
  delay(500);
}