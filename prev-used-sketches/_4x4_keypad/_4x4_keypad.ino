#define RK_0 B10000100
#define RK_1 B00011000
#define RK_2 B00011100
#define RK_3 B00010110
#define RK_4 B00101001
#define RK_5 B00101100
#define RK_6 B00100100
#define RK_7 B01001001
#define RK_8 B01001000
#define RK_9 B01000010
#define RK_NUM B10000110
#define RK_STAR B10000001
#define RK_A B00010010
#define RK_B B00100001
#define RK_C B01000011
#define RK_D B10000011

const int pinKM[] = {4,5,6,7};  // four pins to Keyboard Matrix

void setup(void)
{
  Serial.begin(9600);
}

void loop(void) {

  char c = ReadKey(); 
  Serial.println(c);
}

// -------------------  ReadKey -------------------
// wait for a key to be pressed
char ReadKey(void) {
  char c;
  byte r;

  unsigned long ti=millis()+2000;              // simple autorepeat
  while (0!=ReadKeyboardRaw() && ti>millis());  // wait for previeus key to be unpressed..

  do {
    while (0==(r=ReadKeyboardRaw()));
    delay(10);
  }
  while( r!=ReadKeyboardRaw() );
  switch(r) {
    case RK_0: c = '0'; break;   
    case RK_1: c = '1'; break;
    case RK_2: c = '2'; break;   
    case RK_3: c = '3'; break;       
    case RK_4: c = '4'; break;       
    case RK_5: c = '5'; break;   
    case RK_6: c = '6'; break;   
    case RK_7: c = '7'; break;   
    case RK_8: c = '8'; break;   
    case RK_9: c = '9'; break;   
    case RK_NUM: c = '#'; break;
    case RK_STAR:c = '*'; break;
    case RK_A: c = 'A'; break;
    case RK_B: c = 'B'; break;
    case RK_C: c = 'C'; break;
    case RK_D: c = 'D'; break;   
    default: c = '?';
  }
  return c;
}

// -------------------  ReadKeyboardRaw -------------------
byte ReadKeyboardRaw(void) {
  byte rawkey=0;
 
  for(byte i=0;i<4;i++) {
// setup ports..   
    for(byte j=0;j<4;j++) {
      if (i==j) {    // output '0'
        pinMode(pinKM[j], OUTPUT);             
        digitalWrite( pinKM[j], LOW);           
      }
      else { // input
        pinMode(pinKM[j], INPUT);                         
        digitalWrite( pinKM[j], HIGH);                                     
      }
    }
    delay(1);
// read ports..   
    for(byte j=0;j<4;j++) {
      if (i!=j) {
        if (LOW==digitalRead(pinKM[j])) {
          bitSet(rawkey, j);                                   
        }
      }
    }
    if (rawkey!=0x0) {  // key pressed, return key
      bitSet(rawkey, i+4);
      break; //return rawkey;     
    }
  }
  return rawkey;
}



