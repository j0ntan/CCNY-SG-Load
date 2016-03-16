#include <MuxShield.h>

// Initialize the Mux Shield
MuxShield muxShield;

  
// Assign the Mux shield digital I/O pins
// (row,column)for reading inputs from the keypad
// L lines and writing to the R lines.
int keypad_LR_pins[2][4] = {{ 1, 2}, {15, 14, 13, 12}};

// Assign the Mux shield digital I/O pins
// (row, column) for relay outputs for
// AC 3-phase (A,B,C)loads (resistors 1-10)
int AC_3phase_Load_pins[2][10] = {{1, 2, 3}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

// Assign the Mux shield diugital I/O pins
// (row, column) for relay outputs for
// DC loads (resistors 1-10)
int DC_Load_pins[2][6] = {{1, 2, 3}, {10, 11, 12, 13, 14, 15}};

// Used to keep track of pressed buttons
// by their rows and column status
bool keypad_row_pressed[4] = {0, 0, 0, 0};
bool keypad_col_pressed[4][4] = {
  {0, 0, 0, 0}, {0, 0, 0, 0},
  {0, 0, 0, 0}, {0, 0, 0, 0}
}; // this variable stores columns pressed for each row

// Used to store the sequence of pressed keys
// and operate relays after a SEND(*) input.
// Max of 7 values for the case of an input
// ABCD 10 * where all the relays are turned on.
char inputSequence[7] = {'?', '?', '?', '?', '?', '?', '?'};

// Used to keep track of stored valid inputs.
int inputCount = 0;

// Used to make sure that the required parameters are given
// in the input sequence. First 4 values are set to 1 if A,B,C,
// or D are given, 0 if they are not. Last value determines the
// number of relays to turn on (1-10) or turn off (0).
// Examples:
// input: AB5*        outputSettings: 1 1 0 0 5  (turn on 5 relays on phases A&B)
// input: ABCD10*     outputSettings: 1 1 1 1 10 (turn on all relays on all phases)
// input: A0*         outputSettings: 1 0 0 0 0  (turn off phase A)
int outputSettings[5] = {0, 0, 0, 0, -1};



//-------------------------------------------------//
//-----------------Main code start-----------------//
//-------------------------------------------------//
void setup() {
  Serial.begin(9600);
  initializeKeypadIOpins();
}

void loop() {
  if (detectButtonPress()) {
    Serial.println("Pushed");

    // Pressed key rows were found with
    // detectButtonPress(), columns are found next.
    findWhichKeys();

    // Gets the character of the pressed key. For multiple
    // key presses, we get '?', which will be ingored later.
    char keyChar = getKeyChar();
    Serial.print("key pressed: "), Serial.println(keyChar);

    //
    storeKeyInput(keyChar);

    //
    if (checkForSEND()) {
      Serial.println("SEND");

      bool outputReady = prepareOutput();

      if (outputReady) {
        Serial.println("Required parameters met.");
        activateOutput();
      }
      else {
        Serial.println("Required parameters missing.");
      }
      
      // Resets the current input sequence and is ready
      // for the next input sequence.
      for (int i = 0; i < 7; i++)
        inputSequence[i] = '?';
      inputCount = 0;
      
      outputSettings[0] = 0;
      outputSettings[1] = 0;
      outputSettings[2] = 0;
      outputSettings[3] = 0;
      outputSettings[4] = -1;
    }
    else if (checkForCANCEL()) {
      Serial.println("CANCEL");
      for (int i = 0; i < 7; i++)
        inputSequence[i] = '?';
      inputCount = 0;
    }
    else
      ; // Do nothing. Wait for next input
    Serial.println();
  }

  else;
  //Serial.println("Not pushed  ");
  delay(50);
  delay(150);
}
//-------------------------------------------------//
//-----------------Main code ends------------------//
//-------------------------------------------------//

void initializeKeypadIOpins(void) {
  // Set Mux input/output pins
  muxShield.setMode(keypad_LR_pins[0][0], DIGITAL_IN_PULLUP);
  muxShield.setMode(keypad_LR_pins[0][1], DIGITAL_OUT);
  muxShield.setMode(DC_Load_pins[0][2], DIGITAL_IN_PULLUP);

  // Set channels 0-11 HIGH to prevent
  // turning on relays during initialization
  for (int i = 0; i < 12; i++)
    muxShield.digitalWriteMS(keypad_LR_pins[0][1], i, 1);
  for (int i = 12; i < 16; i++)
    muxShield.digitalWriteMS(keypad_LR_pins[0][1], i, 0);
  Serial.println("Initialization complete.");
}

bool detectButtonPress(void) {
  keypad_row_pressed[0] = !muxShield.digitalReadMS(keypad_LR_pins[0][0], keypad_LR_pins[1][0]);
  keypad_row_pressed[1] = !muxShield.digitalReadMS(keypad_LR_pins[0][0], keypad_LR_pins[1][1]);
  keypad_row_pressed[2] = !muxShield.digitalReadMS(keypad_LR_pins[0][0], keypad_LR_pins[1][2]);
  keypad_row_pressed[3] = !muxShield.digitalReadMS(keypad_LR_pins[0][0], keypad_LR_pins[1][3]);

  return keypad_row_pressed[0] || keypad_row_pressed[1] ||
         keypad_row_pressed[2] || keypad_row_pressed[3];
}

void findWhichKeys(void) {
  for (int i = 0; i < 4; i++) {
    if (keypad_row_pressed[i]) {
      for (int j = 0; j < 4; j++) {
        muxShield.digitalWriteMS(keypad_LR_pins[0][1], keypad_LR_pins[1][j], 1);
        if (muxShield.digitalReadMS(keypad_LR_pins[0][0], keypad_LR_pins[1][i]))
          keypad_col_pressed[i][j] = 1;
        else
          keypad_col_pressed[i][j] = 0;
        muxShield.digitalWriteMS(keypad_LR_pins[0][1], keypad_LR_pins[1][j], 0);
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
      if (keypad_col_pressed[i][j]) keypressCount++;

  if (keypressCount == 1) {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        if (keypad_col_pressed[i][j]) {
          keyID = i * 4 + (j + 1);
          break;
        }
    switch (keyID) {
      case 1: keyChar = 'A'; break;
      case 2: keyChar = '1'; break;
      case 3: keyChar = '2'; break;
      case 4: keyChar = '3'; break;
      case 5: keyChar = 'B'; break;
      case 6: keyChar = '4'; break;
      case 7: keyChar = '5'; break;
      case 8: keyChar = '6'; break;
      case 9: keyChar = 'C'; break;
      case 10: keyChar = '7'; break;
      case 11: keyChar = '8'; break;
      case 12: keyChar = '9'; break;
      case 13: keyChar = 'D'; break;
      case 14: keyChar = '*'; break;
      case 15: keyChar = '0'; break;
      case 16: keyChar = '#'; break;
      default: keyChar = '?';
    }
  }
  else
    Serial.println("Multiple keys pressed");
  return keyChar;
}

void storeKeyInput(char keyInput) {
  inputSequence[inputCount] = keyInput;
  if (inputCount != 6)
    inputCount++;

  Serial.print("input sequence: ");
  for ( int i = 0; i < 7; i++)
    if (inputSequence[i] != '?')
      Serial.print(inputSequence[i]), Serial.print(' ');
  Serial.println();
}

bool checkForSEND(void) {
  for ( int i = 0; i < 7; i++)
    if (inputSequence[i] == '*')
      return true;
  return false;
}
bool checkForCANCEL(void) {
  for ( int i = 0; i < 7; i++)
    if (inputSequence[i] == '#')
      return true;
  return false;
}

bool prepareOutput(void) {
  // First search for ABCD
  for (int i = 0; i < 7; i++) {
    switch (inputSequence[i]) {
      case 'A': outputSettings[0] = 1; break;
      case 'B': outputSettings[1] = 1; break;
      case 'C': outputSettings[2] = 1; break;
      case 'D': outputSettings[3] = 1; break;
    }
  }

  // Now search for value between 0-10.
  // 10 is a special case, where 1 is first found
  // and then 0 must follow.
  for (int i = 0; i < 7; i++) {
    switch (inputSequence[i]) {
      case '1': {
          if (i < 6 && (inputSequence[i + 1] == '?' || inputSequence[i + 1] == '*'))
            outputSettings[4] = 1;
          else if (i < 6 && inputSequence[i + 1] == '0')
            outputSettings[4] = 10;
          else
            return false;
          i++;
          break;
        }
      case '2': outputSettings[4] = 2; break;
      case '3': outputSettings[4] = 3; break;
      case '4': outputSettings[4] = 4; break;
      case '5': outputSettings[4] = 5; break;
      case '6': outputSettings[4] = 6; break;
      case '7': outputSettings[4] = 7; break;
      case '8': outputSettings[4] = 8; break;
      case '9': outputSettings[4] = 9; break;
      case '0': outputSettings[4] = 0; break;
    }
  }
  return (outputSettings[0] || outputSettings[1] ||
          outputSettings[2] || outputSettings[3]) &&
         (outputSettings[4] > -1 && outputSettings[4] < 11);
}

void activateOutput(void){
  //muxShield.
}

