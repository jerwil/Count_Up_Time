// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

// http://www.geeetech.com/wiki/index.php/Serial_I2C_1602_16%C3%972_Character_LCD_Module

// https://learn.adafruit.com/i2c-spi-lcd-backpack/connect-to-i2c

// https://github.com/marcoschwartz/LiquidCrystal_I2C The library used

// http://arduino.cc/playground/Main/I2cScanner


#include <RTClib.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS1307 RTC;
 
// Hardware variables
const int pin_A = 6;  // pin 12
const int pin_B = 7;  // pin 11
const int ButtonPin = 8;

int MonthDayCounts[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


// Variables for the rotary encodier
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
int clockwise = 0;
int counterclockwise = 0;

double mode = 0;

// Variables to keep track of last string printed to ensure screen is only changed when input changes
String LastString1;
String LastString2;


// Button press completion check variables
int button_state = 0;
int button_pushed = 0; // This is the indicator that the button was pushed and released
int button_counter = 0; // This is used to detect how long the button is held for
int button_press_initiate[1];     // storage for button press function
int button_press_completed[1];    // storage for button press function
int timeout = 0; // Time out for not pushing the button for a while
boolean button_hi = false;

// Boolean for the backlight LED status
bool LCDon = true;

// Variables for displaying time
int hour_int = 0;
int minute_int = 0;
int second_int = 0;
int month_int = 0;
int day_int = 0;
int year_int = 0;
int digit_array[8] = {0,0,0,0,0,0,0,0};

// Tick timer veriables
double second_timer[1] = {0}; // This is used to keep track of the timer used to tick for each second
int old_second = 0; //This is used for the tick mechanism
int now_second = 0;
unsigned long currentTime;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(57600); 
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  RTC.begin();

    if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

//RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
               
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
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
        if (mode != 5.5 && mode != 6.5){mode += 1;}
        // B is high so clockwise
        // increase the brightness, dont go over 255          
      }   
      else {
        Serial.println("Counter Clockwise");
        if (mode != 5.5 && mode != 6.5){mode -= 1;} 
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0           
      }   
}
encoder_A_prev = encoder_A;

button_state = digitalRead(ButtonPin);
button_pushed = button_press (button_state, button_press_initiate, button_press_completed);

if (button_state == HIGH){
 button_hi = true;
 timeout = 0; 
}
else {button_hi = false;}

if (button_pushed == 1){
  if (LCDon){LCDon = false;}
  else {LCDon = true;}
}


if (mode < 0){mode = 6;}
else if (mode >= 7){mode = 0;}

    DateTime now = RTC.now();
    DateTime aniv = (1422126000);

month_int = now.month();
day_int = now.day();
year_int = now.year();
hour_int = now.hour();
minute_int = now.minute();
second_int = now.second();


    if (now.year() % 4 == 0){MonthDayCounts[1] = 29;}
    else {MonthDayCounts[1] = 28;}

    long SecondsSince = now.unixtime() - aniv.unixtime();
    long MinutesSince = SecondsSince/60;
    long HoursSince = MinutesSince/60;
    long DaysSince = HoursSince/24;
    long DaysOfMonthSince = MonthDayCounts[now.month()-2]- aniv.day() + now.day();
    long MonthsSince = (now.year()-aniv.year())*12+(now.month()-aniv.month()-1);
    if (now.day() >= aniv.day()){
    MonthsSince += 1;
    DaysOfMonthSince = now.day()- aniv.day();
    }
  
    
    
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
      printstring1(String(MonthsSince) + " Months");
      printstring2(String(DaysOfMonthSince) + " Days");
  }
  else if (mode == 5){
    if (tick(1000, second_timer) == 1){  
      if (button_hi == true){ // This code checks to see if the button has been held down long enough to set alarm
        if (button_counter >= 2) {
        mode = 5.5;
        button_counter = 0;
        button_pushed = 0;
        timeout = 0; 
        }
      button_counter += 1;
      }
      else {
         button_counter = 0; 
      }
    }
 
    time_to_array(hour_int, minute_int, second_int);
    String stringOne =  String(digit_array[0]) + String(digit_array[1]) + ':' + String(digit_array[2]) + String(digit_array[3]) + ':' + String(digit_array[4] + String(digit_array[5]));
    printstring1(stringOne);
    printstring2("The time");
  }
  else if (mode == 5.5){
    time_to_array(hour_int, minute_int, second_int);
    if (tick(1000, second_timer) == 1){  
      if (button_hi == true){ // This code checks to see if the button has been held down long enough to set alarm
        if (button_counter >= 2) {
        mode = 5;
        button_counter = 0;
        button_pushed = 0;
        timeout = 0;
        button_press_initiate[0] = 0; 
        }
      button_counter += 1;
      }
      else {
         button_counter = 0; 
         timeout += 1;
      }
    }
    if (timeout >= 60){
      mode = 5;
      timeout = 0;
    }
 

    String stringOne =  String(digit_array[0]) + String(digit_array[1]) + ':' + String(digit_array[2]) + String(digit_array[3]) + ':' + String(digit_array[4] + String(digit_array[5]));
    printstring1(stringOne);
    printstring2("Time Set");
    LCDon = true;
  }
  else if (mode == 6){
    if (tick(1000, second_timer) == 1){  
      if (button_hi == true){ // This code checks to see if the button has been held down long enough to set alarm
        if (button_counter >= 3) {
        mode = 6.5;
        button_counter = 0;
        button_pushed = 0;
        timeout = 0; 
        }
      button_counter += 1;
      }
      else {
         button_counter = 0; 
      }
    }
    String stringOne =  String(month_int) + '/' + String(day_int) + '/' + String(year_int);
    printstring1(stringOne);
    printstring2("The Date");
  }
  else if (mode == 6.5){
    if (tick(1000, second_timer) == 1){  
      if (button_hi == true){ // This code checks to see if the button has been held down long enough to set alarm
        if (button_counter >= 3) {
        mode = 6;
        button_counter = 0;
        button_pushed = 0;
        timeout = 0;
        button_press_initiate[0] = 0; 
        }
      button_counter += 1;
      }
      else {
         button_counter = 0; 
         timeout += 1;
      }
    }
    if (timeout >= 60){
      mode = 6;
      timeout = 0;
    }
    printstring1("Date Set");
    printstring2("  Mode  ");
    LCDon = true;
  }  
  else {
    printstring1("Error!");
    printstring2("");
  } 

if (LCDon){lcd.backlight();}
else {lcd.noBacklight();}
    
}


void printstring1(String stringtoprint) {
  if (LastString1 != stringtoprint){
  String stringgoingtoprint = stringtoprint;
  // Add a while loop to add spaces to the end
  while(stringgoingtoprint.length() < 16){stringgoingtoprint += " ";}
  int stringlength = stringgoingtoprint.length();
  char StringChar[stringlength + 1]; 
  stringgoingtoprint.toCharArray(StringChar, stringlength + 1);
  for (int i = 0; i < stringlength; i++){
  lcd.setCursor(i, 0);
  lcd.print(StringChar[i]);
  }
  }
  LastString1 = stringtoprint;
}

void printstring2(String stringtoprint) {
  if (LastString2 != stringtoprint){
  String stringgoingtoprint = stringtoprint;
  while(stringgoingtoprint.length() < 16){stringgoingtoprint += " ";}
  int stringlength = stringgoingtoprint.length();
  char StringChar[stringlength + 1]; 
 stringgoingtoprint.toCharArray(StringChar, stringlength + 1);
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

void time_to_array(int hour_int, int minute_int, int second_int){

     if (hour_int < 10 && hour_int > 0){
    digit_array[0] = 0; // 10 will be the designation for not displaying anything
    digit_array[1] = hour_int;
  }
  else {
    digit_array[0] = hour_int/10;
    digit_array[1] = hour_int%10;
  }
  if (minute_int < 10){
  digit_array[2] = 0; // 10 will be the designation for not displaying anything
  digit_array[3] = minute_int;
  }
  else {
    digit_array[2] = minute_int/10;
    digit_array[3] = minute_int%10;
  }
  if (second_int < 10){
  digit_array[4] = 0; // 10 will be the designation for not displaying anything
  digit_array[5] = second_int;
  }
  else {
    digit_array[4] = second_int/10;
    digit_array[5] = second_int%10;
  }
}

int tick(int delay, double timekeeper[1]){
currentTime = millis();
if(currentTime >= (timekeeper[0] + delay)){
  timekeeper[0] = currentTime;
  return 1;
  }
else {return 0;}
}
