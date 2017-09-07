#ifndef LCD_load_emulator_h
#define LCD_load_emulator_h

#include "Arduino.h"


#define I2C_ADDR 0x27 // <-- Address goes here, find from I2C Scanner
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

#define INTERVAL_COUNT 30  // Display time = INTERVAL_COUNT * 100 ms 

/* 
Pins on the Arduino that connect to keypad. Used so that any 
keypress will set the interrupt flag to move on to the next
message to be displayed on the LCD. */

// Use for Arduino Mega board
#define INTERRUPT_READ1 30
#define INTERRUPT_READ2 31
#define INTERRUPT_READ3 32
#define INTERRUPT_READ4 33
#define INTERRUPT_WRITE1 34
#define INTERRUPT_WRITE2 35
#define INTERRUPT_WRITE3 36
#define INTERRUPT_WRITE4 37

/*
// Use for Arduino Uno board
#define INTERRUPT_READ1 13
#define INTERRUPT_READ2 12
#define INTERRUPT_READ3 11
#define INTERRUPT_READ4 10
#define INTERRUPT_WRITE1 7
#define INTERRUPT_WRITE2 6
#define INTERRUPT_WRITE3 5
#define INTERRUPT_WRITE4 4
*/

class Liquid_Crystal_Display 
{
public:
  Liquid_Crystal_Display();
  void initialize(void);
  void show_caution_message(void);
  void show_message(String);
  
private:
  void clear_display(void);
  void clear_row(int row);
    
  bool _interrupt_flag;
};

#endif
