#include "LCD_for_load_emulator.h"
#include "Arduino.h"


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C my_lcd(I2C_ADDR, En_pin, 
                         Rw_pin, Rs_pin, 
                         D4_pin, D5_pin, D6_pin, D7_pin)
;

Liquid_Crystal_Display::
Liquid_Crystal_Display(){}

void Liquid_Crystal_Display::
clear_display(void) {
  Serial.println(F("LCD: Clearing display."));
  Serial.println();
  
  clear_row(0);
  clear_row(1);
  clear_row(2);
  clear_row(3);
}

void Liquid_Crystal_Display::
clear_row(int row) {
  my_lcd.setCursor(0, row);
  my_lcd.print("                    ");
}

void Liquid_Crystal_Display:: 
showMessage(String full_message) {
  _interrupt_flag = false;
  
  // Setup for the (keypad) interrupt pins.
  pinMode(INTERRUPT_READ1, INPUT_PULLUP);
  pinMode(INTERRUPT_READ2, INPUT_PULLUP);
  pinMode(INTERRUPT_READ3, INPUT_PULLUP);
  pinMode(INTERRUPT_READ4, INPUT_PULLUP);
  pinMode(INTERRUPT_WRITE1, OUTPUT);
  pinMode(INTERRUPT_WRITE2, OUTPUT);
  pinMode(INTERRUPT_WRITE3, OUTPUT);
  pinMode(INTERRUPT_WRITE4, OUTPUT);
  digitalWrite(INTERRUPT_WRITE1, LOW);
  digitalWrite(INTERRUPT_WRITE2, LOW);
  digitalWrite(INTERRUPT_WRITE3, LOW);
  digitalWrite(INTERRUPT_WRITE4, LOW);
  
  Serial.println(F("LCD: Showing message"));
  Serial.println(F("********************"));
  Serial.println(full_message.substring( 0,20)); 
  Serial.println(full_message.substring(20,40));
  Serial.println(full_message.substring(40,60)); 
  Serial.println(full_message.substring(60));
  Serial.println(F("********************"));
  Serial.println();
  
  my_lcd.setCursor (0, 0);
  my_lcd.print(full_message.substring( 0,20));
  my_lcd.setCursor (0, 1);
  my_lcd.print(full_message.substring(20,40));
  my_lcd.setCursor (0, 2);
  my_lcd.print(full_message.substring(40,60));
  my_lcd.setCursor (0, 3);
  my_lcd.print(full_message.substring(60));
  
  int i = 0;
  while(_interrupt_flag == false) {
    if (i < INTERVAL_COUNT && 
       (digitalRead(INTERRUPT_READ1) && digitalRead(INTERRUPT_READ2) &&
        digitalRead(INTERRUPT_READ3) && digitalRead(INTERRUPT_READ4))) {
    }
    else {
      _interrupt_flag = true;
      //Serial.println("LCD: Displayed message interrupted.");
    }
    delay(100);
    i++;
  }
}

void Liquid_Crystal_Display::
showMessage(String full_message, String speed) {
  _interrupt_flag = false;
  
  // Setup for the (keypad) interrupt pins.
  pinMode(INTERRUPT_READ1, INPUT_PULLUP);
  pinMode(INTERRUPT_READ2, INPUT_PULLUP);
  pinMode(INTERRUPT_READ3, INPUT_PULLUP);
  pinMode(INTERRUPT_READ4, INPUT_PULLUP);
  pinMode(INTERRUPT_WRITE1, OUTPUT);
  pinMode(INTERRUPT_WRITE2, OUTPUT);
  pinMode(INTERRUPT_WRITE3, OUTPUT);
  pinMode(INTERRUPT_WRITE4, OUTPUT);
  digitalWrite(INTERRUPT_WRITE1, LOW);
  digitalWrite(INTERRUPT_WRITE2, LOW);
  digitalWrite(INTERRUPT_WRITE3, LOW);
  digitalWrite(INTERRUPT_WRITE4, LOW);
  
  Serial.println(F("LCD: Showing message"));
  Serial.println(F("********************"));
  Serial.println(full_message.substring( 0,20)); 
  Serial.println(full_message.substring(20,40));
  Serial.println(full_message.substring(40,60)); 
  Serial.println(full_message.substring(60));
  Serial.println(F("********************"));
  Serial.println();
  
  my_lcd.setCursor (0, 0);
  my_lcd.print(full_message.substring( 0,20));
  my_lcd.setCursor (0, 1);
  my_lcd.print(full_message.substring(20,40));
  my_lcd.setCursor (0, 2);
  my_lcd.print(full_message.substring(40,60));
  my_lcd.setCursor (0, 3);
  my_lcd.print(full_message.substring(60));
  
  int i = 0, interval_count;
  if (speed == "slow") {
    interval_count = 40;
  }
  else if (speed == "fast") {
    interval_count = 20;
  }
  else if (speed == "faster") {
    interval_count = 6;
  }
  else if (speed == "static") {
    interval_count = 0;
    _interrupt_flag = true;
  }
  else {
    interval_count = 30; // normal speed
  }
  while(_interrupt_flag == false) {
    if (i < interval_count && 
       (digitalRead(INTERRUPT_READ1) && digitalRead(INTERRUPT_READ2) &&
        digitalRead(INTERRUPT_READ3) && digitalRead(INTERRUPT_READ4))) {
    }
    else {
      _interrupt_flag = true;
      //Serial.println("LCD: Displayed message interrupted.");
    }
    delay(100);
    i++;
  }
}

void Liquid_Crystal_Display::
show_caution_message(void) {
  Serial.println(F("LCD: Showing \"CAUTION\" message ;)"));
  
  String caution;  
  caution  = "                    ";
  caution += "     Caution!!      ";
  caution += "    !!Caution!!     ";
  caution += "                    ";
  showMessage(caution);

  caution  = "                    ";
  caution += "    Grizzly bear    ";
  caution += "      inside!!      ";
  caution += "                    ";
  showMessage(caution);

  caution  = "                    ";
  caution += "       DO NOT       ";
  caution += "       OPEN!!       ";
  caution += "                    ";
  showMessage(caution);
  
  Serial.print(F("LCD: User has been warned about potential bears :D"));
  Serial.println();
  Serial.println();
}

void Liquid_Crystal_Display::
begin(void) {
  Serial.println("LCD: Begin initialization.");
  
  _interrupt_flag = false;
  
  my_lcd.begin(20, 4);
  
  // Switch on the backlight
  my_lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  my_lcd.setBacklight(HIGH);
  

  String init_str;
  // Begin writing startup message.
  init_str  = "        CCNY        ";
  init_str += "   Smart Grid Lab   ";
  init_str += "  Prof. A. Mohamed  ";
  init_str += "      ST 6/643      ";
  showMessage(init_str, "slow");
  
  // Show 'caution' message at random initializations :)
  randomSeed(analogRead(0));
  if (random(17) == 2 || random(17) == 11) {
    show_caution_message();
  }
  
  // End of startup messages.
  init_str  = "                    ";
  init_str += "  Ready for input.  ";
  init_str += "                    ";
  init_str += "                    ";
  showMessage(init_str, "faster");
  

  Serial.println("LCD: LCD initialized.");
  Serial.println();
}
