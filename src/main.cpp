/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Arduino.h>
#include <DabbleESP32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define BLUE 0x03

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  
}

void loop() {
  display.setRotation(2); //Voer een waarde tussen 1 en 3 in of comment deze line; 1 draait het beeld met 90 graden, 2 draait het beeld met 180 graden, 3 draait het beeld 
  display.setTextSize(5);
  //Grootte tekst in pixels (max 6)
  display.setTextColor(WHITE);
  //
  display.setCursor(50, 30); // int x, int y; waardes om in te stellen hoe ver opzij of hoe hoog de invoer wordt uitgebeeld. (Hoe hoger de y, en hoe hoger de x, )
  // Display static text
  display.println("T"); //invoer wat wordt uitgebeeld op display
  display.display(); 
}



//int Display(InvoerDisplay) {

  //return a + b; // Retourneert de som van a en b}
  /*   display.setRotation(2); //Voer een waarde tussen 1 en 3 in of comment deze line; 1 draait het beeld met 90 graden, 2 draait het beeld met 180 graden, 3 draait het beeld 
  display.setTextSize(5);
  //Grootte tekst in pixels (max 6)
  display.setTextColor(WHITE);
  //
  display.setCursor(50, 30); // int x, int y; waardes om in te stellen hoe ver opzij of hoe hoog de invoer wordt uitgebeeld. (Hoe hoger de y, en hoe hoger de x, )
  // Display static text
  display.println("T"); //invoer wat wordt uitgebeeld op display
  display.display(); 
  */