#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <FastLED.h>
#define LED_PIN     27
#define NUM_LEDS    1
CRGB leds[NUM_LEDS];

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 27;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int previousSensorValue =0;
int menu = 0;
int light = 0;

unsigned int lastchecked;
unsigned int temps;
unsigned long dif = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// OLED FeatherWing buttons map to different pins depending on board: --> took from SSD1306 documentation and examples 
#if defined(ESP8266)
  #define BUTTON_A  0
  #define BUTTON_B 16
  #define BUTTON_C  2
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
#elif defined(ARDUINO_STM32_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
#elif defined(TEENSYDUINO)
  #define BUTTON_A  4
  #define BUTTON_B  3
  #define BUTTON_C  8
#elif defined(ARDUINO_NRF52832_FEATHER)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
#else // 32u4, M0, M4, nrf52840 and 328p
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5
#endif


  
void setup() 
{
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12,1); //1 = High, 0 = Low
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  lastchecked = millis();
  Serial.println(lastchecked);

  Serial.begin(115200);

  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  Serial.println("Button test");

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // Init des buttons
  // LED builtin est le LED rouge de la board
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_NUM_12, INPUT_PULLUP);
  pinMode(GPIO_NUM_25, INPUT_PULLUP);



  // text display initial --> montrer le menu en startup
    display.clearDisplay();
    display.display();
    display.setTextSize(0.5);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0,0);
        
    //display.println("Menu"); --> Commenté et abrevié pcq il n'y a pas autant de lignes
    display.println("a up|c down|pink ok");
    display.println("-----");
    display.println("1 show temp and hum");
    display.println("2 toggle temp F/C");
    display.println("3 toggle light on/off");
    display.println("4 toggle pot action");
    display.println("5 sleep");
}

void sleepNow()         // here we put the arduino to deep sleep
{

   leds[0] = CRGB(0, 0, 0);
   FastLED.show();
   esp_deep_sleep_start();
}


void lightOnOff() {
    digitalWrite(LED_BUILTIN,  !digitalRead(LED_BUILTIN));
}

void displayMenu() {
  /* Afficher le menu comme demandé dans le projet
   * On a changé les textes pour qu'ils s'affichent dans le screen
  */
    enable_sleep();
    menu = 1;
    display.clearDisplay();
    display.display();
    display.setTextSize(0.5);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0,0);
    //display.println("Menu");
    display.println("a up|c down|pink ok");
    display.println("-----");
    display.println("1 show temp and hum");
    display.println("2 toggle temp F/C");
    display.println("3 toggle light on/off");
    display.println("4 toggle pot action");
    display.println("5 sleep");
    int selection = 0;
    while(menu) {

      
      if(!digitalRead(BUTTON_C)){
        previousMillis = currentMillis;
        delay(250);
        selection++;
        display.clearDisplay();
        display.display();
      }
      if(!digitalRead(BUTTON_A)){
        previousMillis = currentMillis;
        delay(250);
        selection--;
        display.clearDisplay();
        display.display();
      }
      switch(selection)
      {
        case 0:
        selection = 5;
        break;
        
        case 1:
        enable_sleep();
        Serial.println("1");
       
        display.setTextSize(0.5);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        //display.println("Menu");
        display.println("a up|c down|pink ok");
        display.println("-----");
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.println("1 show temp and hum");
        display.setTextColor(SH110X_WHITE, SH110X_BLACK);
        display.println("2 toggle temp F/C");
        display.println("3 toggle light on/off");
        display.println("4 toggle pot action");
        display.println("5 sleep");
        display.display();
        if(!digitalRead(GPIO_NUM_25)) {
          previousMillis = currentMillis;
          menu = 0;
          temperature_display();
          }
        
        break;

        case 2:
        enable_sleep();
        Serial.println("2");
        
        display.setTextSize(0.5);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        //display.println("Menu");
        display.println("a up|c down|pink ok");
        display.println("-----");
        display.println("1 show temp and hum");
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.println("2 toggle temp F/C");
        display.setTextColor(SH110X_WHITE, SH110X_BLACK);
        display.println("3 toggle light on/off");
        display.println("4 toggle pot action");
        display.println("5 sleep");
        display.display();
        if(!digitalRead(GPIO_NUM_25)) {
          previousMillis = currentMillis;
          menu = 0;
          temperature_FC();
          }
        break;

        case 3:
        enable_sleep();
        Serial.println("3");
        display.setTextSize(0.5);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0); 
        //display.println("Menu");
        display.println("a up|c down|pink ok");
        display.println("-----");
        display.println("1 show temp and hum");
        display.println("2 toggle temp F/C");
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.println("3 toggle light on/off");
        display.setTextColor(SH110X_WHITE, SH110X_BLACK);
        display.println("4 toggle pot action");
        display.println("5 sleep");
        display.display();
        if(!digitalRead(GPIO_NUM_25)) {
          previousMillis = currentMillis;
          menu = 0;
          toogle_ligth();
          }
        break;

        case 4:
        enable_sleep();
        Serial.println("4");
        
        display.setTextSize(0.5);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        //display.println("Menu");
        display.println("a up|c down|pink ok");
        display.println("-----");
        display.println("1 show temp and hum");
        display.println("2 toggle temp F/C");
        display.println("3 toggle light on/off");
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.println("4 toggle pot action");
        display.setTextColor(SH110X_WHITE, SH110X_BLACK);
        display.println("5 sleep");
        display.display();
        if(!digitalRead(GPIO_NUM_25)) {
          previousMillis = currentMillis;
          menu = 0;
          toogle_pot();
          }
        break;

        case 5:
        enable_sleep();
        
        Serial.println("5");
        display.setTextSize(0.5);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        //display.println("Menu");
        display.println("a up|c down|pink ok");
        display.println("-----");
        display.println("1 show temp and hum");
        display.println("2 toggle temp F/C");
        display.println("3 toggle light on/off");
        display.println("4 toggle pot action");
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.println("5 sleep");
        display.setTextColor(SH110X_WHITE, SH110X_BLACK);
        display.display();
        if(!digitalRead(GPIO_NUM_25)) {
          previousMillis = currentMillis;
          menu = 0;
          toogle_sleep();
        }
        break;

        default:
        selection = 1;
        break;

        
        
      }        
    }
}

void temperature_display(){
  Serial.println("Option 1 selected");
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(0,0);
  display.println("Press ok to go back");
  display.println("Showing temp and humidity");
  display.display();
  while(1){
    back();
  }       
}

void temperature_FC(){
  Serial.println("Option 2 selected");
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(0,0);
  display.println("Press ok to go back");
  display.println("Toogle temp F/C");
  display.display();
  while(1){
    back();
  }       
}

void toogle_ligth(){
  Serial.println("Option 3 selected");
  delay(100);
  led();
  while(1){
    back();
  }       
}


void toogle_pot(){
  Serial.println("Option 4 selected");
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(0,0);
  display.println("Press ok to go back");
  display.println("Toogle pot");
  display.display();
  while(1){
    back();
  }       
}


void toogle_sleep(){
  Serial.println("Option 5 selected");
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(0,0);
  display.println("Going to sleep");
  display.display();
  leds[0] = CRGB(0, 0, 0);
  FastLED.clear();
  delay(50);
  FastLED.show();
  delay(1000);
  display.print("Z");
  display.display();
  delay(1000);
  display.print("Z");  
  display.display();
  delay(1000);
  display.print("Z");
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
  delay(100);
  light=0;
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
  esp_deep_sleep_start();     
}

void screen(){
 while(1){
  
  enable_sleep();
  //Serial.println(dif);
  if((!digitalRead(BUTTON_A))||(!digitalRead(BUTTON_C))){
    Serial.println("b pressed");
    previousMillis = currentMillis;
    //Serial.println(previousMillis);
    displayMenu();
    }
 
  /*if (digitalRead(BUTTON_A)){ --> Millis used to calculate time between button presses : 5000 milis = 5000 ms = 5s (time required) 
    previousMillis = currentMillis;
  }*/
  if(!digitalRead(BUTTON_B)) {
    previousMillis = currentMillis;
    led();
  }
  
  if(digitalRead(GPIO_NUM_12)) {
    lightOnOff();
    previousMillis = currentMillis;
    //Serial.println(previousMillis);
  }

  
  yield();
  display.display();
  
 }
  
}

void led(){
  if (light){
    light=0;
    leds[0] = CRGB(0, 0, 0);
    FastLED.clear();
    delay(50);
    FastLED.show();
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.println("ligth turned off");
    display.display();
    delay(500);
    display.print("go back to menu");
    display.display();
    delay(100);
    display.print(".");
    display.display();
    delay(100);
    display.print(".");
    display.display();
    delay(100);
    display.print(".");
    display.display();
    delay(100);
    displayMenu();
  }
  else{
    
    light=1;
    delay(100);
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    display.setCursor(1,1);
    display.println("Press ok to go back");
    display.println("Toogle light");
    display.println("Use the potentiometer to change the color");
    display.display();
    while(1){
      //says that light is on
      enable_sleep();
       // read the value from the sensor:
      sensorValue = analogRead(sensorPin);
      if (abs(sensorValue - previousSensorValue) >= 100) previousMillis = currentMillis;
      Serial.println(sensorValue);
      // turn the ledPin on
      digitalWrite(ledPin, HIGH);
      // stop the program for <sensorValue> milliseconds:
      delay(100);
      back();
      // turn the ledPin off:
      digitalWrite(ledPin, LOW);
      // stop the program for for <sensorValue> milliseconds:
      delay(100);
      back();
      if (sensorValue < 1365){
        leds[0] = CRGB(0, 255, 0);
        FastLED.show();
        Serial.println("green");
      }
      else if ((sensorValue >= 1365) && (sensorValue < 2730)){
        leds[0] = CRGB(0, 0, 255);
        FastLED.show();
        Serial.println("blue");
      }
      else{
        leds[0] = CRGB(255, 0, 0);
        FastLED.show();
        Serial.println("red");
      }
      previousSensorValue = sensorValue;
      back();
    }
  }
}

void enable_sleep(){
  currentMillis = millis();
  dif = currentMillis - previousMillis;
  Serial.println(dif);
   if (dif>5000){
      Serial.println("sleep");
      display.clearDisplay();
      display.display();
      delay(100);
      light=0;
      leds[0] = CRGB(0, 0, 0);
      FastLED.clear();
      delay(50);
      FastLED.show();
      esp_deep_sleep_start();
    }
}

void back(){
  if(!digitalRead(GPIO_NUM_25)) {
      previousMillis = currentMillis;
      delay(250);
      displayMenu();
    }
}

void loop() {
  //Serial.print("Milliseconds since ignition: "); // used for debugging
  //Serial.println(millis());
  //delay(200);
  //put code here
  //Serial.println(digitalRead(GPIO_NUM_25));
  screen();
  
  
}
