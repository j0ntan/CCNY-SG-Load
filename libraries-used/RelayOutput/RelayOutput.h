#include "Arduino.h"

int DC_pins[8] = {22,23,24,25,26,27,28,29};

// Initialize the Mux Shield object
MuxShield muxShield;


void initializeMuxShield(void) {
  // Set Mux rows as output pins
  muxShield.setMode(1, DIGITAL_OUT);
  muxShield.setMode(2, DIGITAL_OUT);
  muxShield.setMode(3, DIGITAL_OUT);

  // Set rows 1-3 to LOW to prevent turning
  // on relays during initializtion
  for (int i = 0; i < 16; i++) {
    muxShield.digitalWriteMS(1, i, 0);
    muxShield.digitalWriteMS(2, i, 0);
    muxShield.digitalWriteMS(3, i, 0);
  }
  
  // Initialize the output pins for DC load.
  for(int i  = 0; i < 8; i++) {
    pinMode(DC_pins[i], OUTPUT);
	digitalWrite(DC_pins[i], LOW);
  }
  
  Serial.println("MuxShield is ready.");
}

void initializeLatch(int LE1, int LE2, int LE3, int OE) {
  // LE = Latch Enable pin
  // OE = Output Enable pin

  pinMode(LE1, OUTPUT);
  pinMode(LE2, OUTPUT);
  pinMode(LE3, OUTPUT);
  pinMode(OE, OUTPUT);

  digitalWrite(OE, LOW);
  digitalWrite(LE1, HIGH);
  digitalWrite(LE2, HIGH);
  digitalWrite(LE3, HIGH);
  digitalWrite(LE1, LOW);
  digitalWrite(LE2, LOW);
  digitalWrite(LE3, LOW);
  Serial.println("Output Latch Initialized.");
}

void writeLatch(int row, int LE1, int LE2, int LE3) { 
// LE = Latch Enable pin
  if (row == 0)
    digitalWrite(LE1, HIGH), digitalWrite(LE1, LOW);
  else if (row == 1)
    digitalWrite(LE2, HIGH), digitalWrite(LE2, LOW);
  else if (row == 2)
    digitalWrite(LE3, HIGH), digitalWrite(LE3, LOW);
  else;
}

void restartOutput(void) {
  for (int i = 0; i < 16; i++) {
    muxShield.digitalWriteMS(1, i, 0);
    muxShield.digitalWriteMS(2, i, 0);
    muxShield.digitalWriteMS(3, i, 0);
  }
}

void activateOutput(int LE1, int LE2, int LE3) {
  // LE = Latch Enable pin

  bool finalCheck = true;
  for (int i = 0; i < 4; i++) {
    if (desiredState[i] < -1 || desiredState[i] > 16)
      finalCheck = false;
  }
  if(desiredState[3]>8)
	  finalCheck = false;

  if (finalCheck) {
    Serial.println("Output activated.");

    // Make changes to A,B,C pins
    for (int i = 0; i < 3; i++) {
      if (desiredState[i] == -1 || desiredState[i] == currentState[i]) {
        // This line does not get changed
        Serial.print("No changes made to ");
        Serial.print(char(int('A') + i));
        Serial.println(".");
      }
      else if (desiredState[i] > currentState[i]) {
        Serial.print(char(int('A') + i)), Serial.println(" increased");
        for (int j = 0; j < 16; j++)
          if (j < desiredState[i])
            muxShield.digitalWriteMS(i + 1, j, 1);
          else
            muxShield.digitalWriteMS(i + 1, j, 0);
        writeLatch(i, LE1, LE2, LE3);
        currentState[i] = desiredState[i];
      }
      else if (desiredState[i] < currentState[i]) {
        Serial.print(char(int('A') + i)), Serial.println(" decreased");
        for (int j = 0; j < 16; j++)
          if (j < desiredState[i])
            muxShield.digitalWriteMS(i + 1, j, 1);
          else
            muxShield.digitalWriteMS(i + 1, j, 0);
        writeLatch(i, LE1, LE2, LE3);
        currentState[i] = desiredState[i];
      }
      else {
        Serial.print("Couldn't get desired state of ");
        Serial.print(char(int('A') + i));
        Serial.println(". No changes made.");
      }

      restartOutput();
      //delay(500);
    }

    // Make changes to line D
    if (desiredState[3] == -1 || desiredState[3] == currentState[3]) {
      // This line does not get changed
      Serial.println("No changes made to D");
    }
    else if (desiredState[3] > currentState[3]) {
      Serial.println("D increased");
      for (int j = 0; j < 8; j++)
        if (j < desiredState[3])
          digitalWrite(DC_pins[j], HIGH);
        else
          digitalWrite(DC_pins[j], LOW);
      currentState[3] = desiredState[3];
    }
    else if (desiredState[3] < currentState[3]) {
      Serial.println("D decreased");
      for (int j = 0; j < 8; j++)
        if (j < desiredState[3])
          digitalWrite(j + 22, HIGH);
        else
          digitalWrite(j + 22, LOW);
      currentState[3] = desiredState[3];
    }
    else
      Serial.println("Couldn't get desired state of D. No changes made.");


    // Print current state after making changes
    Serial.print("Current states: ");
    Serial.print(currentState[0]), Serial.print(" ");
    Serial.print(currentState[1]), Serial.print(" ");
    Serial.print(currentState[2]), Serial.print(" ");
    Serial.print(currentState[3]), Serial.print(" ");
    Serial.println();
  }
  else {
    Serial.println("Final check has failed. Investigate further.");
    Serial.println("Output not changed");
  }
}