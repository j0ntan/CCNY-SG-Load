#include "../include/HardwareIO.h"
#include "Arduino.h"

HardwareInput::HardwareInput(uint8_t pin, bool enable_pullup) : pin(pin) {
  if (enable_pullup)
    pinMode(pin, INPUT_PULLUP);
  else
    pinMode(pin, INPUT);
}

bool HardwareInput::read() const { return digitalRead(pin) == HIGH; }

HardwareOutput::HardwareOutput(uint8_t pin) : pin(pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void HardwareOutput::set() const { digitalWrite(pin, HIGH); }

void HardwareOutput::clear() const { digitalWrite(pin, LOW); }
