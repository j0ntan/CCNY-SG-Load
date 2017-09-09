#include <Scanner.h>
#include <Parser.h>

/*
This example shows how a user input is processed by the Scanner and
converted into tokens. Each character in the input corresponds to one
of the elements in the Scanner Alphabet, whether legal or not. After
tokenization, the Parser can translate the tokens into a set of integers
that correspond to the state of the load, i.e. how many relays are ON for
each phase, icluding DC. */

// sample inputs
String UnbalancedInput("A1B2C3D1"),
        BalancedInput("ABC4D2"),
        MixedCase("A2b3c2D0"),
        InvalidChar("ABC3.D2"),
        SingleNumber("6"),
        MissingACNumber("ABC");

void PrintDisplayMsgToSerial(const String& msg) {
    Serial.println(msg.substring(0, 20));
    Serial.println(msg.substring(20, 40));
    Serial.println(msg.substring(40, 60));
    Serial.println(msg.substring(60));
}

void PrintSwitchingValues(const int* values) {
    Serial.println("Load relays values are:");
    for(int i=0; i<4; ++i) {
        Serial.print(values[i]);
        Serial.print(' ');
    }
    Serial.println();
}

void scanAndParse(const String& inputStr) {
    Serial.print("Input string is ");
    Serial.println(inputStr);

    Scanner scanner;
    if( scanner.scan(inputStr) ) {
        Serial.println("Scan was successful");
        Parser parser;
        if( parser.parse(scanner.getTokens()) ) {
            Serial.println("Parse was successful");
            PrintSwitchingValues(parser.getSwitchingValues());
        }
        else {
            Serial.println("Error while parsing");
            PrintDisplayMsgToSerial(parser.getErrMsg());
        }
    }
    else {
        Serial.println("Scan shows input error");
        PrintDisplayMsgToSerial(scanner.getErrMsg());
    }
    Serial.println();
}

void setup() {
    Serial.begin(19200);
    Serial.println("Ready");

    // scan and parse several load configuration inputs
    scanAndParse(UnbalancedInput);
    scanAndParse(BalancedInput);
    scanAndParse(MixedCase);

    // produce error messages at scan or parse stage
    scanAndParse(InvalidChar);
    scanAndParse(SingleNumber);
    scanAndParse(MissingACNumber);
}

void loop() {}
