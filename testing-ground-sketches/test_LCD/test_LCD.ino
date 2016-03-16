#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "LCD20x04.h"






char inputSequence[14] = {'?', '?', '?', '?',
                          '?', '?', '?', '?',
                          '?', '?', '?', '?',
                          '?', '?'
                         };
int currentState[4] = {10, 9, 10, 8};


void setup() {
  Serial.begin(9600);
  Serial.println("First message.");

  initializeLCD();
  //delay(3500);
  delay(1000);
  //cautionMessage();

  LCDprintCurrentState(currentState);
  delay(5000);

  inputSequence[0] = 'A';
  LCDprintInputSequence('A', inputSequence);
  delay(3000);

  inputSequence[1] = '3';
  LCDprintInputSequence('3', inputSequence);
  delay(3000);

  inputSequence[0] = 'A';
  inputSequence[1] = '9';
  inputSequence[2] = 'B';
  inputSequence[3] = '1';
  inputSequence[4] = '0';
  inputSequence[5] = 'C';
  inputSequence[6] = '9';
  //inputSequence[7] = '#';
  LCDprintInputSequence('9', inputSequence);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}











