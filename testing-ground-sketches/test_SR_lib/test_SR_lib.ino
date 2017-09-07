#include "Shift_Reg_for_load_emulator.h"

Shift_Registers shift_reg;

void setup() {
  Serial.begin(9600);
  
  //bool bool_array[48] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  bool bool_array[48] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  //bool bool_array[48] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  //bool bool_array[48] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  //bool bool_array[48] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  //byte bool_array[6] = {B10101010, B11110000, B00001111, B11000101, B00000001, B10000000};
  
  //shift_reg.set_Arduino_pins(7, 6, 5, 4);
  shift_reg.initialize(bool_array);

  Serial.println("First data is: ");
  for (int i = 0;i < 48; i++) {
    if (i < 16)
      bool_array[i] = true;
    else
      bool_array[i] = false;
    Serial.print(bool_array[i]);
    Serial.print(' ');
  }
  Serial.println();
  shift_reg.send_serial_data();
  shift_reg.trigger_output();
  Serial.println("First output sent.");
  delay(45000);
  
  Serial.println("Second data is: ");
  for (int i = 0;i < 48; i++) {
    if (i > 15 && i < 32)
      bool_array[i] = true;
    else
      bool_array[i] = false;
  Serial.print(bool_array[i]);
  Serial.print(' ');
  }
  Serial.println();
  shift_reg.send_serial_data();
  shift_reg.trigger_output();
  Serial.println("Second output sent.");

  /*
  for (int i = 0; i < 48; i++) {
    if (i % 2 == 0) {
      bool_array[i] = false;
    }
    else {
      bool_array[i] = true;
    }
  }
  Serial.println("First output sent.");
  shift_reg.send_serial_data();
  shift_reg.trigger_output();
  */
}

void loop() {

  
}
