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
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int brightness = 5;    // how bright the LED is, start at half brightness
int fadeAmount = 1;    // how many points to fade the LED by

const int pin_A = 6;  // pin 12
const int pin_B = 7;  // pin 11
const int LED_Pin = 3;
const int ButtonPin = 8;
int MonthDayCounts[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

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
  RTC.begin();

    if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
               
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
        //lcd.clear();
        mode += 1; 
        // B is high so clockwise
        // increase the brightness, dont go over 255          
      }   
      else {
        Serial.println("Counter Clockwise");
        //lcd.clear();
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

    DateTime now = RTC.now();
    DateTime aniv = (1422126000);

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
    String stringOne =  String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC);
    printstring1(stringOne);
    printstring2("The time");
  }
  
  else {printstring1("Error!");}  
  
analogWrite(LED_Pin, brightness*25);
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

