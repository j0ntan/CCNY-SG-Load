#include "LCD_for_load_emulator.h"

Liquid_Crystal_Display LCD;
String load_status, input_status;

void setup() {
  Serial.begin(9600);
  LCD.initialize();

  load_status  = "                    ";
  load_status += "Load status         ";
  load_status += "A:0 B:0 C:0 D:0     ";
  load_status += "                    ";
  LCD.show_message(load_status);
  
  Serial.println("EOF");
}

void loop() {
  if(!digitalRead(13) || !digitalRead(12) || 
     !digitalRead(11) || !digitalRead(10)) {
    Serial.println("Low");
  }
  delay(100);
}
