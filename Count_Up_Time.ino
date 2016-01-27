// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

// http://www.geeetech.com/wiki/index.php/Serial_I2C_1602_16%C3%972_Character_LCD_Module

// https://learn.adafruit.com/i2c-spi-lcd-backpack/connect-to-i2c

// https://github.com/marcoschwartz/LiquidCrystal_I2C The library used

// http://arduino.cc/playground/Main/I2cScanner

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS1307 rtc;

const int pin_A = 6;  // Rotary Encoder Pin A
const int pin_B = 7;  // Rotary Encoder Pin B

int clockwise = 0;
int counterclockwise = 0;
int mode = 0;

unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

void setup () {
  Serial.begin(57600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);

}

void loop () {

    clockwise = 0;
    counterclockwise = 0;
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      if(encoder_B) {
        // B is high so clockwise
        // increase the brightness multiplier, dont go over 25
        clockwise = 1;
        mode += 1;            
      }   
      else {
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
         counterclockwise = 1;
       mode -= 1;        
      } 
    }
//    if (mode < 0){mode = 4;}
//    else if (mode > 4){mode = 0;}
  
    //rtc.adjust(DateTime(__DATE__, __TIME__));
    DateTime now = rtc.now();
    DateTime aniv = (1422126000);
    
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(' ');
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();
//    
//    Serial.print(" since midnight 1/1/1970 = ");
//    Serial.print(now.unixtime());
//    Serial.print("s = ");
//    Serial.print(now.unixtime() / 86400L);
//    Serial.println("d");
//    Serial.println("Unixtime of anniversary");
//    Serial.println(aniv.unixtime());
//
//    Serial.print(aniv.year(), DEC);
//    Serial.print('/');
//    Serial.print(aniv.month(), DEC);
//    Serial.print('/');
//    Serial.print(aniv.day(), DEC);
//    Serial.print(' ');
//    Serial.print(aniv.hour(), DEC);
//    Serial.print(':');
//    Serial.print(aniv.minute(), DEC);
//    Serial.print(':');
//    Serial.print(aniv.second(), DEC);
//    Serial.println();

    long SecondsSince = now.unixtime() - aniv.unixtime();
    long MinutesSince = SecondsSince/60;
    long HoursSince = MinutesSince/60;
    long DaysSince = HoursSince/24;
    long MonthsSince = (now.year()-aniv.year())*12+(now.month()-aniv.month());
    //lcd.clear();
//    String stringOne =  String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC);
//    printstring(stringOne,0);
//    printstring("The time",1);

  if (mode == 0){
      printstring(String(SecondsSince),0);
      printstring("Seconds",1);
  }
  else if (mode == 1){
      printstring(String(MinutesSince),0);
      printstring("Minutes",1);
  }
  else if (mode == 2){
      printstring(String(HoursSince),0);
      printstring("Hours",1);
  }
  else if (mode == 3){
      printstring(String(DaysSince),0);
      printstring("Days",1);
  }
  else if (mode == 4){
      printstring(String(MonthsSince),0);
      printstring("Months",1);
  }
  else {printstring("Error!",0);  
  }
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.println(mode);
    //lcd.backlight();
    //delay(1000);
    //lcd.noBacklight();
    
}

void printstring(String stringtoprint, int row) {
  int stringlength = stringtoprint.length();
  char StringChar[stringlength + 1]; 
  stringtoprint.toCharArray(StringChar, stringlength + 1);
  for (int i = 0; i < stringlength; i++){
  lcd.setCursor(i, row);
  lcd.print(StringChar[i]);
}
}
