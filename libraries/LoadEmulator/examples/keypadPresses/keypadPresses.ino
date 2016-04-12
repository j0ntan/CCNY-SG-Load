#include <Keypad_for_load_emulator.h>

Keypad keypad;

void setup() {
  Serial.begin(19200);
  keypad.initialize();
  Serial.println("Ready.");
}

void loop() {
  if(keypad.check_any_press()) {
    keypad.find_pressed_key();
    // Check serial monitor for indication of keypresses.
    delay(1);
  }
}
