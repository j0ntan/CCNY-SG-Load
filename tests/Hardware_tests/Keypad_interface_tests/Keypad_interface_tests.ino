// This sketch tests the keypad interface using a 4x4 matrix keypad.

#include "include/Keypad.h"
#include "include/HardwareKeypad.h"

const HardwareKeypad::pin R1 = 2, R2 = 3, R3 = 4, R4 = 5;
const HardwareKeypad::pin C1 = 6, C2 = 7, C3 = 8, C4 = 9;
HardwareKeypad keypad{R1, R2, R3, R4, C1, C2, C3, C4};

void setup() {
  Serial.begin(9600);
  Serial.println(F("Begin tests.\n"));

  verifyInitialization();
  detectAnyButtonPress();
  detectAnyButtonHeld();
  verifyAllButtonsPressed();

  Serial.println(F("End of tests."));
}

void loop() {}

void waitForEnter() {
  Serial.println(F("Press ENTER to continue...\n"));
  bool enterPressed = false;
  do {
    while (Serial.available())
      if (Serial.read() == '\n') enterPressed = true;
  } while (!enterPressed);
}

void printTestResult(bool result) {
  if (result)
    Serial.println(F("PASSED"));
  else
    Serial.println(F("FAILED"));

  waitForEnter();
}

void waitForDepress() {
  while (keypad.anyButtonPressed())
    ;
}

bool buttonPressedWithinFiveSeconds() {
  unsigned long time_elapsed = 0;
  const unsigned long TIME_INTERVAL = 10;
  bool button_was_pressed = false;

  while (!button_was_pressed && time_elapsed <= 5000) {
    delay(TIME_INTERVAL);
    time_elapsed += TIME_INTERVAL;
    button_was_pressed = keypad.anyButtonPressed();
  }

  return button_was_pressed;
}

void checkChosenButtonPressed(Keypad::Button chosenButton) {
  if (buttonPressedWithinFiveSeconds()) {
    if (chosenButton == keypad.getButton())
      Serial.println(F("PASSED"));
    else
      Serial.println(F("FAILED"));
  } else {
    Serial.println(F("FAILED, timeout"));
  }

  waitForDepress();
}

void verifyInitialization() {
  Serial.println(F("Test: verifyInitialization"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Check the state of the following keypad pin"));
  Serial.println(F("  connections (with an LED or voltmeter). Verify"));
  Serial.println(F("  that the column pins are at 0 V and row pins at"));
  Serial.println(F("  5 V."));
  Serial.print(F("  ROW pins: "));
  Serial.print(R1);
  Serial.print(F(", "));
  Serial.print(R2);
  Serial.print(F(", "));
  Serial.print(R3);
  Serial.print(F(", "));
  Serial.println(R4);
  Serial.print(F("  COLUMN pins: "));
  Serial.print(C1);
  Serial.print(F(", "));
  Serial.print(C2);
  Serial.print(F(", "));
  Serial.print(C3);
  Serial.print(F(", "));
  Serial.println(C4);

  waitForEnter();
}

void detectAnyButtonPress() {
  Serial.println(F("Test: detectAnyButtonPress"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the detection of a keypad button press."));
  waitForEnter();

  Serial.println(F("- Press any button on the keypad within the next"));
  Serial.println(F("  5 seconds."));
  printTestResult(buttonPressedWithinFiveSeconds());
}

void detectAnyButtonHeld() {
  Serial.println(F("Test: detectAnyButtonHeld"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the detection of a keypad button press &"));
  Serial.println(F("  hold."));
  waitForEnter();

  Serial.println(F("- Press & hold any button on the keypad within"));
  Serial.println(F("  the next 5 seconds."));
  uint16_t time_elapsed = 0;
  bool button_held = false;
  while (!button_held && time_elapsed <= 5000) {
    delay(10);
    time_elapsed += 10;
    button_held = keypad.anyButtonHeld();
  }
  printTestResult(button_held);
}

void verifyAllButtonsPressed() {
  const char *LABEL[] = {"number 0", "number 1", "number 2", "number 3",
                         "number 4", "number 5", "number 6", "number 7",
                         "number 8", "number 9", "A",        "B",
                         "C",        "D",        "*",        "#"};
  const Keypad::Button KEYS[] = {
      Keypad::Button::NUM0, Keypad::Button::NUM1, Keypad::Button::NUM2,
      Keypad::Button::NUM3, Keypad::Button::NUM4, Keypad::Button::NUM5,
      Keypad::Button::NUM6, Keypad::Button::NUM7, Keypad::Button::NUM8,
      Keypad::Button::NUM9, Keypad::Button::A,    Keypad::Button::B,
      Keypad::Button::C,    Keypad::Button::D,    Keypad::Button::STAR,
      Keypad::Button::HASH};

  Serial.println(F("Test: verifyAllButtonsPressed"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the detection of each individual keypad"));
  Serial.println(F("  button press."));
  waitForEnter();

  Serial.println(F("- Press the following indicated buttons, each"));
  Serial.println(F("  within a 5 second interval."));
  for (uint8_t i = 0; i < 16; i++) {
    Serial.print(F("- Press the '"));
    Serial.print(LABEL[i]);
    Serial.print(F("' button: "));
    checkChosenButtonPressed(KEYS[i]);
  }
  waitForEnter();
}
