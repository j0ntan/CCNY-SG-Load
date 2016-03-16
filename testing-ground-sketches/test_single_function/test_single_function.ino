char inputSequence[14] = {'A', 'B', 'C', 'D',
                          '1', '0', '#', '?',
                          '?', '?', '?', '?',
                          '?', '?'
                         };
int desiredState[4] = { -1, -1, -1, -1};
int currentState[4] = {0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  getDesiredState();

}

void loop() {
  // put your main code here, to run repeatedly:

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
    else;
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
    else;
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
    else;
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
    else;
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
  // (A,B, or C)(A,B, or C)$ D$               (for example BC4 D8)
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
        // This is the case for (A,B, or C)$ (A,B, or C)$
        Serial.println("(A,B, or C)$ (A,B, or C)$ detected");
        if (Acount == 1 && Bcount == 1 && Bpos > Apos) {
          Serial.println("A$ B$ detected");
          // Set state for A
          setAstate(Apos);
          // Set state for B
          setBstate(Bpos);
          // CASE FINISHED
        }
        else if (Acount == 1 && Ccount == 1 && Cpos > Apos) {
          Serial.println("A$ C$ D$ detected");
          // Set state for A
          setAstate(Apos);
          // Set state for C
          setCstate(Cpos);
          // CASE FINISHED
        }
        else if (Bcount == 1 && Ccount == 1 && Cpos > Bpos) {
          Serial.println("B$ C$ D$ detected");
          // Set state for B
          setBstate(Bpos);
          // Set state for C
          setCstate(Cpos);
          // CASE FINISHED
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
    Serial.println("D$ detectd");
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
