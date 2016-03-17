//--------------------Preprocessor directives--------------------//

/*
Refer to the documentation for details on cascading of the shift 
registers and for the connection of the shift registers to
Arduino and the relays. The currently installed lab setup uses the
Arduino pins 4-7 for the Shift Register Clock, Output Clock, Data 
input , and the Shift Register Clear pins of the shift register, 
respectively. */
// Assign the shift register pins here:
#define DATA_STREAM   7
#define SHIFT_REG_CLK 6
#define OUTPUT_CLK    5
#define SHIFT_REG_CLR 4

// Assign the delay values, in milliseconds, for both clocks
#define SHIFT_REG_CLK_DELAY 10
#define OUTPUT_CLK_DELAY 10




//-----------------------Global Variable------------------------//

// Stores the Arduino pin numbers that connect to the DC relays.
double DC_pins[8] = {22,23,24,25,26,27,28,29};

/*
An array that holds the states for each relay. The first 16 array 
values are the states for Phase A. The next 16 values are for Phase B,
followed by Phase C. */
bool data_stream_states[48];



//--------------------------------------------------------------//
//---------------------Function Definitions---------------------//
//--------------------------------------------------------------//

void initialize_Shift_Reg(void) {
  /*
  This function initializes the shift registers by setting the shift
  register clear pin to HIGH state and sets other shift register pins
  to default LOW state. Also initializes data_stream_states to False 
  value, becuase relays should be OFF by default at initial power up.*/
  
  // Set Arduino pins for shift register as outputs.
  pinMode(SHIFT_REG_CLK, OUTPUT);
  pinMode(OUTPUT_CLK,    OUTPUT);
  pinMode(DATA_STREAM,   OUTPUT);
  pinMode(SHIFT_REG_CLR, OUTPUT);
  
  // Set initial states of ouptut pins.
  digitalWrite(SHIFT_REG_CLK, LOW);
  digitalWrite(OUTPUT_CLK,    LOW);
  digitalWrite(DATA_STREAM,   LOW);
  digitalWrite(SHIFT_REG_CLR, HIGH);
  
  // Set initial default False states of the 'data_stream_states' array.
  for(int i = 0; i < 48; i++) {
    data_stream_states[i] = false;
  }
  
  Serial.println("Shift Registers Initialized.");
}

bool final_check_of_states(void) {
  /*
  This function checks that the values stored in the array 
  'desiredState' are within an expected set of values. For example,
  it does not make sense if there are values like 99 or -4, becuase
  values must be between -1 to 16 for AC or 0 to 8 for DC. */
  
  // Check for abnormal AC state values (must be within -1 to 16).
  for (int i = 0; i < 4; i++) {
    if (desiredState[i] < -1 || desiredState[i] > 16) {
      return false;
    }
  }
    
  
  // Check for abnormal DC values (must be within 0 to 8).
  if(desiredState[3]>8) {
	  return false;
  }
}

void create_data_stream(void) {
  /*
  This function creates the data stream that will be passed to shift 
  registers. The values in the array 'desiredState' are used to create 
  a sequence of 16 bits, once for each phase. Each bit corresponds to
  a relay and 0 or 1 corresponds to whether that relay is ON or OFF. 
  The relays for DC are handled by Arduino directly, not by shift 
  registers.
  
  For example:
  desiredState[] = {2,0,-1,4}
  Phase A: 0000000000000011
  Phase B: 0000000000000000
  Phase C: (no input recieved for this phase, so copy the previous
            state that is stored in the array 'currentState')
  Phase D: (handled by Arduino digital I/O pins) */
  
  Serial.println("Final check passed, output is activated.");
  
  /* Go through each value in the array 'desiredState', which 
  corresponds to the state of a single phase or DC relays. */
  for (int i = 0; i < 4; i++) {

    /*
    Either no input received or received a value that is the same as
    the previous value (e.g. 'A1#' and then 'A1B2C3#'). */
    if (desiredState[i] == -1 || desiredState[i] == currentState[i]) {
      Serial.print("No changes made to ");
      Serial.print(char(int('A') + i));
      Serial.println(".");
      
      for (int j = 0; j < 16; j++) {
        if (j < currentState[i])
          data_stream_states[j] = true;
        else
          data_stream_states[j] = false;
      }
    }
    
    /*
    Received a state that is different than previous state (e.g.'B2#'
    and then 'B5#'). So we use the array 'desiredState' to create the
    data stream. */
    else if (desiredState[i] > currentState[i]) {
      Serial.print(char(int('A') + i)), Serial.println(" increased");
      
      for (int j = 0; j < 16; j++)
        if (j < desiredState[i])
          data_stream_states[j + (i * 16)] = true;
        else
          data_stream_states[j + (i * 16)] = false;
    }
    
    /*
    Something went wrong and a deep inspection is needed*/
    else {
        Serial.print("Couldn't get desired state of ");
        Serial.print(char(int('A') + i));
        Serial.println(". No changes made.");
    }
  }
}

void send_data_and_activate_relays(void) {
  /*
  This function sends the data stream, which corresponds to the states
  of the relays (True or False for ON or OFF, respectively), to the 
  shift registers. Then it signals the shift registers to activate this
  data stream to its output pins so that the relays can recieve their 
  respective states (ON or OFF).*/
  
  /*
   Send data stream to the shift registers in the following order:
   Phase C, relay 16
   Phase C, relay 15
   ...
   Phase C, relay 1
   Phase B, relay 16
   ...
   Phase B, relay 1
   (same for Phase A) */
  for (int i = 48; i > 0; i--) {
    digitalWrite(DATA_STREAM, data_stream_states[i - 1]);
    
    digitalWrite(SHIFT_REG_CLK, HIGH);
    delay(SHIFT_REG_CLK_DELAY);
    digitalWrite(SHIFT_REG_CLK, LOW);    
  }
  
  // Signal shift registers to activate the outputs.
  
  //Serial.println("READY"); delay(3000);
  
  digitalWrite(OUTPUT_CLK, HIGH);
  delay(OUTPUT_CLK_DELAY);
  digitalWrite(OUTPUT_CLK, LOW);
  Serial.println("Finished sending.");
}

void activateOutput(void) {
  /*
  This function is called to send the data stream to the shift registers
  and turn on the relays. Before this happens, there is a final check up
  on the values in the array 'desiredState' to make sure that we don't
  have any unusual values. */
  bool states_are_valid = final_check_of_states();
  
  if(states_are_valid == true) {
    create_data_stream();
    send_data_and_activate_relays();
    //Serial.println("Finished output.");
  }
  else {
    Serial.println("Final check has failed. Investigate further.");
    Serial.println("Output not changed");
  }  
}

//--------------------------------------------------------------//
//-------------------Function Definitions end-------------------//
//--------------------------------------------------------------//


//--------------Preprocessor directives terminators--------------//
#undef SHIFT_REG_CLK
#undef OUTPUT_CLK
#undef DATA_STREAM

#undef SHIFT_REG_CLK_DELAY
#undef OUTPUT_CLK_DELAY
