// source for this code found at:
// http://arduinotronics.blogspot.com/2014/02/sainsmart-i2c-lcd.html

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27 // <<----- Add your address here. Find it from I2C Scanner
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

void setup()
{
  lcd.begin (20, 4); // or (20,4)

  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.setCursor (0, 0); // (column, row)
  lcd.print("SainSmart");
  lcd.setCursor (0, 1);
  lcd.print("I2C 20x04");
}

void loop()
{

}
