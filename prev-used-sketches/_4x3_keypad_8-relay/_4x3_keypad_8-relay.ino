// assign Shift Register pins
const int SR_OUT = 2;
const int SR_LOAD = 5;
const int SR_CLK_INH = 6;
const int SR_CLK = 7;

// assign the relay output pins
int Relay_IN[8] = {8,9,10,11,12,13,14,15};


void setup()
{
  // assign Shift Register pin I/O
  pinMode(SR_OUT,INPUT);
  pinMode(SR_LOAD,INPUT);
  pinMode(SR_CLK,OUTPUT);
  pinMode(SR_CLK_INH,OUTPUT);
  
  // assign output pins for relays
  for(int i = 0; i < 8; i++)
    pinMode(Relay_IN[i],OUTPUT);

  // Initialize all ouptut pins
  digitalWrite(SR_CLK,LOW);
  digitalWrite(SR_CLK_INH,LOW);
  setOutputRelays(0);  // turns off all relays

  Serial.begin(9600);

  //SR INITIALIZE:
  // initialize the Shift Register by applying a clock
  // a few times and clearing the stored values to zeroes.
  resetSR(),resetSR(),resetSR();
  digitalWrite(SR_CLK_INH,HIGH);
  Serial.println("Shift Register Initialized");
}

void loop()
{
  int keyPushed = digitalRead(SR_LOAD);
  if( keyPushed == LOW)
  {
    Serial.println("PUSHED");
    while( !digitalRead(SR_LOAD) );

    String keyValue = "00000000";
    digitalWrite(SR_CLK_INH,LOW);
    for(int i = 0; i < 8; i++)
    {
      // assign values to string to figure
      // out which key was pressed
      if(digitalRead(SR_OUT))
        keyValue[i] = '1';
      else
        keyValue[i] = '0';
      digitalWrite(SR_CLK,LOW);
      delay(10);
      digitalWrite(SR_CLK,HIGH);
      delay(10);
    }

    /*
    // this part is not working
     // switch statement is expecting an
     // integer arguement for some reason
     switch (keyValue) {
     case "01101110":
     Serial.println("Key pressed: 1");
     break;
     case "01011110":
     Serial.println("Key pressed: 2");
     break;
     case "00111110":
     Serial.println("Key pressed: 3");
     break;
     case "01101101":
     Serial.println("Key pressed: 4");
     break;
     case "01011101":
     Serial.println("Key pressed: 5");
     break;
     case "00111101":
     Serial.println("Key pressed: 6");
     break;
     case "01101011":
     Serial.println("Key pressed: 7");
     break;
     case "01011011":
     Serial.println("Key pressed: 8");
     break;
     case "00111011":
     Serial.println("Key pressed: 9");
     break;
     case "01100111":
     Serial.println("Key pressed: *");
     break;
     case "01010111":
     Serial.println("Key pressed: 0");
     break;
     case "00110111":
     Serial.println("Key pressed: #");
     break;
     default:
     Serial.println("Not Recognized");
     }
     */

    if(keyValue == "01101110")
      Serial.println("Key pressed: 1"), setOutputRelays(1);
    else if(keyValue == "01011110")
      Serial.println("Key pressed: 2"), setOutputRelays(2);
    else if(keyValue == "00111110") 
      Serial.println("Key pressed: 3"), setOutputRelays(3);
    else if(keyValue == "01101101")
      Serial.println("Key pressed: 4"), setOutputRelays(4);
    else if(keyValue == "01011101")
      Serial.println("Key pressed: 5"), setOutputRelays(5);
    else if(keyValue == "00111101")
      Serial.println("Key pressed: 6"), setOutputRelays(6);
    else if(keyValue == "01101011")
      Serial.println("Key pressed: 7"), setOutputRelays(7);
    else if(keyValue == "01011011")
      Serial.println("Key pressed: 8"), setOutputRelays(8);
    else if(keyValue == "00111011")
      Serial.println("Key pressed: 9");
    else if(keyValue == "01100111") 
      Serial.println("Key pressed: *");
    else if(keyValue == "01010111")
      Serial.println("Key pressed: 0"), setOutputRelays(0);
    else if(keyValue == "00110111")
      Serial.println("Key pressed: #");
    else{
      Serial.println("Key not recognized");
      Serial.println("Relays turned off");
      setOutputRelays(0);
    }

    // resets the inputs to zeroes
    resetSR();

    // puts SR into standby for next keypress
    digitalWrite(SR_CLK_INH,HIGH);
  }
  else
  {
    //Serial.println("NOT PUSHED");
  }
  delay(100);
}

void resetSR(void){
  for(int i = 0; i < 8; i++){
    digitalWrite(SR_CLK,LOW);
    delay(10);
    digitalWrite(SR_CLK,HIGH);
    delay(10);
  }
}

void setOutputRelays(int relaysON){
  for(int i = 0; i < 8; i++){
    if(i < relaysON)      // turns on relays with active LOW
      digitalWrite(Relay_IN[i],LOW);
    else                  // turns off relays by setting HIGH
      digitalWrite(Relay_IN[i],HIGH);
  }
}
