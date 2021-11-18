#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <FastLED.h>
#include "DHT.h"

#define LED_PIN     27
#define NUM_LEDS    1
#define DHTPIN      4
#define DHTTYPE     DHT22

DHT dht(DHTPIN, DHTTYPE);
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

int isCelsius = 1; // Afficher temperature en Celsius par default

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

  dht.begin();
  
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
    display.println("1. show temp and hum");
    display.println("2. toggle temp F/C");
    display.println("3. toggle light on/off");
    display.println("4. toggle pot action: dim/set");
    display.println("5. sleep");
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
    display.setTextColor(BLACK, WHITE);
    display.println("1. show temp and hum");
    display.println("2. toggle temp F/C");
    display.println("3. toggle light on/off");
    display.println("4. toggle pot action: dim/set");
    display.println("5. sleep");
    
    int cnt = 0;


    Serial.println("before while");
    while(1){
      if(digitalRead(BUTTON_B)) {
        cnt++;
        Serial.print(cnt);
      }
      if (cnt>5){
        cnt = 1;
      }

      switch(cnt){
          case 1 :
              display.clearDisplay();
              display.display();
              display.setTextSize(0.5);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(0,0);
              //display.println("Menu");
              display.println("-> 1. show temp and hum");
              display.println("2. toggle temp F/C");
              display.println("3. toggle light on/off");
              display.println("4. toggle pot action: dim/set");
              display.println("5. sleep");
              break;

          case 2 :
              display.clearDisplay();
              display.display();
              display.setTextSize(0.5);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(0,0);
              //display.println("Menu");
              display.println("1. show temp and hum");
              display.println("-> 2. toggle temp F/C");
              display.println("3. toggle light on/off");
              display.println("4. toggle pot action: dim/set");
              display.println("5. sleep");
              break;


          case 3 :
              display.clearDisplay();
              display.display();
              display.setTextSize(0.5);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(0,0);
              //display.println("Menu");
              display.println("1. show temp and hum");
              display.println("2. toggle temp F/C");
              display.println("-> 3. toggle light on/off");
              display.println("4. toggle pot action: dim/set");
              display.println("5. sleep");
              break;

          case 4 :

              display.clearDisplay();
              display.display();
              display.setTextSize(0.5);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(0,0);
              //display.println("Menu");
              display.println("1. show temp and hum");
              display.println("2. toggle temp F/C");
              display.println("3. toggle light on/off");
              display.println("-> 4. toggle pot action: dim/set");
              display.println("5. sleep");

              break;

          case 5 :
              display.clearDisplay();
              display.display();
              display.setTextSize(0.5);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(0,0);
              //display.println("Menu");
              display.println("1. show temp and hum");
              display.println("2. toggle temp F/C");
              display.println("3. toggle light on/off");
              display.println("4. toggle pot action: dim/set");
              display.println("-> 5. sleep");
              break;
              
          default : 
              Serial.println("Not enteinrg");
              display.clearDisplay();
              display.display();
              display.setTextSize(0.5);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(0,0);
              //display.println("Menu");
              display.println("1. show temp and hum");
              display.println("2. toggle temp F/C");
              display.println("3. toggle light on/off");
              display.println("4. toggle pot action: dim/set");
              display.println("5. sleep");
      }
      
    }
    

    
    
}

void changeTemperature(){
    // when action pour changer

    if(isCelsius)
      isCelsius = 0;
    else
      isCelsius = 1;
}


void displayTemperature(){
  
    float h = dht.readHumidity();
    float t;

    if (isCelsius)
      t = dht.readTemperature();
    else
      t = dht.readTemperature(true);

    display.print("Temperature: ");
    display.println(t);
    display.print("Humidity: ");
    display.println(h);

    
}

void screen(){
 while(1){
  currentMillis = millis();
  dif = currentMillis - previousMillis;
  Serial.println(dif);
  if(!digitalRead(BUTTON_A)) {
    displayMenu();
    Serial.println("test");
    previousMillis = currentMillis;
    Serial.println(previousMillis);
  }
  /*if (digitalRead(BUTTON_A)){ --> Millis used to calculate time between button presses : 5000 milis = 5000 ms = 5s (time required) 
    previousMillis = currentMillis;
  }*/
  if(!digitalRead(BUTTON_C)) {
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
    if (abs(sensorValue - previousSensorValue) >= 30) previousMillis = currentMillis; //verifie si le potentiometre a été touché (marge d'erreur)
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
