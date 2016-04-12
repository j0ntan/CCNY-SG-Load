#include <LCD_for_load_emulator.h>

Liquid_Crystal_Display lcd;

void setup() {
  Serial.begin(19200);

  lcd.begin();

  /*
    Strings to show different display speeds. The displayed messages
  appear on the LCD for 3 seconds display time, on the normal speed,
  and then allows the next functon to execute. The speed can be 
  varied as slow, fast, or faster, where the display time varies as 
  4 seconds, 2 seconds, and 1.5 seconds, respectively.
    A special case is a static display, where there is no display time.
  The message is shown on the LCD and the next function is immediately
  allowed to execute. This is useful when displaying an idle state
  message and immediately look for the next input. */
  String normalMSG, slowMSG, fastMSG, fasterMSG, staticMSG, normalMSG2;

  /*
  Strings must be 80 characters long in order to fit on the LCD, 
  which has 4 rows with 20 characters on each row. */
  normalMSG  = "                    ";
  normalMSG += "   The first msg.   ";
  normalMSG += "   Normal speed.    ";
  normalMSG += "                    ";
  
  slowMSG  = "                    ";
  slowMSG += "  The second msg.   ";
  slowMSG += "    Slow speed.     ";
  slowMSG += "                    ";

  fastMSG  = "                    ";
  fastMSG += "   The third msg.   ";
  fastMSG += "    Fast speed.     ";
  fastMSG += "                    ";
  
  fasterMSG  = "                    ";
  fasterMSG += "  The fourth msg.   ";
  fasterMSG += "   Faster speed.    ";
  fasterMSG += "                    ";
  
  staticMSG  = "                    ";
  staticMSG += "  The fifth msg.    ";
  staticMSG += "    Static msg.     ";
  staticMSG += "                    ";
  
  normalMSG2  = "                    ";
  normalMSG2 += "   The sixth msg.   ";
  normalMSG2 += "   Normal speed.    ";
  normalMSG2 += "                    ";
  
  /*
  Ooen the serial monitor. An initialization message for the CCNY load
  will appear. After this message, we can see the variable speeds for
  displaying messages. */
  Serial.println("Ready to show variable speeds.");
  delay(4000);
  lcd.showMessage(normalMSG);
  lcd.showMessage(slowMSG, "slow");
  lcd.showMessage(fastMSG, "fast");
  lcd.showMessage(fasterMSG, "faster");
  lcd.showMessage(staticMSG, "static");
  Serial.println("Some other functions executing...");
  delay(3500);
  Serial.println("...");
  delay(800);
  Serial.println("...");
  delay(800);
  Serial.println("...");
  delay(800);
  Serial.println("...");
  delay(800);
  Serial.println();
  lcd.showMessage(normalMSG2);

  Serial.println("Done showing messages.");
}

void loop() {}
