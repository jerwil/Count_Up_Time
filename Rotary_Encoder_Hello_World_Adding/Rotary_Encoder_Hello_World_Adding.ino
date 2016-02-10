/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS1307 rtc;
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int brightness = 5;    // how bright the LED is, start at half brightness
int fadeAmount = 1;    // how many points to fade the LED by

const int pin_A = 6;  // pin 12
const int pin_B = 7;  // pin 11
const int LED_Pin = 3;
const int ButtonPin = 8;

unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

int clockwise = 0;
int counterclockwise = 0;
int mode = 0;

String LastString1;
String LastString2;

int button_state = 0;
int button_pushed = 0; // This is the indicator that the button was pushed and released
int button_counter = 0; // This is used to detect how long the button is held for
int button_press_initiate[1];     // storage for button press function
int button_press_completed[1];    // storage for button press function

bool LCDon = true;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(57600); 
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();
               
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(ButtonPin, INPUT);

  lcd.init();
  lcd.backlight();    
}

// the loop routine runs over and over again forever:
void loop() {
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      if(encoder_B) {
        Serial.println("Clockwise");
        lcd.clear();
        mode += 1; 
        // B is high so clockwise
        // increase the brightness, dont go over 255          
      }   
      else {
        Serial.println("Counter Clockwise");
        lcd.clear();
        mode -= 1; 
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0           
      }   
}
encoder_A_prev = encoder_A;

button_state = digitalRead(ButtonPin);
button_pushed = button_press (button_state, button_press_initiate, button_press_completed);

if (button_pushed == 1){
  if (LCDon){LCDon = false;}
  else {LCDon = true;}
}

if (LCDon){lcd.backlight();}
else {lcd.noBacklight();}

if (mode < 0){mode = 5;}
else if (mode > 5){mode = 0;}

    DateTime now = rtc.now();
    DateTime aniv = (1422126000);

    long SecondsSince = now.unixtime() - aniv.unixtime();
    long MinutesSince = SecondsSince/60;
    long HoursSince = MinutesSince/60;
    long DaysSince = HoursSince/24;
    long MonthsSince = (now.year()-aniv.year())*12+(now.month()-aniv.month());
  if (mode == 0){
      printstring1(String(SecondsSince));
      printstring2("Seconds");
  }
  else if (mode == 1){
      printstring1(String(MinutesSince));
      printstring2("Minutes");
  }
  else if (mode == 2){
      printstring1(String(HoursSince));
      printstring2("Hours");
  }
  else if (mode == 3){
      printstring1(String(DaysSince));
      printstring2("Days");
  }
  else if (mode == 4){
      printstring1(String(MonthsSince));
      printstring2("Months");
  }
  else if (mode == 5){
    String stringOne =  String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC);
    printstring1(stringOne);
    printstring2("The time");
  }
  
  else {printstring1("Error!");}  
  
analogWrite(LED_Pin, brightness*25);
}


void printstring1(String stringtoprint) {
  if (LastString1 != stringtoprint){
  int stringlength = stringtoprint.length();
  // Add a while loop to add spaces to the end
  char StringChar[stringlength + 1]; 
  stringtoprint.toCharArray(StringChar, stringlength + 1);
  for (int i = 0; i < stringlength; i++){
  lcd.setCursor(i, 0);
  lcd.print(StringChar[i]);
  }
  }
  LastString1 = stringtoprint;
}

void printstring2(String stringtoprint) {
  if (LastString2 != stringtoprint){
  int stringlength = stringtoprint.length();
  char StringChar[stringlength + 1]; 
  stringtoprint.toCharArray(StringChar, stringlength + 1);
  for (int i = 0; i < stringlength; i++){
  lcd.setCursor(i, 1);
  lcd.print(StringChar[i]);
  }
  }
  LastString2 = stringtoprint;
}

int button_press (int button_indicator, int button_press_initiated[1], int button_press_complete[1]){
  if (button_indicator == 0 && button_press_initiated[0] == 1) {
  button_press_complete[0] = 1;
  button_press_initiated[0] = 0;
  }
  else if (button_indicator == 1){
  button_press_initiated[0] = 1;
  button_press_complete[0] = 0;
  }
  else {button_press_complete[0] = 0;}
return button_press_complete[0];
}

