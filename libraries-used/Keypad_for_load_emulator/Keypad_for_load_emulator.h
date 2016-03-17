//--------------------Preprocessor directives--------------------//

/*
 Refer to the documentation for the working principles of the 
keypad and to see how the Arduino accepts keypresses. The currently 
installed lab setup uses pins 30-33 & 34-37 for ROW1-ROW4 &
COL1-COL4, respectively. */
// Assign the ROW1-ROW4, COL1-COL4 keypad pins here:
#define ROW1 30
#define ROW2 31
#define ROW3 32
#define ROW4 33

#define COL1 34
#define COL2 35
#define COL3 36
#define COL4 37


//--------------------------------------------------------------//
//-----------------------Global Variables-----------------------//
//--------------------------------------------------------------//

// Stores the Arduino pin numbers that connect to the keypad.
unsigned int keypad_row_pins[4] = {ROW1, ROW2, ROW3, ROW4};
unsigned int keypad_col_pins[4] = {COL1, COL2, COL3, COL4};

/*
Used to keep track of pressed buttons by their rows and 
column status. False or 0 means the key is not pressed. When 
the key is pressed and the keypad pins are read, these values 
change to True or 1. */
bool keypad_row_pressed[4] = {0, 0, 0, 0};
bool keypad_button_pressed[4][4] = {
  {0, 0, 0, 0}, {0, 0, 0, 0},
  {0, 0, 0, 0}, {0, 0, 0, 0}};

/*
Used to store characters according to the keys that are 
pressed and operate relays after an ENTER(#) key is pressed.
Max of 12 values for the case of the input "A16 B16 C16 D8#" 
(without spaces) where all the relays are turned on. */
char inputSequence[12] = {
  '?', '?', '?', '?',
  '?', '?', '?', '?',
  '?', '?', '?', '?'};

// Used to keep track of stored valid inputs.
int inputCount = 0;

/*
Stores the state of the relays AFTER the user passed some input
sequence. The user enters some input on the keypad, it gets stored
in this array, and then it gets processed and passed to the shift
registers so that it activates the relays.
First 3 values correspond to number of resistors turned on for 
each AC phase. Last value is for DC load. 
Values can be either:
1-16 - that phase (or DC) will have this many resistors connected.
   0 - that phase (or DC) will have no resistors connected. 
  -1 - that phase (or DC) did not receive a value, it will keep its
       previous state and remain unchanged, and 
       nothing will be sent to the shift registers or relays. */ 
int desiredState[4] = {-1,-1,-1,-1};

/*
Similar to the desiredState array, except that it stores the number
of resistors turned on for each phase (or DC). 
Values range from 0-16. */
int currentState[4] = {0,0,0,0};

//--------------------------------------------------------------//
//---------------------Global Variables end---------------------//
//--------------------------------------------------------------//





//--------------------------------------------------------------//
//---------------------Function Definitions---------------------//
//--------------------------------------------------------------//

void initialize_keypad_pins(void) {
  /*
  This function assigns the Arduino INPUT and OUTPUT pins connected
  to the keypad pins. Also, sets the initial state of OUTPUT pins to
  LOW state (becuase, by default, no keys are pressed when the Arduino 
  is initially powered on). */

  // Initialize Arduino input pins for keypad.
  for (int i = 0; i < 4; i++)
    pinMode(keypad_row_pins[i], INPUT_PULLUP);

  // Initialize Arduino ouput pins for keypad to LOW state.
  for (int i = 0; i < 4; i++) {
    pinMode(keypad_col_pins[i], OUTPUT);
    digitalWrite(keypad_col_pins[i],0);
  }
  Serial.println("Keypad Initialized.");
}

bool detect_button_press(void) {
  /*
  This function checks if any of the keypad row pins have changed
  from HIGH to LOW, indicating that button has been pushed. If a
  button was pressed, a True value for that button's row will be
  stored in the array 'keypad_row_pressed'. */
  
  keypad_row_pressed[0] = !digitalRead(keypad_row_pins[0]);
  keypad_row_pressed[1] = !digitalRead(keypad_row_pins[1]);
  keypad_row_pressed[2] = !digitalRead(keypad_row_pins[2]);
  keypad_row_pressed[3] = !digitalRead(keypad_row_pins[3]);

  // Return True if 1 or more row pins are set to True.
  return keypad_row_pressed[0] || keypad_row_pressed[1] ||
         keypad_row_pressed[2] || keypad_row_pressed[3];
}

void identify_pressed_buttons_row_and_col(void) {
  /*
  This function finds the row and column of the pressed buttons
  by toggling the column pins and reading the row pins. Refer 
  to the documentation for more details on this method. Buttons 
  that are pressed will have a True value stored in the 4x4
  array 'keypad_button_pressed'. */
  
  // Begin by looking at first row pins.
  for (int row = 0; row < 4; row++) {
    if (keypad_row_pressed[row]) {
      
      // Begin looking at each column pin within a row.
      for (int col = 0; col < 4; col++) {
        
        // Begin toggling and recording row pin changes.
        digitalWrite(keypad_col_pins[col], HIGH);
        if (digitalRead(keypad_row_pins[row]))
          keypad_button_pressed[row][col] = true;
        else
          keypad_button_pressed[row][col] = false;
        digitalWrite(keypad_col_pins[col], LOW);
        
        // use this line to print 4x4 results
        //Serial.print(keypad_button_pressed[row][col]);
        //Serial.print(" ");
        }
      }
    else {
      for (int col = 0; col < 4; col++) {
        keypad_button_pressed[row][col] = false;
        // use this line to print 4x4 results
        //Serial.print(keypad_button_pressed[row][col])
        //Serial.print(" ");
      }
    }
    // use this line to print 4x4 results
    //Serial.println();
  }
}

char get_key_char(void) {
  /*
  This function uses the array 'keypad_button_pressed' to find the
  alphanumeric character that the user intended to enter. There are
  16 elements in this 4x4 keypad. This function assigns an ID
  to each element in a (row,column) pair. The layout of the currently
  installed keypad and its corresponding ID are as follows: 
  (Layout)    (ID numbers)
  1 2 3 A      1  2  3  4
  4 5 6 B      5  6  7  8
  7 8 9 C      9 10 11 12
  * 0 # D     13 14 15 16
  Using the assigned ID number, we can just match the correct
  alphanumeric character. For the case where more than one button
  has been pressed, the function returns the '!' character so that
  other functions can receive this information. */

  // Find out how many buttons were pressed and its (row,col) values.
  int buttons_pressed = 0;
  int row = 0, col = 0;
  for (int temp_row = 0; temp_row < 4; temp_row++)
    for (int temp_col = 0; temp_col < 4; temp_col++)
      if (keypad_button_pressed[temp_row][temp_col]) {
        buttons_pressed++;
        row = temp_row;
        col = temp_col;
        /*
        NOTE: The variables 'row' and 'col' are used later to get the
        ID for the pressed button. If more than one button is pressed
        then these variables will change multiple times. But this 
        doesn't matter because in that case we return the character 
        '!' by default, there is no need to use 'row' or 'col'. */
      }

  // If only one button is pressed, get the key_ID
  if (buttons_pressed == 1) {
    // Calculate key_ID using the layout shown above.
    int key_ID = row * 4 + (col + 1);

    // Return the alphanumeric character correctly matched with key_ID.
    switch (key_ID) {
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
      default: return '?'; // Something is wrong if this is returned.
    }
  }
  else
    // More than one key was pressed so we return the '!' character.
    return '!';
}

void store_keypresses(char keyInput) {
  /*
  This function stores a single keypress to an array so that the Arduino
  can keep track of the complete input sequence and display it back to 
  the user. */
  
  // This is the case of more than one simultaneous button press.
  if (keyInput == '!') {
    // In this case, just print a message and ignore the input.
    Serial.println("Multiple keypress detected.");
    Serial.println("Input sequence unchanged");
  }
  else {
    /*
    Check that the first keypress is a letter. Otherwise, ignore the 
    keypress until a letter has been pressed. This is just so that the 
    format of the input sequence follows a structure, such as 
    'letter' 'number' 'other letter' 'other number' 'etc'. */
    if (inputCount == 0 &&
        keyInput != 'A' && keyInput != 'B' && 
        keyInput != 'C' && keyInput != 'D' )
      Serial.println("First input must be ABC or D");
    else {
      inputSequence[inputCount] = keyInput;
      if (inputCount < 11)
        /*
        Variable advances the position for storing inputs in the array. 
        When 'inputCount' reaches 11, the last character in 
        'inputSequence' will just be written over for any additional 
        keypresses. */
        inputCount++;
    }
  }

  Serial.print("input sequence: ");
  for ( int i = 0; i < 13; i++)
    if (inputSequence[i] != '?')
      Serial.print(inputSequence[i]), Serial.print(' ');
  Serial.println();
}

bool check_keypress_ENTER(char keyChar) {
  /*
  Simply checks if the key that was pressed is the ENTER key. In our 
  case, we designated the '#' key to be the ENTER key. */
  if (keyChar == '#')
    return true;
  else
    return false;
}

bool check_inputSequence_valid(void) {
  int letterCount = 0;  // counts how many total letters
  int Acount = 0;       // counts letter A occurence
  int Bcount = 0;       // ...
  int Ccount = 0;       // ...
  int Dcount = 0;       // counts letter D occurence
  int NumCount = 0;     // counts occurence of numbers

  
  /* Each values corresponds to the occurence of a single digit number.
  For example, the first value records how many zeros are in 
  inputSequence. The next value records how many 1's there are, etc. */
  int DigitCount[10] = {0, 0, 0, 0, 0, 
                        0, 0, 0, 0, 0};
  int tensCount = 0; // Increases when the number 1 is followed by 
                     // another number, i.e. 11, 12, ..., 16.
  int invalidChars = 0;

  // Count the occurence of letters and numbers.
  for ( int i = 0; i < 12; i++) {
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
    if (i < 11 && inputSequence[i] == '1' &&
        ( inputSequence[i + 1] == '0' ||  inputSequence[i + 1] == '1' ||
          inputSequence[i + 1] == '2' ||  inputSequence[i + 1] == '3' ||
          inputSequence[i + 1] == '4' ||  inputSequence[i + 1] == '5' ||
          inputSequence[i + 1] == '6'
        )
       )
      tensCount++;

    /*
    Check that a number doesn't follow another number
    except for 1 followed by 0-6. */
    if (i < 11 && inputSequence[i] == '1' &&
        (inputSequence[i + 1] == '7' || inputSequence[i + 1] == '8' ||
         inputSequence[i + 1] == '9') ) {
      Serial.println("Numbers not valid, must be within 0-16.");
      Serial.println("Re-enter input sequence.");
      return false;
    }
    else if (
      /*
      Check for other invalid combination of two-digit numbers, in the
      range of 20-99. */
      i < 11 && (
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

  /*
  Check that there are no invalid characters
  in the input sequence. (Still not implemented) */
  if (invalidChars > 0) {
    Serial.println("Invalid Characters detected");
    Serial.println("Re-enter input sequence.");
    return false;
  }
  
  // Check for a max of 4 letters (ABCD) and 7 numbers (16,16,16,8)
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
  // NOTE: Consider re-writing this else if statement.
  else if (Acount == 1 && Bcount == 1 && Ccount == 1) {
    if (inputSequence[1] != 'A' && inputSequence[1] != 'B' &&
        inputSequence[1] != 'C' && inputSequence[1] != 'D' )
        ; // There is a number in second char in sequence.
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

//--------------------------------------------------------------//
//-------------------Function Definitions end-------------------//
//--------------------------------------------------------------//


//--------------Preprocessor directives terminators--------------//
#undef ROW1
#undef ROW2
#undef ROW3
#undef ROW4

#undef COL1
#undef COL2
#undef COL3
#undef COL4