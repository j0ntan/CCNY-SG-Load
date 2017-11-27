#include <Display.h>

/*
This example shows how messages are displayed on the LCD module.

The strings containing the messages should be formatted for a 4 x 20
display.

The duration of the display for a given message can be set to STATIC,
SLOW, FAST, FASTER, or NORMAL, which is the default case. At the end of
the duration, the proceeding instruction can be executed. For the STATIC
case the duration is effectively zero, i.e. the message is displayed and
the next instruction is immediately executed.

The display duration can be interrupted by any input from two Serial
ports, one of which is usually connected to an XBee, and from a connected
keypad. */

// Input sources that can interrupt display duration
HardwareSerial &xbee = Serial3;       // create an alias
Keypad keypad;                        // use default pins

// pass input souces; use default pins
Display disp(Serial, xbee, keypad);

void setup() {
  Serial.begin(19200);

  // pass number of cols & rows; an intro message gets displayed
  disp.begin(20, 4);

  String msg;

  // using the F macro to reduce sketch size
  msg  = F("                    ");
  msg += F("Display test msg    ");
  msg += F("Speed: NORMAL       ");
  msg += F("                    ");
  disp.showMessage(msg); // using the default NORMAL

  msg.replace(F("NORMAL"), F("FAST  "));
  disp.showMessage(msg, Display::Speed::FAST);

  msg.replace(F("FAST  "), F("FASTER"));
  disp.showMessage(msg, Display::Speed::FASTER);

  msg.replace(F("FASTER"), F("SLOW  "));
  disp.showMessage(msg, Display::Speed::SLOW);

  msg.replace(F("SLOW  "), F("STATIC"));
  disp.showMessage(msg, Display::Speed::STATIC);

  Serial.println("End of example.");
}

void loop() {}
