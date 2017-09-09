#include <Keypad.h>

/*
This example shows how a keypress is detected and read. The character
corresponding to the pressed key is printed on the Serial Monitor and
another messaage indicates if the pressed key was held down past a
threshold time. */

Keypad keypad; // use default pins

void setup() {
  Serial.begin(19200);
  Serial.println("Ready.");
}

void loop() {
  if(keypad.isPressed()) {
    Serial.print("Pressed key: ");
    Serial.println(keypad.getKey());

    if(keypad.isHeld()) Serial.print("Button was held\n");

    Serial.println();

    while(keypad.isPressed())
      ;// Do nothing until key is released

    delay(300); // guard time between presses
  }
}
