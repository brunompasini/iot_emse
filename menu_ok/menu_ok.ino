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
    while(1) {
      
      if(!digitalRead(BUTTON_C)){
        delay(250);
        selection++;
        display.clearDisplay();
        display.display();
      }
      if(!digitalRead(BUTTON_A)){
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
        break;

        case 2:
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
        break;

        case 3:
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
        break;

        case 4:
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
        break;

        case 5:
        
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
        break;

        default:
        selection = 1;
        break;

        
        
      }        
    }
}

void screen(){
 while(1){
  currentMillis = millis();
  dif = currentMillis - previousMillis;
  Serial.println(dif);
  if((!digitalRead(BUTTON_A))||(!digitalRead(BUTTON_C))){
    Serial.println("b pressed");
    previousMillis = currentMillis;
    Serial.println(previousMillis);
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
    Serial.println(previousMillis);
  }
  /*if (digitalRead(GPIO_NUM_12)){
    previousMillis = currentMillis;
  }*/
  
  yield();
  display.display();
  if (dif>5000){
    Serial.println("sleep");
    display.clearDisplay();
    display.display();
    delay(100);
    break;
  }
 }
  sleepNow(); // deepsleep after while 1, as to prevent that erros break the loop
}

void led(){
  while(1){
    currentMillis = millis();
    dif = currentMillis - previousMillis;
     // read the value from the sensor:
    sensorValue = analogRead(sensorPin);
    if (abs(sensorValue - previousSensorValue) >= 30) previousMillis = currentMillis;
    Serial.println(sensorValue);
    // turn the ledPin on
    digitalWrite(ledPin, HIGH);
    // stop the program for <sensorValue> milliseconds:
    delay(500);
    // turn the ledPin off:
    digitalWrite(ledPin, LOW);
    // stop the program for for <sensorValue> milliseconds:
    delay(500);
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
    if (dif>5000){
      Serial.println("sleep");
      display.clearDisplay();
      display.display();
      delay(100);
      sleepNow();
    }
    if(!digitalRead(BUTTON_C)) {
    previousMillis = currentMillis;
    break;
    }
  }
  screen(); // deepsleep after while 1, as to prevent that erros break the loop
}

void loop() {
  Serial.print("Milliseconds since ignition: "); // used for debugging
  Serial.println(millis());
  delay(200);
  //put code here
  screen();
  
  
}
