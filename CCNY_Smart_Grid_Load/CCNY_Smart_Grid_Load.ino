// Author: Jonathan Gamboa
// version: 4.3
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/CCNY_Smart_Grid_Load.h"

void processInput(const String &);      // function prototype

HardwareSerial &xbee = Serial3;         // create an alias
Keypad keypad;                          // use default pins
Display disp(Serial, xbee, keypad);     // use default pins
SDprofile sd;                           // record SD connected status
ShiftRegister shiftReg(7, 5, 6, 4);     // chosen pins for shift register

Encoder encoder;

void setup() {
    Serial.begin(19200); Serial.setTimeout(10);
    xbee.begin(9600); xbee.setTimeout(10);
    shiftReg.begin();

    disp.begin(20, 4);                  // pass num of cols and rows

    // initialize DC relays
    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    digitalWrite(22, LOW);
    digitalWrite(23, LOW);

    // Done with Arduino initialization, now check SD card

    if(sd.checkConnected()==false) {
        String sdConnMsg;
        sdConnMsg.concat( F("    SD card not     ") );
        sdConnMsg.concat( F(" detected. Initial  ") );
        sdConnMsg.concat( F("check failed. Please") );
        sdConnMsg.concat( F("re-check connection.") );

        disp.showMessage(sdConnMsg, Display::Speed::SLOW);
    }

    // Done all initializations, go into idle state
    disp.showMessage(encoder.getIdleState(), Display::Speed::STATIC);
}

void loop() {
    if(keypad.isPressed()) {
        KeypadCol kCol(keypad);
        while(kCol.isCollecting())
            if(keypad.isPressed()) {
                kCol.collectKeypress();
                disp.showMessage(kCol.getColMsg(), kCol.getSpeed()); // update display
                kCol.waitKeyRelease();
            } // else do nothing in-between keypresses, until done collecting

        // send to scanner, etc...
        if(kCol.getInput().length()) processInput(kCol.getInput());
        else disp.showMessage(encoder.getIdleState(), Display::Speed::STATIC);
    } // end if, check keypad source
    else if(Serial.available()) {
        // allow buffer time to fill
        delay(10);

        SerialCol serCol(Serial.readString());
        disp.showMessage(serCol.getColMsg(), serCol.getSpeed()); // update display
        processInput(serCol.getInput());
    }
    else if(xbee.available()) {
        // allow buffer time to fill
        delay(200);

        if(xbee.available() < 12) { // dSPACE spams with data, User doesn't
            SerialCol xbCol(xbee.readString());
            disp.showMessage(xbCol.getColMsg(), xbCol.getSpeed()); // update display
            processInput(xbCol.getInput());
        }
        else {
            dspaceCol dsCol(xbee);
            if(dsCol.profileModeRequested()) {
                dsProfileCol dsPCol(xbee, sd);
                if(dsPCol.openFile()) {
                    disp.showMessage(dsPCol.getColMsg(), dsPCol.getSpeed());// display file opened msg

                    while(dsPCol.isCollecting())
                        if(dsPCol.readLine()) {
                            processInput(dsPCol.getInput());
                            delay(dsPCol.getDelay());
                        }
                }
                else {
                    disp.showMessage(dsPCol.getColMsg(), dsPCol.getSpeed()); // display error msg
                    disp.showMessage(encoder.getIdleState(), Display::Speed::STATIC);
                }
            }
            else {
                dsManualCol dsMC(xbee);

                while(dsMC.isCollecting()) {
                    dsMC.collectInt();
                    disp.showMessage(dsMC.getColMsg(), dsMC.getSpeed());
                }

                disp.showMessage(dsMC.getColMsg(), dsMC.getSpeed()); // display input or cancel msg
                if(dsMC.getInput().length()) processInput(dsMC.getInput());
                else disp.showMessage(encoder.getIdleState(), Display::Speed::STATIC);
            }
        } // end dspace collectors
    } // end else if xbee avail
}


void processInput(const String &input) {
    Scanner scanner;
        if(scanner.scan(input)) { // if scan successful
            Parser parser;
            if(parser.parse(scanner.getTokens())) { // if parse successful
                encoder.encode(parser.getSwitchingValues());

                // Activate AC relays
                shiftReg.outputData(encoder.getACEncodedValues(), 6);
                // Activate DC relays
                int relaysOn  = encoder.getDCValue();
                switch(relaysOn) {
                    case 0:
                        digitalWrite(22, LOW);
                        digitalWrite(23, LOW);
                        break;
                    case 1:
                        digitalWrite(22, HIGH);
                        digitalWrite(23, LOW);
                        break;
                    case 2:
                        digitalWrite(22, HIGH);
                        digitalWrite(23, HIGH);
                        break;
                };
            }
            else {
                disp.showMessage(parser.getErrMsg(), Display::Speed::NORMAL);
            }
        }
        else {
            disp.showMessage(scanner.getErrMsg(), Display::Speed::NORMAL);
        }
    disp.showMessage(encoder.getIdleState(), Display::Speed::STATIC);
}
