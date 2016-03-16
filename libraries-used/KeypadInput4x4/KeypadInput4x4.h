//#include "..\MuShield\MuxShield.h"
#include "Arduino.h"


//---------------------------------------------------------------//
//-----------------------Global Variables-----------------------//
//---------------------------------------------------------------//


/* Holds the Arduino pin numbers that connect to the keypad. 
Assign the Arduino digital input pins for reading inputs
from the keypad L lines. */
int keypad_L_pins[4] = {30, 31, 32, 33}; // use with push button keypad
//int keypad_L_pins[4] = {2, 3, 4, 5};     // use with flat membrane keypad

// Assign the Mux shield digital output pins
// (row 3, column i) for writing to the keypad R lines.
int keypad_R_pins[4] = {34, 35, 36, 37};
//int keypad_R_pins[4] = {6, 7, 8, 9};

// Used to keep track of pressed buttons
// by their rows and column status
bool keypad_row_pressed[4] = {0, 0, 0, 0};
bool keypad_col_pressed[4][4] = {
  {0, 0, 0, 0}, {0, 0, 0, 0},
  {0, 0, 0, 0}, {0, 0, 0, 0}
}; // this variable stores columns pressed for each row

// Used to store the sequence of pressed keys
// and operate relays after a SEND(#) input.
// Max of 7 values for the case of an input
// A11 B12 C13 D8* where all the relays are turned on.
char inputSequence[14] = {'?', '?', '?', '?',
                          '?', '?', '?', '?',
						  '?', '?', '?', '?',
                          '?', '?'
                         };

// Used to keep track of stored valid inputs.
int inputCount = 0;

// Stores values of changes made by the user based on the input.
// First 3 values correspond to number of resistors turned on for 
// each AC phase. Last value is for DC load. A value of 0 means that
// line will have 0 resistors connected. A value of -1 means that line
// will remain unchanged. 
int desiredState[4] = {-1,-1,-1,-1};

// Stores the number of resistors turned on for each line. 
// Values range from 0-16.
int currentState[4] = {0,0,0,0};

//---------------------------------------------------------------//
//---------------------Global Variables end---------------------//
//---------------------------------------------------------------//













//---------------------------------------------------------------//
//---------------------Function Definitions---------------------//
//---------------------------------------------------------------//
void initializeKeypadIO(void) {

  // Initialize Arduino input pins for keypad
  for (int i = 0; i < 4; i++)
    pinMode(keypad_L_pins[i], INPUT_PULLUP);

  // Initialize Arduino ouput pins for keypad
  for (int i = 0; i < 4; i++) {
    pinMode(keypad_R_pins[i], OUTPUT);
    digitalWrite(keypad_R_pins[i],0);
  }
  Serial.println("Keypad Initialized.");
}
// Use this for manually inputting the Keypad pins
void initializeKeypadIO(int L1, int L2, int L3, int L4,
                        int R1, int R2, int R3, int R4) {

  // Initialize Arduino input pins for keypad
  pinMode(L1, INPUT_PULLUP);
  pinMode(L2, INPUT_PULLUP);
  pinMode(L3, INPUT_PULLUP);
  pinMode(L4, INPUT_PULLUP);

  // Initialize Arduino ouput pins for keypad
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  digitalWrite(R1, 0);
  digitalWrite(R2, 0);
  digitalWrite(R3, 0);
  digitalWrite(R4, 0);
    
  Serial.println("Keypad Initialized.");
}

bool detectButtonPress(void) {
  keypad_row_pressed[0] = !digitalRead(keypad_L_pins[0]);
  keypad_row_pressed[1] = !digitalRead(keypad_L_pins[1]);
  keypad_row_pressed[2] = !digitalRead(keypad_L_pins[2]);
  keypad_row_pressed[3] = !digitalRead(keypad_L_pins[3]);

  return keypad_row_pressed[0] || keypad_row_pressed[1] ||
         keypad_row_pressed[2] || keypad_row_pressed[3];

}

void findWhichKeys(void) {
  for (int i = 0; i < 4; i++) {
    if (keypad_row_pressed[i]) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(keypad_R_pins[j], HIGH);
        if (digitalRead(keypad_L_pins[i]))
          keypad_col_pressed[i][j] = 1;
        else
          keypad_col_pressed[i][j] = 0;
        digitalWrite(keypad_R_pins[j], LOW);
        // use this line to print 4x4 results
        //Serial.print(keypad_col_pressed[i][j]),Serial.print(" ");
      }
    }
    else
      for (int j = 0; j < 4; j++) {
        keypad_col_pressed[i][j] = 0;
        // use this line to print 4x4 results
        //Serial.print(keypad_col_pressed[i][j]),Serial.print(" ");
      }
    // use this line to print 4x4 results
    //Serial.println();
  }
}

char getKeyChar(void) {
  int keypressCount = 0;
  int keyID = 0;
  char keyChar = '?';
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (keypad_col_pressed[i][j])
        keypressCount++;

  // If one key is pressed, get the keyID
  if (keypressCount == 1) {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        if (keypad_col_pressed[i][j]) {
          keyID = i * 4 + (j + 1);
          break;
        }

    // Using the keyID, find the corresponding
    // keypad charactaer.
    switch (keyID) {
      case 1: return '1';
      case 2: return '2';
      case 3: return '3';
      case 4: return 'A';
      case 5: return '4';
      case 6: return '5';
      case 7: return '6';
      case 8: return 'B';
      case 9: return '7';
      case 10: return '8';
      case 11: return '9';
      case 12: return 'C';
      case 13: return '*';
      case 14: return '0';
      case 15: return '#';
      case 16: return 'D';
      default: return '?';
    }
  }
  else
    // More than one key was pressed. Indicate this by
    // returning the '!' character.
    keyChar = '!';
  return keyChar;
}

void storeKeyInput(char keyInput) {
  if (keyInput == '!') {
    // More than one keypress has been input.
    // Notify the user and don't change the sequence.
    Serial.println("Multiple keypress detected.");
    Serial.println("Input sequence unchanged");
  }
  else {
    // Check that the first input is a letter.
    if (inputCount == 0 &&
        keyInput != 'A' && keyInput != 'B' && keyInput != 'C' && keyInput != 'D'
       )
      Serial.println("First input must be ABC or D");
    else {
      inputSequence[inputCount] = keyInput;
      if (inputCount != 12)
        inputCount++;
    }
  }

  Serial.print("input sequence: ");
  for ( int i = 0; i < 13; i++)
    if (inputSequence[i] != '?')
      Serial.print(inputSequence[i]), Serial.print(' ');
  Serial.println();
}

bool checkForENTER(char keyChar) {
  if (keyChar == '#')
    return true;
  else
    return false;
}

bool checkForValidInput(void) {
  int letterCount = 0;
  int Acount = 0;
  int Bcount = 0;
  int Ccount = 0;
  int Dcount = 0;
  int NumCount = 0;
  // Used to count digit frequency (0-9) in input
  int DigitCount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int tensCount = 0;
  int invalidChars = 0;

  for ( int i = 0; i < 14; i++) {
    // Count the frequency of input chars
    switch (inputSequence[i]) {
      case 'A': Acount++; letterCount++; break;
      case 'B': Bcount++; letterCount++; break;
      case 'C': Ccount++; letterCount++; break;
      case 'D': Dcount++; letterCount++; break;
      case '1': NumCount++; DigitCount[1]++; break;
      case '2': NumCount++; DigitCount[2]++; break;
      case '3': NumCount++; DigitCount[3]++; break;
      case '4': NumCount++; DigitCount[4]++; break;
      case '5': NumCount++; DigitCount[5]++; break;
      case '6': NumCount++; DigitCount[6]++; break;
      case '7': NumCount++; DigitCount[7]++; break;
      case '8': NumCount++; DigitCount[8]++; break;
      case '9': NumCount++; DigitCount[9]++; break;
      case '0': NumCount++; DigitCount[0]++; break;
      case '#': break;
      case '*': break;
      case '?': break;
      default: invalidChars++;
    }

    // Count the frequency of the numbers 10-16
    if (i < 12 && inputSequence[i] == '1' &&
        ( inputSequence[i + 1] == '0' ||  inputSequence[i + 1] == '1' ||
          inputSequence[i + 1] == '2' ||  inputSequence[i + 1] == '3' ||
          inputSequence[i + 1] == '4' ||  inputSequence[i + 1] == '5' ||
          inputSequence[i + 1] == '6'
        )
       )
      tensCount++;

    // Check that a number doesn't follow a number
    // except for 1 followed by 0-6.
    if (i < 12 && inputSequence[i] == '1' &&
        (inputSequence[i + 1] == '7' || inputSequence[i + 1] == '8' ||
         inputSequence[i + 1] == '9') ) {
      Serial.println("Numbers not valid, must be within 0-16.");
      Serial.println("Re-enter input sequence.");
      return false;
    }
    else if (
      i < 12 && (
        inputSequence[i] == '0' || inputSequence[i] == '2' ||
        inputSequence[i] == '3' || inputSequence[i] == '4' ||
        inputSequence[i] == '5' || inputSequence[i] == '6' ||
        inputSequence[i] == '7' || inputSequence[i] == '8' ||
        inputSequence[i] == '9'
      ) && (
        inputSequence[i + 1] == '0' || inputSequence[i + 1] == '1' ||
        inputSequence[i + 1] == '2' || inputSequence[i + 1] == '3' ||
        inputSequence[i + 1] == '4' || inputSequence[i + 1] == '5' ||
        inputSequence[i + 1] == '6' || inputSequence[i + 1] == '7' ||
        inputSequence[i + 1] == '8' || inputSequence[i + 1] == '9' )) {
      Serial.println("Numbers not valid, must be within 0-16.");
      Serial.println("Re-enter input sequence.");
      return false;
    }
  }




  // Check that there are no invalid characters
  // in the input sequence. (Still not implemented)
  if (invalidChars > 0) {
    Serial.println("Invalid Characters detected");
    Serial.println("Re-enter input sequence.");
    return false;
  }
  
  // Check for a max of 4 letters (ABCD) and 8 numbers (16,16,16,16)
  else if (letterCount > 4 || NumCount > 8) {
    Serial.println("Too many letters or numbers.");
    Serial.println("Re-enter input sequence.");
    return false;
  }

  // Check for repeated letters
  else if (Acount > 1 || Bcount > 1 || Ccount > 1 || Dcount > 1) {
    Serial.println("Repeated letters not allowed. ");
    Serial.println("Re-enter input sequence.");
    return false;
  }

  // Check for inputs with only letters or numbers
  else if (NumCount == 0 || letterCount == 0) {
    Serial.println("Missing some arguments.");
    Serial.println("Re-enter input sequence.");
    return false;
  }

  // Check for repeated numbers (0-9)
  else if (DigitCount[0] > 4 || DigitCount[1] > 4 ||
           DigitCount[2] > 4 || DigitCount[3] > 4 ||
           DigitCount[4] > 4 || DigitCount[5] > 4 ||
           DigitCount[6] > 4 || DigitCount[7] > 4 ||
           DigitCount[8] > 4 || DigitCount[9] > 4) {
    Serial.println("Max digit repetition (x4) exceeded.");
    Serial.println("Re-enter input sequence.");
    return false;
  }

  // Check that a balanced input must be ABC $ or ABCD $
  // or ABC $ D $, where $ is some number 0-16, in that order.
  else if (Acount == 1 && Bcount == 1 && Ccount == 1) {
    if (inputSequence[1] != 'A' && inputSequence[1] != 'B' &&
        inputSequence[1] != 'C' && inputSequence[1] != 'D' )
        ; // There is some number in second char in sequence.
    else {
      if (inputSequence[0] != 'A' || inputSequence[1] != 'B' ||
          inputSequence[2] != 'C') {
        Serial.println("Balanced load detected with incorrect order (ABC).");
        Serial.println("Re-enter input sequence.");
        return false;
      }
    }
  }
  
  return true;
}

bool checkForCANCEL(char keyChar) {
  if (keyChar == '*')
    return true;
  else
    return false;
}

void resetSequence(void) {
  for (int i = 0; i < 14; i++)
    inputSequence[i] = '?';
  inputCount = 0;
  
  desiredState[0] = -1;
  desiredState[1] = -1;
  desiredState[2] = -1;
  desiredState[3] = -1;
}



int getNumFromChar(char numericalChar) {
  switch (numericalChar) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
  }
}


void setAstate(int Apos) {
  if (inputSequence[Apos + 1] == '1')
    if (inputSequence[Apos + 2] == '0')
      desiredState[0] = 10;
    else if (inputSequence[Apos + 2] == '1')
      desiredState[0] = 11;
    else if (inputSequence[Apos + 2] == '2')
      desiredState[0] = 12;
    else if (inputSequence[Apos + 2] == '3')
      desiredState[0] = 13;
    else if (inputSequence[Apos + 2] == '4')
      desiredState[0] = 14;
    else if (inputSequence[Apos + 2] == '5')
      desiredState[0] = 15;
    else if (inputSequence[Apos + 2] == '6')
      desiredState[0] = 16;
    else
      desiredState[0] = 1;
  else
    desiredState[0] = getNumFromChar(inputSequence[Apos + 1]);
}
void setBstate(int Bpos) {
  if (inputSequence[Bpos + 1] == '1')
    if (inputSequence[Bpos + 2] == '0')
      desiredState[1] = 10;
    else if (inputSequence[Bpos + 2] == '1')
      desiredState[1] = 11;
    else if (inputSequence[Bpos + 2] == '2')
      desiredState[1] = 12;
    else if (inputSequence[Bpos + 2] == '3')
      desiredState[1] = 13;
    else if (inputSequence[Bpos + 2] == '4')
      desiredState[1] = 14;
    else if (inputSequence[Bpos + 2] == '5')
      desiredState[1] = 15;
    else if (inputSequence[Bpos + 2] == '6')
      desiredState[1] = 16;
    else
      desiredState[1] = 1;
  else
    desiredState[1] = getNumFromChar(inputSequence[Bpos + 1]);
}
void setCstate(int Cpos) {
  if (inputSequence[Cpos + 1] == '1')
    if (inputSequence[Cpos + 2] == '0')
      desiredState[2] = 10;
    else if (inputSequence[Cpos + 2] == '1')
      desiredState[2] = 11;
    else if (inputSequence[Cpos + 2] == '2')
      desiredState[2] = 12;
    else if (inputSequence[Cpos + 2] == '3')
      desiredState[2] = 13;
    else if (inputSequence[Cpos + 2] == '4')
      desiredState[2] = 14;
    else if (inputSequence[Cpos + 2] == '5')
      desiredState[2] = 15;
    else if (inputSequence[Cpos + 2] == '6')
      desiredState[2] = 16;
    else
      desiredState[2] = 1;
  else
    desiredState[2] = getNumFromChar(inputSequence[Cpos + 1]);
}
void setDstate(int Dpos) {
  if (inputSequence[Dpos + 1] == '1')
    if (inputSequence[Dpos + 2] == '0')
      desiredState[3] = 10;
    else if (inputSequence[Dpos + 2] == '1')
      desiredState[3] = 11;
    else if (inputSequence[Dpos + 2] == '2')
      desiredState[3] = 12;
    else if (inputSequence[Dpos + 2] == '3')
      desiredState[3] = 13;
    else if (inputSequence[Dpos + 2] == '4')
      desiredState[3] = 14;
    else if (inputSequence[Dpos + 2] == '5')
      desiredState[3] = 15;
    else if (inputSequence[Dpos + 2] == '6')
      desiredState[3] = 16;
    else
      desiredState[3] = 1;
  else
    desiredState[3] = getNumFromChar(inputSequence[Dpos + 1]);
}

void getDesiredState(void) {
  // This function will look for several input combinations.
  // If the input sequence doesn't match these combinations,
  // then the output will not change. The $ symbol indicates
  // a number in range 0-16. Order of letters is important
  // (Must be A,B,C,D).

  // Balanced AC combos:
  // ABC $
  // ABC $ D $
  // ABCD $

  // Unbalanced AC combos (no DC):
  // (A,B, or C)&                             (for example B14)
  // (A,B, or C)$ (A,B, or C)$                (for example A3 C4)
  // A$ B$ C$                                 (for example A2 B3 C4)

  // Unbalanced AC + DC combos:
  // (A,B, or C)& D$                          (for example B11 D4)
  // (A,B, or C)$ (A,B, or C)$ D$             (for example A3 C12 D0)
  // (A,B, or C)(A,B, or C)$                  (for example BC4) (this case works)
  // (A,B, or C)(A,B, or C)$ D$               (for example BC4 D8) (not working yet)
  // A$ B$ C$ D$                              (for example A2 B3 C4 D7)

  // Only DC:
  // D$                                       (for example D5)


  int Acount = 0, Apos = 0;
  int Bcount = 0, Bpos = 0;
  int Ccount = 0, Cpos = 0;
  int Dcount = 0, Dpos = 0;
  int letterCount = 0;
  for ( int i = 0; i < 13; i++) {
    switch (inputSequence[i]) {
      case 'A': Acount++; letterCount++; Apos = i; break;
      case 'B': Bcount++; letterCount++; Bpos = i; break;
      case 'C': Ccount++; letterCount++; Cpos = i; break;
      case 'D': Dcount++; letterCount++; Dpos = i; break;
    }
  }

  // Look for Balanced AC (i.e., ABC $)
  if (inputSequence[Apos] == 'A' && inputSequence[Apos + 1] == 'B' &&
      inputSequence[Apos + 2] == 'C') {
    Serial.println("ABC detected somewhere");
    if (Dcount == 1) {
      Serial.println("D also detected");
      if (inputSequence[Cpos + 1] == 'D')
      {
        Serial.println("ABCD deteceted together");
        // This is the case for ABCD$
        setAstate(Dpos);
        setBstate(Dpos);
        setCstate(Dpos);
        setDstate(Dpos);
        // CASE FINISHED
      }
      else
      {
        if (Dpos > Apos) {
          // This is the case for ABC$ D$
          Serial.println("ABC$ D$ detected");
          // First set state for ABC
          setAstate(Cpos);
          setBstate(Cpos);
          setCstate(Cpos);
          // Second, set state for D
          setDstate(Dpos);
          // CASE FINISHED
        }
        else {
          Serial.println("D$ ABC$ detected");
          // This is the incorrect case of D$ ABC$
          for (int i = 0; i < 4; i++)
            desiredState[i] = -1;
          Serial.println("AC load parameters must be first.");
          Serial.println("Output not changed. Re-enter sequence.");
          // CASE FINISHED
        }
      }
    }
    else
    {
      Serial.println("ABC$ detected");
      // This is the case for ABC $
      setAstate(Cpos);
      setBstate(Cpos);
      setCstate(Cpos);
      // CASE FINISHED
    }
  }
  else if (Acount == 1 || Bcount == 1 || Ccount == 1) {
    if (Dcount == 1) {
      if (letterCount == 2) {
        Serial.println("(A,B, or C)$ D$ detected");
        // This is the case for (A,B, or C)$ D$

        if (Acount == 1 && Dpos > Apos) {
          Serial.println("A$ D$ detected");
          // Set state for A
          setAstate(Apos);
          //Set state for D
          setDstate(Dpos);
          // Set state for B,C
          desiredState[1] = -1;
          desiredState[2] = -1;
          // CASE FINISHED
        }
        else if (Bcount == 1 && Dpos > Bpos) {
          Serial.println("B$ D$ detected");
          // Set state for B
          setBstate(Bpos);
          //Set state for D
          setDstate(Dpos);
          // Set state for A,C
          desiredState[0] = -1;
          desiredState[2] = -1;
          // CASE FINISHED
        }
        else if (Ccount == 1 && Dpos > Cpos) {
          Serial.println("C$ D$ detected");
          // Set state for C
          setCstate(Cpos);
          //Set state for D
          setDstate(Dpos);
          // Set state for A,B
          desiredState[0] = -1;
          desiredState[1] = -1;
          // CASE FINISHED
        }
        else {
          Serial.println("Some error found with (A,B,C)$ D$");
          Serial.println("Possible error with order of letters.");
          Serial.println("Output not changed. Re-enter sequence.");
          // CASE FINISHED
        }
      }
      else if (letterCount == 3) {
        Serial.println("(A,B, or C)$ (A,B, or C)$ D$ detected");
        // This is the case for (A,B, or C)$ (A,B, or C)$ D$
        if (Acount == 1 && Bcount == 1 && Bpos > Apos && Dpos > Bpos) {
          Serial.println("A$ B$ D$ detected");
          // Set state for A
          setAstate(Apos);
          // Set state for B
          setBstate(Bpos);
          //Set state for D
          setDstate(Dpos);
          // Set state for C
          desiredState[2] = -1;
          // CASE FINISHED
        }
        else if (Acount == 1 && Ccount == 1 && Cpos > Apos && Dpos > Cpos) {
          Serial.println("A$ C$ D$ detected");
          // Set state for A
          setAstate(Apos);
          // Set state for C
          setCstate(Cpos);
          //Set state for D
          setDstate(Dpos);
          // Set state for B
          desiredState[1] = -1;
          // CASE FINISHED
        }
        else if (Bcount == 1 && Ccount == 1 && Cpos > Bpos && Dpos > Cpos) {
          Serial.println("B$ C$ D$ detected");
          // Set state for B
          setBstate(Bpos);
          // Set state for C
          setCstate(Cpos);
          //Set state for D
          setDstate(Dpos);
          // Set state for A
          desiredState[0] = -1;
          // CASE FINISHED
        }
        else {
          Serial.println("Some error found with (A,B,C)$ (A,B,C)$ D$");
          Serial.println("Possible error with order of letters.");
          Serial.println("Output not changed. Re-enter sequence.");
          // CASE FINISHED
        }
      }
      else if (letterCount == 4) {
        Serial.println("A$ B$ C$ D$ detected");
        // This is the case for A$ B$ C$ D$
        if (Dpos > Cpos && Cpos > Bpos && Bpos > Apos) {
          setAstate(Apos);
          setBstate(Bpos);
          setCstate(Cpos);
          setDstate(Dpos);
          // CASE FINISHED
        }
        else {
          Serial.println("Some error found with A$ B$ C$ D$");
          Serial.println("Possible error with order of letters.");
          Serial.println("Output not changed. Re-enter sequence.");
          // CASE FINISHED
        }
      }
    }
    else {
      if (letterCount == 1) {
        // This is the case for (A,B, or C)$
        Serial.println("(A,B, or C)$ detected");
        if (Acount == 1)
          setAstate(Apos);
        // CASE FINISHED
        else if (Bcount == 1)
          setBstate(Bpos);
        // CASE FINISHED
        else if (Ccount == 1)
          setCstate(Cpos);
        // CASE FINISHED
        else {
          Serial.println("Some error found with (A,B,C)$");
          Serial.println("Investigate further");
          Serial.println("Output not changed. Re-enter sequence.");
          // CASE FINISHED
        }
      }
      else if (letterCount == 2) {
        // This is the case for (A,B, or C)($)(A,B, or C)$
        Serial.println("(A,B, or C)($)(A,B, or C)$ detected");
        if (Acount == 1 && Bcount == 1 && Bpos > Apos) {
          if (Bpos == Apos + 1) {
            Serial.println("AB$ detected");
            // Set state for A
            setAstate(Bpos);
            // Set state for B
            setBstate(Bpos);
            // CASE FINISHED
          }
          else {
            // This is the case for (A,B, or C)$ (A,B, or C)$
            Serial.println("A$ B$ detected");
            // Set state for A
            setAstate(Apos);
            // Set state for B
            setBstate(Bpos);
            // CASE FINISHED
          }
        }
        else if (Acount == 1 && Ccount == 1 && Cpos > Apos) {
          if (Cpos == Apos + 1) {
            Serial.println("AC$ detected");
            // Set state for A
            setAstate(Cpos);
            // Set state for B
            setCstate(Cpos);
          }
          else {
            Serial.println("A$ C$ detected");
            // Set state for A
            setAstate(Apos);
            // Set state for C
            setCstate(Cpos);
            // CASE FINISHED
          }
        }
        else if (Bcount == 1 && Ccount == 1 && Cpos > Bpos) {
          if (Cpos == Bpos + 1) {
            Serial.println("BC$ detected");
            // Set state for B
            setBstate(Cpos);
            // Set state for C
            setCstate(Cpos);
            // CASE FINISHED
          }
          else {
            Serial.println("B$ C$ detected");
            // Set state for B
            setBstate(Bpos);
            // Set state for C
            setCstate(Cpos);
            // CASE FINISHED
          }

        }
        else {
          Serial.println("Some error found with (A,B,C)$ (A,B,C)$");
          Serial.println("Possible error with order of letters.");
          Serial.println("Output not changed. Re-enter sequence.");
          // CASE FINISHED
        }
      }
      else if (letterCount == 3) {
        // This is the case for A$ B$ C$
        Serial.println("A$ B$ C$ detected");
        if (Cpos > Bpos && Bpos > Apos) {
          setAstate(Apos);
          setBstate(Bpos);
          setCstate(Cpos);
        }
        else {
          Serial.println("Some error found with A$ B$ C$");
          Serial.println("Possible error with order of letters.");
          Serial.println("Output not changed. Re-enter sequence.");
        }
      }
    }
  }
  else if (Dcount == 1 && letterCount == 1) {
    Serial.println("D$ detected");
    // This is the case for D$
    setDstate(Dpos);
  }
  else {
    Serial.println("no case match detected");
    // This is the case where the input doesn't match any
    // expected cases. Thus, the output will not change state
    // and the user gets a message.
    for (int i = 0; i < 4; i++)
      desiredState[i] = -1;
    Serial.println("Unexpected input");
    Serial.println("Output not changed. Re-enter sequence.");
  }
  // for testing purposes
  for (int i = 0; i < 4; i++)
    Serial.print(desiredState[i]), Serial.print(" ");
  Serial.println();
}




void readAllSerialInput(void) {
  while (Serial.available() > 0) {
    inputSequence[inputCount] = char(Serial.read());
    if (inputCount != 12)
      inputCount++;
  }
  inputSequence[inputCount] = '#';

  Serial.print("Registered serial input: ");
  for ( int i = 0; i < 14; i++)
    if (inputSequence[i] != '?')
      Serial.print(inputSequence[i]), Serial.print(' ');
  Serial.println();
}


//---------------------------------------------------------------//
//-------------------Function Definitions end-------------------//
//---------------------------------------------------------------//