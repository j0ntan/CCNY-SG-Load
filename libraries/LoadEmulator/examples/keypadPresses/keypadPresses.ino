#include <Keypad_for_load_emulator.h>

Keypad keypad;

void setup() {
  Serial.begin(19200);

  keypad.begin();
  //keypad.begin(1,2,3,4,5,6,7,8); // Use this instead to use non-default Arduino-keypad pins,
                                   // replace numbers 1-8 to your connection pins.

  Serial.println("Ready.");
}

void loop() {
  if(keypad.isPressed()) {
    keypad.getKey();
    // Check serial monitor for indication of keypresses.

    if(keypad.heldStatus) {
      Serial.println("The button was held.");
      Serial.println("Pausing for 2 seconds.");
      delay(2000);
      Serial.println("Finished pause.");
      Serial.println();
    }
    delay(1);
  }
}
