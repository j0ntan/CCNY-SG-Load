/*
FUNCTION PROTOTYPES:
The following functions are USED in this file but they are DEFINED
in a DIFFERENT  .h  file. Becuase of this, we must include these
protoypes so that the code may compile (refer to C++ documentation
on "function protoypes" or "function declaration"). */
/*
void LCDprintCurrentState(int []);
void LCDprintInputSequence(char, char []);
void LCDprintInputSequence(char []);
void LCDprintInvalidInput(void);
*/



void look_for_input(void) {
  /*
  This is the main function that runs in the Arduino loop(). It will
  look for user input by either the keypad or using the Arduino 
  serial monitor (for testng and debugging purposes). Future update
  will include ability to take input from Xbee module. */
  
  // Look for any keypress on the keypad
  if (detect_button_press()) {
    Serial.println("Keypress detected.");


    /*
    Now that one or more buttons have been pressed, we need to find
    their corresponding row and column and store in an array. */
    identify_pressed_buttons_row_and_col();

    /*
    Using the array that stores row and column of pressed keys, we
    find which button was pressed and its corresponding mark on the 
    keypad. If more than one key was pressed, we indicate this with 
    the character '!'. */
    char key_char = get_key_char();
    Serial.print("key pressed: "), Serial.println(key_char);

    /*
    Store the obtained character into an array. Every press of a button
    will get stored in the array 'inputSequence'. This array will be 
    parsed and determines which relays to turn on. The array has a
    maximum size of 12 characters. If the user reaches the 12th 
    character and presses more keys, then those additional keys will
    just keep overwriting the 12th character until an ENTER or CANCEL
    key is pressed. Also, for the case where multiple keys were pressed
    at the same time, the '!' character will cause the function to 
    ignore this input attempt and just wait for the next keypress. */ 
    store_keypresses(key_char);

    /*
    Update the LCD to show the user the keypresses that the Arduino has
    stored. Future update may include smarter parsing and displaying of
    the entered kepresses. For example, the LCD should be able to
    display the user's keypresses in a nicely formated way, using 
    spaces. At the moment, the LCD just displays a continuous string. */
    LCDprintInputSequence(key_char, inputSequence);

    /*
    Now we have to consider three cases. These are: 1) ENTER button was 
    pressed, 2) CANCEL button was pressed, or 3) a letter/number was 
    pressed. Each case will involve different following steps. */
    
    
    /*
    Case 1: ENTER button was pressed. Next, the input sequence must be
    checked to make sure that there are no obvious errors, e.g. an input
    sequence like 'A18 B44 A3 CB #'. In that example we see errors where
    number is out of range (A18), a number might be mistakenly repeated
    (B44), a letter was used twice (A18 ... A3), a letter not followed by
    a number (CB). After checking the input, we parse the input in order 
    to obtain the correct relay states. The accepted format is something
    like 'A1 B2 C3 #', and these characters get stored in an array, but 
    now we must give a meaning to these characters thorugh the parsing 
    function. After parsing, we activate the output and send the data 
    stream to the shift registers so that they may control the relays. */
    if (check_keypress_ENTER(key_char)) {
      Serial.println("ENTER");

      if (check_inputSequence_valid()) {
        Serial.println("First input validation passed.");

        /*
        Use the array that holds the sequence of characters to assign
        a state for the relays that the user wants to obtain. This means
        parsing the inputSequence array and recording the states of the
        relays into a separate array. */
        getDesiredState();

        /*
        Use the array that holds the states of the relays to send the 
        data stream to the shift registers so that they can turn ON or 
        OFF the appropriate relays. */
        activateOutput();

        // A delay between activating each phase.
        delay(2000);
        LCDprintCurrentState(currentState);
      }
      else
        // Error message will appear in serial window with some 
        // description.
        // User notofied on LCD on invalid input.
        LCDprintInvalidInput();


      // Get ready for the next input
      resetSequence();
      LCDprintCurrentState(currentState);

    }
    else if (checkForCANCEL(key_char)) {
      Serial.println("CANCEL");

      // Get ready for the next input
      resetSequence();
      delay(2000);
      LCDprintCurrentState(currentState);
    }
    else
      ; // Do nothing. Wait for next input

    Serial.println();
    Serial.println();

    //LCDprintCurrentState(currentState);

  }
  else if (Serial.available() > 0) {
    // read and store serial data and use some previous functions.
    // at this point, it's as if the keypad enter key was pressed,
    // so we continue with proceeding functions as above.
    readAllSerialInput();

    if (check_inputSequence_valid()) {
      Serial.println("First input validation passed.");
      LCDprintInputSequence(inputSequence);
      // Extracts the desired output state from the input
      // sequence and stores this in an array. The array follows
      // as [Astate, Bstate, Cstate, Dstate]. For example,
      // an input of A 8 C 3 D 4 gives [8,-1,3,4], where
      // -1 means phase B will not change its current state.
      getDesiredState();


      // Makes the appropriate changes to the output current state.
      activateOutput();

      delay(2000);
      LCDprintCurrentState(currentState);
    }
    else; // Error message will appear with some description.

    resetSequence();
    Serial.println();

    //LCDprintCurrentState(currentState);
  }
  else
    ; // Do nothing. Wait for next input
}
