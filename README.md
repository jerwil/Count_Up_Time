# Count Up Box

This is a project I created as a Valentine's day gift to my girlfriend. It counts up the amount of time we have been together as a couple in various units.

![] (https://github.com/jerwil/Count_Up_Time/raw/master/Photos/Front.JPG "Front View")
![] (https://github.com/jerwil/Count_Up_Time/raw/master/Photos/Back.JPG "Back View")

## Features

* Displays amount of time since our first date in various units of time.
* Uses a rotary encoder to determine direction knob is turned.
* Backlight can be toggled using rotary encoder's built in momentary button.

## How to Use

* Press the button to turn on/off the backlight.
* Turn the knob to change the units being displayed or the current time and date.
* When displaying current time or date, hold down the button for 3 seconds to edit. Hold down again to switch between hours and minutes.

## Blog Post

Check out the associated blog post here: [Count up Box - A Valentine's Day Gift](http://www.jeremyadamwilson.com/count-up-box/)

## Demo Video

[Demo Video](https://youtu.be/PbRNsSK7r4M)


## Requirements

### Hardware

* Arduino Nano (or equivalent)
* DS1307 RTC (Real time clock)
* 32.768 kHz crystal oscillator
* 220Ω resistor
* 4 10kΩ resistors (2 for rotary encoder, 2 for RTC)
* 1602 LCD Screen with HD44780 controller
* IIC/I2C Serial Interface Adapter Module (To communicate to screen via I2C)
* Rotary encoder
* Battery holder
* Body/housing (lasercut design included in repository)

![] (https://github.com/jerwil/Count_Up_Time/raw/master/Photos/IMG_2618.JPG "Circuit board picture")
![] (https://github.com/jerwil/Count_Up_Time/raw/master/Photos/IMG_2620.JPG "View inside box")

### Schematics

![] (https://github.com/jerwil/Count_Up_Time/raw/master/Photos/Count%20Up%20Box%20Schematic%20Production_pcb.png "Circuit Board Layout")
![] (https://github.com/jerwil/Count_Up_Time/raw/master/Photos/Rotary%20Encoder%20Mini%20Board_pcb.png "Rotary Encoder Board")

See the included Fritzing diagram (.fzz file) for details.

### License

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Count up box</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="http://www.JeremyAdamWilson.com">Jeremy Wilson</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.


