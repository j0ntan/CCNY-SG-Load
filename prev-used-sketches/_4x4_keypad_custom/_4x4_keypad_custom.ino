// Assign the I/O pins for the keypad L and R pins
const int KP_L[4] = {8, 9, 10, 11};
const int KP_R[4] = {4, 5, 6, 7};

// These variables are used later
// to detect a button press
int KP_L_status[4] = {1, 1, 1, 1};
int KP_R_status[4] = {0, 0, 0, 0};

bool buttonpress = false;

void setup() {
  // define I/O type for keypad pins
  // & initialize outputs
  for (int i = 0; i < 4; i++)
    pinMode(KP_L[i], INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    pinMode(KP_R[i], OUTPUT);
    digitalWrite(KP_R[i], LOW);
  }

  Serial.begin(9600);
  Serial.println("Keypad Initialized");
}

void loop() {
  buttonpress = detectKeyPress_row();


  // a key(s) has been pressed in one row
  if (buttonpress)
  {
    int rowNum = getRow();

    // detects press of one row and
    // one col for a single keypress
    if (detectKeyPress_col(rowNum)) {
      Serial.println("Pushed");
      int colNum = getCol();
      Serial.print("row: ");
      Serial.print(rowNum);
      Serial.print(" col: ");
      Serial.println(colNum);
    }
    else
      Serial.println("Not Pushed");
  }

  // no key is pressed
  else
    Serial.println("Not Pushed");

  delay(400);
}

bool detectKeyPress_row(void) {
  for (int i  = 0; i < 4; i++)
    KP_L_status[i] = digitalRead(KP_L[i]);

  return !KP_L_status[0] ^ !KP_L_status[1] ^
         !KP_L_status[2] ^ !KP_L_status[3];
}

int getRow() {
  return 1 * !KP_L_status[0] + 2 * !KP_L_status[1] +
         3 * !KP_L_status[2] + 4 * !KP_L_status[3];
}

bool detectKeyPress_col(int rowNum) {
  int colsPressed = 0;

  for (int i = 0; i < 4; i++) {
    digitalWrite(KP_R[i], HIGH);
    if (digitalRead(KP_L[rowNum - 1])) {
      colsPressed++;
      KP_R_status[i] = 1;
    }
    else
      KP_R_status[i] = 0;
    digitalWrite(KP_R[i], LOW);
  }
  if (colsPressed == 1)
    return true;
  else
    return false;
}

int getCol(void) {
  return 1 * KP_R_status[0] + 2 * KP_R_status[1] +
         3 * KP_R_status[2] + 4 * KP_R_status[3];
}
