// This sketch tests the Arduino interface, which allows the use of functions
// that usually require an Arduino device and compiler.
//
// The following connections are needed to pass the tests in this sketch:
//    - 5 V at digital pin 2
//    - About 2.5 V at analog pin A0
//    - Optional floating wire at analog pin A1

#include "include/ArduinoInterface.h"
#include "include/HardwareArduino.h"

Arduino* arduino = new HardwareArduino;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Begin tests.\n"));

  readInputPin();
  readAnalogValue();
  printRandomNumbers();
  blinkLED();

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

void readInputPin() {
  const int INPUT_PIN = 2;
  const int EXPECTED_VALUE = 1;  // logic value for 5 V

  Serial.println(F("Test: readInputPin"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing arduino interface functions \"pinMode()\""));
  Serial.println(F("  and \"digitalRead()\""));
  Serial.println(F("- Check that the following input pin is connected"));
  Serial.println(F("  to 5 V."));
  Serial.print(F("  INPUT PIN: "));
  Serial.println(INPUT_PIN);
  waitForEnter();

  arduino->pinMode(INPUT_PIN, INPUT);
  Serial.print(F("- Value read at input pin: "));
  const int READ_VALUE = arduino->digitalRead(INPUT_PIN);
  Serial.println(READ_VALUE);
  printTestResult(READ_VALUE == EXPECTED_VALUE);
}

void readAnalogValue() {
  const uint8_t ANALOG_PIN = A0;
  const long TOLERANCE = 20;        // about 100mV
  const long EXPECTED_VALUE = 512;  // half of 1024 for 10-bit ADC

  Serial.println(F("Test: readAnalogValue"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the arduino interface function"));
  Serial.println(F("  \"analogRead()\""));
  Serial.println(F("- Check that the following analog input pin is"));
  Serial.println(F("  connected to approximately 2.5 V."));
  Serial.print(F("  ANALOG INPUT PIN: A"));
  Serial.println(ANALOG_PIN - A0);  // print silkscreen label, not pin number
  waitForEnter();

  Serial.print(F("- Analog reading (after ADC conversion): "));
  const long READ_VALUE = arduino->analogRead(ANALOG_PIN);
  Serial.println(READ_VALUE);

  const bool WITHIN_EXPECTED_RANGE =
      READ_VALUE >= (EXPECTED_VALUE - TOLERANCE) &&
      READ_VALUE <= (EXPECTED_VALUE + TOLERANCE);
  printTestResult(WITHIN_EXPECTED_RANGE);
}

void printRandomNumbers() {
  /*
  Re: Random numbers
  We can verify that the randomSeed function is working by rebooting the Arduino
  a few times, which provides a new seed on each reboot, and recording and
  comparing the printed set of numbers for randomness.

  NOTE: We seed the random number generator using a noisy analog reading. For
  more info, look at the official documenation for the randomSeed function on
  the "arduino.cc/reference/en" site. */

  const uint8_t SEED_PIN = A1;  // make different than ANALOG_PIN in prev. test
  const int LOWER_BOUND = 15;   // arbitrarily set
  const int UPPER_BOUND = 20;   // arbitrarily set

  Serial.println(F("Test: printRandomNumbers"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing arduino interface functions \"random()\""));
  Serial.println(F("  and \"randomSeed()\""));
  Serial.println(F("- Check that the following analog input pin is"));
  Serial.println(F("  either unconnected or connected with a floating"));
  Serial.println(F("  wire (to get a noisy reading)."));
  Serial.print(F("  ANALOG INPUT SEED PIN: A"));
  Serial.println(SEED_PIN - A0);  // print silkscreen label, not pin number
  waitForEnter();

  arduino->randomSeed(analogRead(SEED_PIN));

  Serial.print(F("- Printing 20 random numbers,  upper bound of "));
  Serial.println(UPPER_BOUND);
  for (uint8_t i = 0; i < 2; i++) {
    for (uint8_t j = 0; j < 10; j++) {
      Serial.print(arduino->random(UPPER_BOUND));
      Serial.print(' ');
    }
    Serial.println();
  }

  Serial.print(F("- Printing 20 random numbers with lower bound of "));
  Serial.println(LOWER_BOUND);
  Serial.print(F("  and upper bound of "));
  Serial.println(UPPER_BOUND);
  for (uint8_t i = 0; i < 2; i++) {
    for (uint8_t j = 0; j < 10; j++) {
      Serial.print(arduino->random(LOWER_BOUND, UPPER_BOUND));
      Serial.print(' ');
    }
    Serial.println();
  }

  Serial.println(F("- Verify that these numbers are random and within"));
  Serial.println(F("  the set bounds."));
  Serial.println(F("- Read sketch comment regarding random numbers."));
  // The referred comment is at the start of this function.

  waitForEnter();
}

void blinkLED() {
  Serial.println(F("Test: blinkLED"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing arduino interface functions"));
  Serial.println(F("  \"digitalWrite()\", \"delay()\", & "));
  Serial.println(F("  \"delayMicroseconds()\"."));
  Serial.println(F("- The LED on the Arduino board will blink 3 times"));
  Serial.println(F("  with a period of 2 seconds."));
  waitForEnter();

  pinMode(LED_BUILTIN, OUTPUT);  // pinMode not being tested here
  for (uint8_t n = 0; n < 3; n++) {
    arduino->digitalWrite(LED_BUILTIN, LOW);
    arduino->delay(1000);
    arduino->digitalWrite(LED_BUILTIN, HIGH);
    for (int i = 0; i < 1000; i++) arduino->delayMicroseconds(1000);
  }

  Serial.println(F("- Verify that the LED blinked as expected."));
  waitForEnter();
}
