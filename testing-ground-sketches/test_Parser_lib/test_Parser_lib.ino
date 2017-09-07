#include "Parser_for_load_emulator.h"


Parser parser;

char _input_sequence[12];
String input_sequence;
int i;

void setup() {
  Serial.begin(9600);

  // Balanced
  String balanced_w_DC_1 = "ABC3D2#";
  String balanced_w_DC_2 = "A2B2C2D6#";
  String balanced_wo_DC_1 = "ABC3#";
  String balanced_wo_DC_2 = "A2B2C2#";

  // Unbalanced w/ 3 phases.
  String unbalanced_3_phase_w_DC = "A1B2C3D6#";
  String unbalanced_3_phase_wo_DC = "A1B2C3#";

  // Unbalanced w/ 2 phases.
  String two_phase_A_B_w_DC = "A4B9D2#";
  String two_phase_A_C_w_DC = "A4C9D2#";
  String two_phase_B_C_w_DC = "A4C9D2#";
  String two_phase_A_B_wo_DC = "A4B9#";
  String two_phase_A_C_wo_DC = "A4C9#";
  String two_phase_B_C_wo_DC = "B4C9#";
  String two_phase_AB_w_DC = "AB9D2#";
  String two_phase_AC_w_DC = "AC9D2#";
  String two_phase_BC_w_DC = "AC9D2#";
  String two_phase_AB_wo_DC = "AB9#";
  String two_phase_AC_wo_DC = "AC9#";
  String two_phase_BC_wo_DC = "BC9#";

  //  Unbalanced w/ 1 phase.
  String single_phase_A_w_DC = "A6D8#";
  String single_phase_B_w_DC = "B6D8#";
  String single_phase_C_w_DC = "C6D8#";
  String single_phase_A_wO_DC = "A6#";
  String single_phase_B_wO_DC = "B6#";
  String single_phase_C_wO_DC = "C6#";

  String reset_key = "*";

  //parser.receive_input(balanced_key);
  Serial.println("Ready."); Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    i = 0;
    while (Serial.available() > 0) {
      _input_sequence[i] = char(Serial.read());
      if (i != (12 - 1)) {
        i++;
      }
      delay(10); // needed to give serial bus time to read before exiting while loop.
    }
    if (_input_sequence[i-1] != '*')
      _input_sequence[i] = '#';
    input_sequence = _input_sequence + '\0';
    Serial.print("Main: Sent input string "); Serial.println(input_sequence);

    parser.parse(input_sequence);

    // Reset char array and string.
    input_sequence = "";
    for ( int k = 0; k < 12; k++)
      _input_sequence[k] = '\0';
  }
}
