/*
New control system for the load bank uses Shift Registers instead 
of latches in order to reduce complexity and to avoid issues and
 malcfunctions with the latch system.

Most of the code here will be the same as in the previous system, 
which was implemented with the sketch: 
"_4x4_keypad_control_with_Mux_sheild_v2.ino." */


/*
Keypad library: 
contains function definitions for connecting the keypad to the 
Arduino and accepting input from keypresses. */
#include "Keypad_for_load_emulator.h"

/*
Shift Register library:
contains the functions that controls the clocks and data stream
that is sent to the shift registers. */
#include "Shift_Reg_for_load_emulator.h"

/* 
LCD libraries:
First three libraries are needed for communicating with the LCD.
Note that the LCD and LiquidCrystal_I2C libraries are custom 
and they have replaced the original libraries that came installed.
Details on these libraries can be found at:
https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
The last library is one I created to handle the text that shows
on the running display. */
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "LCD_for_load_emulator.h"

/*
Logical Flow library:
This library breaks down the "look_for_input" function in the 
loop() section into smaller functions that improves readability and 
understanding of the flow of all the functions and logic for this
relay control code.*/
#include "main_structure_for_load_emulator.h"


void setup() {
  Serial.begin(9600);
  initialize_keypad_pins();
  initialize_Shift_Reg();
  initializeLCD();
  delay(2500);
  //cautionMessage(), delay(3500);
  LCDprintCurrentState(currentState);
}

void loop() {
  look_for_input();
  
  /*
  Delay must be long enough so that the kepress can be detected
  by the Arduino, according to the current detect method. Next, 
  version of the code should be able to run without this delay. */
  delay(300);
}


